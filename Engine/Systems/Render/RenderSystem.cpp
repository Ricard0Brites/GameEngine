#include "RenderSystem.h"
#include "Component/RenderComponent.h"

#include <iostream>
#include <utility>
#include <memory>

#include "Logger.h"
#include "Windows/Spawnable/SpawnableWindow.h"
#include "Core/Object/Object.h"

// Static Initializations
const std::string RenderSystem::LogCategory = "D3D12_Renderer";
std::vector<RenderComponent> RenderSystem::RenderComponentsCache = {};
uint64_t RenderSystem::ComponentIDCounter = 0;

RenderSystem::RenderSystem(std::shared_ptr<SpawnableWindow> InAssociatedWindow)
{
	DX12Data.AssociatedWindow = InAssociatedWindow;
}

void RenderSystem::AsyncInit()
{
	// Enable Debug Layer
	if (IsDebugEnabled())
	{
		Microsoft::WRL::ComPtr<ID3D12Debug> debugController;
		if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
			debugController->EnableDebugLayer();
	}
	if (!DX12Data.Init())
	{
		Logger::Log(Logger::ELogCategories::Critical, "RenderSystem::AsyncInit Structure 'FDX12Data' couldn't initialize. Aborting render system initialization;", RenderSystem::LogCategory);
		__debugbreak();
		return;
	}

	if (DX12Data.AssociatedWindow.get())
		DX12Data.AssociatedWindow->OnWindowResizeDelegate.Bind(shared_from_this(), &RenderSystem::OnWindowResizedEvent);
}

float timer = 0;
void RenderSystem::AsyncTick(float Delta)
{
}

#pragma region DX12

bool RenderSystem::IsDebugEnabled()
{
#if defined(_DEBUG)
    return true;
#endif

	return false;
}

RenderSystem::FDX12Data RenderSystem::DX12Data;

void RenderSystem::OnWindowResizedEvent(FVector2 NewResolution)
{
	DX12Data.ResizeSwapChain(NewResolution);
}

bool RenderSystem::FDX12Data::Init()
{
	if (!CreateDX12Device())
	{
		__debugbreak();
		Logger::Log(Logger::ELogCategories::Critical, "Could Not Create D3D12 Device RenderSystem::FDX12Data::Init()", LogCategory);
		return IsValid;
	}

	if (!SupportsDX12())
	{
		__debugbreak();
		Logger::Log(Logger::ELogCategories::Critical, "System Does Not Support DirectX 12", LogCategory);
		return IsValid;
	}

	if (!CreateCommandQueues())
	{
		__debugbreak();
		Logger::Log(Logger::ELogCategories::Critical, "Could Not Create Command Queue RenderSystem::FDX12Data::Init()", LogCategory);
		return IsValid;
	}

	if (!CreateFence())
	{
		__debugbreak();
		Logger::Log(Logger::ELogCategories::Critical, "Could not Create Fence RenderSystem::FDX12Data::Init()", LogCategory);

		return IsValid;
	}

	if (!(AssociatedWindow && CreateSwapchain(AssociatedWindow->GetWindow())))
	{
		__debugbreak();
		Logger::Log(Logger::ELogCategories::Critical, "Could not Create Swapchain RenderSystem::FDX12Data::Init()", LogCategory);
		return IsValid;
	}
	
	IsValid = true;
	return IsValid;
}

void RenderSystem::FDX12Data::ResizeSwapChain(FVector2 NewResolution)
{
	WaitForGPU(D3D12_COMMAND_LIST_TYPE_DIRECT);

	for (int i = 0; i < BufferCount; ++i)
	{
		BackBuffers[i].Reset();
	}

	HRESULT Res = SwapChain->ResizeBuffers(BufferCount, (int)NewResolution.GetX(), (int)NewResolution.GetY(), DXGI_FORMAT_R8G8B8A8_UNORM, 0);
	if (FAILED(Res))
	{
		__debugbreak();
		Logger::Log(Logger::ELogCategories::Error, "RenderSystem::FDX12Data::ResizeSwapChain Failed to resize swapchain buffers;", RenderSystem::LogCategory);
	}

	CreateRTVs();
}

void RenderSystem::FDX12Data::WaitForGPU(D3D12_COMMAND_LIST_TYPE CommandListType)
{
	if (!Fence || !CommandQueues[CommandListType])
		return;

	CommandQueues[CommandListType]->Signal(Fence.Get(), ++FenceValue);

	if (Fence->GetCompletedValue() < FenceValue)
	{
		HANDLE OnGPUFinished = CreateEvent(nullptr, FALSE, FALSE, nullptr);
		Fence->SetEventOnCompletion(FenceValue, OnGPUFinished);
		WaitForSingleObject(OnGPUFinished, INFINITE);
		CloseHandle(OnGPUFinished);
	}
}

void RenderSystem::FDX12Data::CreateRTVs()
{
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle(RTVHeap->GetCPUDescriptorHandleForHeapStart());
	for (UINT i = 0; i < BufferCount; i++)
	{
		SwapChain->GetBuffer(i, IID_PPV_ARGS(&BackBuffers[i]));
		Device->CreateRenderTargetView(BackBuffers[i].Get(), nullptr, rtvHandle);
		rtvHandle.ptr += RTVDescriptorSize;
	}
}

bool RenderSystem::FDX12Data::SupportsDX12()
{
	// If the Init function has not run this value might be incorrect
	return Device.Get() != nullptr;
}

bool RenderSystem::FDX12Data::CreateDX12Device()
{
	using Microsoft::WRL::ComPtr;

	// Factory
	ComPtr<IDXGIFactory7> Factory = nullptr;
	
#if defined(_DEBUG)
	HRESULT Res = CreateDXGIFactory2(DXGI_CREATE_FACTORY_DEBUG, IID_PPV_ARGS(&Factory));
#else
	HRESULT Res = CreateDXGIFactory2(0, IID_PPV_ARGS(&Factory));
#endif

	if (Res < 0)
	{
		Logger::Log(Logger::ELogCategories::Critical, "RenderSystem::FDX12Data::CreateDX12Device Could not create a factory", RenderSystem::LogCategory);
		__debugbreak();
		return false;
	}

	// Adapter
	ComPtr<IDXGIAdapter1> Adapter = nullptr;

	for (UINT adapterIndex = 0; Factory->EnumAdapters1(adapterIndex, &Adapter) != DXGI_ERROR_NOT_FOUND; ++adapterIndex)
	{
		DXGI_ADAPTER_DESC1 desc;
		Adapter->GetDesc1(&desc);

		if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE) // Skip Warp Adapters
			continue;
		
		// Create Device
		if (D3D12CreateDevice(Adapter.Get(), D3D_FEATURE_LEVEL_12_1, IID_PPV_ARGS(&Device)) >= 0)
		{
			DXGI_ADAPTER_DESC1 desc = {};
			Adapter->GetDesc1(&desc);
			break;
		}
	}

	return Device.Get() != nullptr;
}

bool RenderSystem::FDX12Data::CreateCommandQueues()
{
	using Microsoft::WRL::ComPtr;
	for (auto& T : CommandQueues)
	{
		D3D12_COMMAND_QUEUE_DESC QueueDesc = {};

		// This flag is unchanged across the program because if it does hang we
		// want windows to run recovery. If the system hangs there is somewhere else a heavy GPU performance issue.
		QueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
		QueueDesc.Type = T.first;

		//Create Command Queue
		HRESULT Res = Device->CreateCommandQueue(&QueueDesc, IID_PPV_ARGS(&T.second));

		if (Res < 0)
		{
			Logger::Log(Logger::ELogCategories::Critical, "RenderSystem::FDX12Data::CreateCommandQueues Failed to create a command queue.", RenderSystem::LogCategory);
			__debugbreak();
			return false;
		}
	}

	return true;
}

bool RenderSystem::FDX12Data::CreateFence()
{
	if (!Device)
	{
		__debugbreak();
		//Debug("Device is Invalid Cant generate fence.");
		return false;
	}

	Device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&Fence));
	FenceValue = 0;

	return true;
}

bool RenderSystem::FDX12Data::CreateSwapchain(const HWND* WindowHandle)
{
	if (!AssociatedWindow)
		return false;

	WaitForGPU(D3D12_COMMAND_LIST_TYPE_DIRECT);
	SwapChain.Reset();

	using Microsoft::WRL::ComPtr;
	ComPtr<IDXGIFactory7> Factory = nullptr;

#if defined(_DEBUG)
	HRESULT Res = CreateDXGIFactory2(DXGI_CREATE_FACTORY_DEBUG, IID_PPV_ARGS(&Factory));
#else
	HRESULT Res = CreateDXGIFactory2(0, IID_PPV_ARGS(&Factory));
#endif

	DXGI_SWAP_CHAIN_DESC1 SwapchainDesc = {};
	SwapchainDesc.BufferCount = BufferCount;
	SwapchainDesc.Width = (UINT)AssociatedWindow->GetResolution().GetX();
	SwapchainDesc.Height = (UINT)AssociatedWindow->GetResolution().GetY();
	SwapchainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // 8 bit depth
	SwapchainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	SwapchainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	SwapchainDesc.SampleDesc.Count = 1;

	Microsoft::WRL::ComPtr<IDXGISwapChain1> SwapChainCache = nullptr;

	Factory->CreateSwapChainForHwnd(
		CommandQueues[D3D12_COMMAND_LIST_TYPE_DIRECT].Get(),
		*WindowHandle,
		&SwapchainDesc,
		nullptr,
		nullptr,
		&SwapChainCache
		);
	if (!SwapChainCache)
		return false;

	if(FAILED(SwapChainCache.As(&SwapChain)))
		return false;

	Factory->MakeWindowAssociation(*WindowHandle, 0);

	FrameIndex = SwapChain->GetCurrentBackBufferIndex();

	// Create RTV Descriptor Heap
	D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
	rtvHeapDesc.NumDescriptors = BufferCount;
	rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	Device->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&RTVHeap));

	RTVDescriptorSize = Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

	CreateRTVs();

	return true;
}

#pragma endregion

#pragma region Rendering

RenderComponent* RenderSystem::CreateRenderComponent(Object* Owner)
{
	if (!Owner)
		return nullptr;

	RenderComponent* Comp = &RenderComponentsCache.emplace_back(Owner->GetGID());
	
	return Comp;
}

#pragma endregion

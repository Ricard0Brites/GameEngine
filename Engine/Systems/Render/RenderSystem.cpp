#include "RenderSystem.h"
#include <iostream>
#include <utility>
#include "Windows/WindowBase.h"


RenderSystem::RenderSystem(WindowBase* InAssociatedWindow)
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
		//TODO - Error Message here (Add Log System)
		__debugbreak();
		return;
	}
}

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



}

bool RenderSystem::FDX12Data::Init()
{

	if (!CreateDX12Device())
	{
		__debugbreak();
		//Debug("Could Not Create D3D12 Device RenderSystem::FDX12Data::Init()");
		return IsValid;
	}

	if (!SupportsDX12())
	{
		__debugbreak();
		//Debug("System Does Not Support DirectX 12");
		return IsValid;
	}

	if (!CreateCommandQueues())
	{
		__debugbreak();
		//Debug("Could Not Create Command Queue RenderSystem::FDX12Data::Init()");
		return IsValid;
	}

	if (!CreateFence())
	{
		__debugbreak();
		//Debug("Could not Create Fence RenderSystem::FDX12Data::Init()");
		return IsValid;
	}

	if (!(DX12Data.AssociatedWindow && CreateSwapchain(DX12Data.AssociatedWindow->GetWindow())))
	{
		__debugbreak();
		//Debug("Could not Create Swapchain RenderSystem::FDX12Data::Init()");
		return IsValid;
	}
	
	IsValid = true;
	return IsValid;
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
		//TODO - Log Error Message here (Add Log System)
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
	for (std::pair<D3D12_COMMAND_LIST_TYPE, ComPtr<ID3D12CommandQueue>> T : CommandQueues)
	{
		D3D12_COMMAND_QUEUE_DESC QueueDesc = {};

		// This flag is unchanged across the program because if it does hang we
		// want windows to run recovery. If the system hangs there is somewhere else a heavy GPU performance issue.
		QueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
		QueueDesc.Type = T.first;

		//Create Command Queue
		ComPtr<ID3D12CommandQueue> Cache;
		HRESULT Res = Device->CreateCommandQueue(&QueueDesc, IID_PPV_ARGS(&Cache));

		CommandQueues[T.first] = Cache;

		if (Res < 0)
		{
			//TODO - Log Error Message here (Add Log System)
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

	return true;
}

bool RenderSystem::FDX12Data::CreateSwapchain(const HWND* WindowHandle)
{
	if (!AssociatedWindow)
		return false;

	using Microsoft::WRL::ComPtr;
	ComPtr<IDXGIFactory7> Factory = nullptr;

#if defined(_DEBUG)
	HRESULT Res = CreateDXGIFactory2(DXGI_CREATE_FACTORY_DEBUG, IID_PPV_ARGS(&Factory));
#else
	HRESULT Res = CreateDXGIFactory2(0, IID_PPV_ARGS(&Factory));
#endif

	DXGI_SWAP_CHAIN_DESC1 SwapchainDesc = {};
	SwapchainDesc.BufferCount = 3;
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

	SwapChain = (IDXGISwapChain4*)SwapChainCache.Get();

	if (!SwapChain)
		return false;

	Factory->MakeWindowAssociation(*WindowHandle, 0);

	FrameIndex = SwapChain->GetCurrentBackBufferIndex();

	return true;
}

#pragma endregion
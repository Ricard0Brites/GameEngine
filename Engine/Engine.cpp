#include "Engine.h"
#include "Windows/WindowBase.h"
#include <string>
#include <cstdio>


using namespace Microsoft::WRL;


Engine::Engine(const WCHAR* InWindowTitle)
    : WindowBase(InWindowTitle)
{
    OnDestroy.Bind(this, &Engine::Quit);
}

Engine::~Engine()
{
}

void Engine::InitializeD3D12()
{
    CreateD3DDevice();
    CreateCommandQueue();
    CreateSwapChain();
    CreateRendertargets();
    CreateCommandAllocator();
}

#pragma region DirectX 12

void Engine::CreateD3DDevice()
{
    // Make a Factory
    CreateDXGIFactory2(0, __uuidof(Factory), &Factory);

    // Represents the GPUs information (Dedicated memory, Shared memory, etc)
    ComPtr<IDXGIAdapter4> AdapterInfo;

    // Fetch Data for GPU with index 0 ( the most powerful one according to the "DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE" flag
    Factory->EnumAdapterByGpuPreference(0,DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE,IID_PPV_ARGS(&AdapterInfo));

    // Create the device
    D3D12CreateDevice(AdapterInfo.Get(), D3D_FEATURE_LEVEL_12_0, IID_PPV_ARGS(&DeviceRef));
}

void Engine::CreateCommandQueue()
{
    // Create Queue Description
    D3D12_COMMAND_QUEUE_DESC QueueDesc = {};

    //Define type of queue
    QueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
    QueueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;

    // Create Queue and assign it to a class variable
    DeviceRef->CreateCommandQueue(&QueueDesc, IID_PPV_ARGS(&CommandQ)); 
}

void Engine::CreateSwapChain()
{
    // Create Swap Chain
    DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
    swapChainDesc.BufferCount = 2;
    swapChainDesc.Width = ResX; // Window Resolution X
    swapChainDesc.Height = ResY; // Window Resolution Y
    swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    swapChainDesc.SampleDesc.Count = 1;

    const HWND& WindowRef = *GetWindowHandle();

    ComPtr<IDXGISwapChain1> TempSwapChain;

    Factory->CreateSwapChainForHwnd(
        CommandQ.Get(),
        WindowRef,
        &swapChainDesc,
        nullptr,
        nullptr,
        &TempSwapChain
    );
    Factory->MakeWindowAssociation(WindowRef, DXGI_MWA_NO_ALT_ENTER);
    SwapChain = (IDXGISwapChain4*)TempSwapChain.Get();
    CurrentFrameIndex = SwapChain->GetCurrentBackBufferIndex();
}

void Engine::CreateRendertargets()
{
    // Creates Description for 2 Render Target View Heaps (RTV)

    D3D12_DESCRIPTOR_HEAP_DESC RTVDescHeapDesc = {};
    RTVDescHeapDesc.NumDescriptors = 2; // create 2 heap descriptions
    RTVDescHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV; // Define as RTV
    RTVDescHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE; // Not GPU accessible
    
    // Create Heap
    DeviceRef->CreateDescriptorHeap(&RTVDescHeapDesc, IID_PPV_ARGS(&RTVHeapDesc));

    // Save Descriptor Size ( To Loop through multiple RTV Handles & swap chain indexing )
    RTVDescriptorSize = DeviceRef->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV); 


    // Create Render Target Views
    D3D12_CPU_DESCRIPTOR_HANDLE RTVHandle = RTVHeapDesc->GetCPUDescriptorHandleForHeapStart(); // Get First Descriptor Handle
    
    for (UINT n = 0; n < RTVHeapDesc->GetDesc().NumDescriptors; ++n)
    {
        SwapChain->GetBuffer(n, IID_PPV_ARGS(&RenderTargets[n])); // Get Handle to the buffer id **n** from the swap chain
        DeviceRef->CreateRenderTargetView(RenderTargets[n].Get(), nullptr, RTVHandle); // Create RTV in the handle
        RTVHandle.ptr += (1 * RTVDescriptorSize); // Move to the next Descriptor
    }
}


#pragma endregion 

void Engine::GetAdapterInformation(const Microsoft::WRL::ComPtr<IDXGIAdapter4>& Adapter, DXGI_ADAPTER_DESC3 &Desc)
{
    Adapter->GetDesc3(&Desc);
}

void Engine::Launch()
{
    InitializeD3D12();

    IsRunning = true;
    while (IsRunning)
    {
        PumpMessages();
    }

}

void Engine::Quit()
{
    IsRunning = false;
}
#include "Engine.h"
#include "Windows/WindowBase.h"
#include <string>
#include <cstdio>
#include <d3d12sdklayers.h>

using namespace Microsoft::WRL;

struct Vertex
{

    float position[3];
    float color[4];
};

Engine::Engine(const WCHAR* InWindowTitle)
    : WindowBase(InWindowTitle)
{
    OnDestroy.Bind(this, &Engine::Quit);
}

Engine::~Engine()
{
}

void Engine::OnMessageReceived(MSG InMessage)
{
    //std::cout << InMessage.message << std::endl;
    if (InMessage.message == WM_SIZE)
    {        
        RECT WR;
        if (GetWindowRect(InMessage.hwnd, &WR))
        {
            ResX = WR.right;
            ResY = WR.bottom;
        }
    }
}

void Engine::InitializeD3D12()
{
    Viewport.Width = static_cast<float>(ResX);
    Viewport.Height = static_cast<float>(ResY);
    Viewport.TopLeftX = 0.0f;
    Viewport.TopLeftY = 0.0f;
    Viewport.MaxDepth = 1.0f;
    ScissorRect.left = 0;
    ScissorRect.top = 0;
    ScissorRect.right = static_cast<LONG>(ResX);
    ScissorRect.bottom = static_cast<LONG>(ResY);

    CreateD3DDevice();

    CreateCommandQueue();
    CreateCommandAllocator();
    CreateCommandsList();

    CreateVertexShader();
    CreateVertexBuffer();
    
    CreatePixelShader();

    CreateSwapChain();
    CreateRendertargets();
    CreateRootSignature();
    CreatePSO();
    CreateFence();
}

#pragma region DirectX 12

void Engine::CreateD3DDevice()
{
    // Make a Factory
    CreateDXGIFactory1(IID_PPV_ARGS(&Factory));

    // Represents the GPUs information (Dedicated memory, Shared memory, etc)
    ComPtr<IDXGIAdapter4> AdapterInfo;

    // Fetch Data for GPU with index 0 ( the most powerful one according to the "DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE" flag
    Factory->EnumAdapterByGpuPreference(0,DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE,IID_PPV_ARGS(&AdapterInfo));

    #ifdef _DEBUG
        ComPtr<ID3D12Debug> DebugController;
        if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&DebugController))))
        {
            DebugController->EnableDebugLayer();
        }
    #endif // _DEBUG


    // Create the device
    D3D12CreateDevice(AdapterInfo.Get(), D3D_FEATURE_LEVEL_12_1, IID_PPV_ARGS(&DeviceRef));
}

// Commands
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

void Engine::CreateCommandAllocator()
{
    DeviceRef->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&CommandAllocator));
}

void Engine::CreateCommandsList()
{
    DeviceRef->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, CommandAllocator.Get(), PSO.Get(), IID_PPV_ARGS(&CommandList));
    CommandList->Close();
}

//Shaders
void Engine::CreateVertexShader()
{
    ComPtr<ID3D10Blob> error;
    HRESULT hr = D3DCompile(Shader.c_str(), Shader.length(), NULL, NULL, NULL, "VSMain", "vs_5_0", ShaderCompilationFlags, 0, &VertexShader, &error);
    if (FAILED(hr))
    {
        if (error)
        {
            OutputDebugStringA(static_cast<char*>(error->GetBufferPointer()));
        }
        __debugbreak();
    }
}

void Engine::CreateVertexBuffer()
{
    float AR = ((float)ResX / (float)ResY);
    Vertex triangleVertices[] =
    {
        { { 0.0f, 0.25f * AR, 0.5f}, {1.0f, 0.0f, 0.0f, 1.0f}},
        { { 0.25f, -0.25f * AR, 0.5f }, { 0.0f, 1.0f, 0.0f, 1.0f } },
        { { -0.25f, -0.25f * AR, 0.5f }, { 0.0f, 0.0f, 1.0f, 1.0f } }
    };

    const UINT vertexBufferSize = sizeof(triangleVertices);

    D3D12_HEAP_PROPERTIES heapProps;
    heapProps.Type = D3D12_HEAP_TYPE_UPLOAD;
    heapProps.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
    heapProps.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
    heapProps.CreationNodeMask = 1;
    heapProps.VisibleNodeMask = 1;

    D3D12_RESOURCE_DESC vertexBufferResourceDesc;
    vertexBufferResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
    vertexBufferResourceDesc.Alignment = 0;
    vertexBufferResourceDesc.Width = vertexBufferSize;
    vertexBufferResourceDesc.Height = 1;
    vertexBufferResourceDesc.DepthOrArraySize = 1;
    vertexBufferResourceDesc.MipLevels = 1;
    vertexBufferResourceDesc.Format = DXGI_FORMAT_UNKNOWN;
    vertexBufferResourceDesc.SampleDesc.Count = 1;
    vertexBufferResourceDesc.SampleDesc.Quality = 0;
    vertexBufferResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
    vertexBufferResourceDesc.Flags = D3D12_RESOURCE_FLAG_NONE;

    DeviceRef->CreateCommittedResource(
        &heapProps,
        D3D12_HEAP_FLAG_NONE,
        &vertexBufferResourceDesc,
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(&VertexBuffer));

    UINT8* pVertexDataBegin;
    D3D12_RANGE readRange = { 0, 0 };
    VertexBuffer->Map(0, &readRange, reinterpret_cast<void**>(&pVertexDataBegin));
    memcpy(pVertexDataBegin, triangleVertices, sizeof(triangleVertices));
    VertexBuffer->Unmap(0, nullptr);

    VertexBufferView.BufferLocation = VertexBuffer->GetGPUVirtualAddress();
    VertexBufferView.StrideInBytes = sizeof(Vertex);
    VertexBufferView.SizeInBytes = vertexBufferSize;
    
}

void Engine::CreatePixelShader()
{
    ComPtr<ID3D10Blob> error;
    HRESULT hr = D3DCompile(Shader.c_str(), Shader.length(), NULL, NULL, NULL, "PSMain", "ps_5_0", ShaderCompilationFlags, 0, &PixelShader, &error);
    if (FAILED(hr))
    {
        if (error)
        {
            OutputDebugStringA(static_cast<char*>(error->GetBufferPointer()));
        }
        __debugbreak();
    }
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
    Factory->MakeWindowAssociation(WindowRef, 0);
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

void Engine::CreateRootSignature()
{
    D3D12_ROOT_SIGNATURE_DESC RootSignatureDesc;
    RootSignatureDesc.NumParameters = 0;
    RootSignatureDesc.pParameters = nullptr;
    RootSignatureDesc.NumStaticSamplers = 0;
    RootSignatureDesc.pStaticSamplers = nullptr;
    RootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

    ComPtr<ID3DBlob> signature;
    ComPtr<ID3DBlob> error;
    D3D12SerializeRootSignature(&RootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &signature, &error);
    DeviceRef->CreateRootSignature(0, signature->GetBufferPointer(), signature->GetBufferSize(), IID_PPV_ARGS(&RootSignature));
}

void Engine::CreatePSO()
{
    // in an actual project this would be material dependent but we are hard coding it here
    D3D12_INPUT_ELEMENT_DESC inputElementDescs[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
        { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
    };

    D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
    psoDesc.InputLayout = { inputElementDescs, _countof(inputElementDescs) };
    psoDesc.pRootSignature = RootSignature.Get(); 
    psoDesc.VS.pShaderBytecode = VertexShader->GetBufferPointer(); 
    psoDesc.VS.BytecodeLength = VertexShader->GetBufferSize();
    psoDesc.PS.pShaderBytecode = PixelShader->GetBufferPointer();
    psoDesc.PS.BytecodeLength = PixelShader->GetBufferSize(); 
    psoDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID; 
    psoDesc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE; 
    psoDesc.BlendState.AlphaToCoverageEnable = FALSE; 
    psoDesc.BlendState.IndependentBlendEnable = FALSE;
    psoDesc.BlendState.RenderTarget[0].BlendEnable = FALSE;
    psoDesc.BlendState.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL; 
    psoDesc.DepthStencilState.DepthEnable = FALSE; 
    psoDesc.DepthStencilState.StencilEnable = FALSE; 
    psoDesc.SampleMask = UINT_MAX; // opacity 1?
    psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE; 
    psoDesc.NumRenderTargets = 1; 
    psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM; 
    psoDesc.SampleDesc.Count = 1;
    DeviceRef->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&PSO)); // Create PSO
}

void Engine::CreateFence()
{
    DeviceRef->CreateFence(0, D3D12_FENCE_FLAG_NONE, __uuidof(Fence), &Fence);
    FenceValue = 1;
    FenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
}

void Engine::OnRender()
{
    // Record all the commands we need to render the scene into the command list.
    CommandAllocator->Reset();
    CommandList->Reset(CommandAllocator.Get(), PSO.Get());

    CommandList->SetGraphicsRootSignature(RootSignature.Get());
    CommandList->RSSetViewports(1, &Viewport);
    CommandList->RSSetScissorRects(1, &ScissorRect);

    D3D12_RESOURCE_BARRIER renderTargetBarrier;
    renderTargetBarrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
    renderTargetBarrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
    renderTargetBarrier.Transition.pResource = RenderTargets[CurrentFrameIndex].Get();
    renderTargetBarrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
    renderTargetBarrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
    renderTargetBarrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
    CommandList->ResourceBarrier(1, &renderTargetBarrier);

    D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = RTVHeapDesc->GetCPUDescriptorHandleForHeapStart();
    rtvHandle.ptr += (CurrentFrameIndex * RTVDescriptorSize);

    CommandList->OMSetRenderTargets(1, &rtvHandle, FALSE, nullptr);

    const float clearColor[] = { 0.15f, 0.15f, 0.15f, 1.0f };
    CommandList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);
    CommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    CommandList->IASetVertexBuffers(0, 1, &VertexBufferView);
    CommandList->DrawInstanced(3, 1, 0, 0);


    D3D12_RESOURCE_BARRIER presentBarrier;
    presentBarrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
    presentBarrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
    presentBarrier.Transition.pResource = RenderTargets[CurrentFrameIndex].Get();
    presentBarrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
    presentBarrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
    presentBarrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
    CommandList->ResourceBarrier(1, &presentBarrier);

    CommandList->Close();

    // Execute the command list.
    ID3D12CommandList* ppCommandLists[] = { CommandList.Get() };
    CommandQ->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);

    // Present the frame.
    SwapChain->Present(1, 0);

    AwaitFrameRender();
}

void Engine::AwaitFrameRender()
{
    // WAITING FOR THE FRAME TO COMPLETE BEFORE CONTINUING IS NOT BEST PRACTICE.
    // This is code implemented as such for simplicity. The D3D12HelloFrameBuffering
    // sample illustrates how to use fences for efficient resource usage and to maximize GPU
    // utilization.

    // Signal and increment the fence value.
    const UINT64 FenceValueCache = FenceValue;
    CommandQ->Signal(Fence.Get(), FenceValueCache);
    FenceValue++;

    // Wait until the previous frame is finished.
    if (Fence->GetCompletedValue() < FenceValueCache)
    {
        Fence->SetEventOnCompletion(FenceValueCache, FenceEvent);
        WaitForSingleObject(FenceEvent, INFINITE);
    }

    CurrentFrameIndex = SwapChain->GetCurrentBackBufferIndex();
}

#pragma endregion 

void Engine::GetAdapterInformation(const Microsoft::WRL::ComPtr<IDXGIAdapter4>& Adapter, DXGI_ADAPTER_DESC3 &Desc)
{
    Adapter->GetDesc3(&Desc);
}

void Engine::OnWindowResize(int X, int Y)
{
    ResX = X;
    ResY = Y;

    using namespace std;
    cout << "Resizing X=" << X << " Y=" << Y << endl;
}

void Engine::Launch()
{
    InitializeD3D12();

    IsRunning = true;
    while (IsRunning)
    {
        PumpMessages();
        OnRender();
    }

}

void Engine::Quit()
{
    IsRunning = false;
}

void Engine::TestFunc()
{
    std::cout << "TEST FUNC" << std::endl;
}


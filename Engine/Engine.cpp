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

void Engine::CreateCommandAllocator()
{
    DeviceRef->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&CommandAllocator));
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

void Engine::CreateVertexShader()
{
    D3DCompile(Shader.c_str(), Shader.length(), NULL, NULL, NULL, "VSMain", "vs_5_0", ShaderCompilationFlags, 0, &VertexShader, NULL);
}

void Engine::CreatePixelShader()
{
    D3DCompile(Shader.c_str(), Shader.length(), NULL, NULL, NULL, "PSMain", "ps_5_0", ShaderCompilationFlags, 0, &PixelShader, NULL);
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
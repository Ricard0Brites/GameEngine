#pragma once
#include "Core/Core.h"
#include "Windows/WindowBase.h"
#include <windows.h>
#include <d3d12.h>
#include <d3dcompiler.h>
#include <dxgi1_6.h>
#include <wrl.h>

class ENGINE_API Engine : private WindowBase
{
public:
    Engine(const WCHAR* InWindowTitle);
    void Launch();
    void Quit();

private:
    ~Engine();
    bool IsRunning = false;

    
    #pragma region DirectX 12

    void InitializeD3D12();
    void CreateD3DDevice();
    void CreateCommandQueue();
    void CreateSwapChain();
    void CreateRendertargets();
    void CreateCommandAllocator();
    void CreateRootSignature();
    void CreateVertexShader();
    void CreatePixelShader();
    void CreatePSO();
    void CreateCommandsList(); // WIP
    void CreateVertexBuffer(); // WIP
    void CreateFence(); // WIP

    void GetAdapterInformation(const Microsoft::WRL::ComPtr<IDXGIAdapter4>& Adapter, DXGI_ADAPTER_DESC3 &Desc);

    // Interface Factory
    Microsoft::WRL::ComPtr<IDXGIFactory7> Factory;

    // GPU
    Microsoft::WRL::ComPtr<ID3D12Device> DeviceRef; 

    // Command Queue
    Microsoft::WRL::ComPtr<ID3D12CommandQueue> CommandQ; 
    
    // Command Allocator
    Microsoft::WRL::ComPtr<ID3D12CommandAllocator> CommandAllocator;

    // Swap Chain
    Microsoft::WRL::ComPtr<IDXGISwapChain4> SwapChain;

    //Render Target View
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> RTVHeapDesc;
    Microsoft::WRL::ComPtr<ID3D12Resource> RenderTargets[2];
    UINT RTVDescriptorSize;

    // Root Signature
    Microsoft::WRL::ComPtr<ID3D12RootSignature> RootSignature;
    
    // Shaders
    Microsoft::WRL::ComPtr<ID3DBlob> VertexShader, PixelShader;
    std::string Shader = R"(
        struct PSInput 
        {
            float4 position : SV_POSITION;
            float4 color : COLOR;
        };
        PSInput VSMain(float4 position : POSITION, float4 color : COLOR) 
        {
            PSInput result;
            result.position = position;
            result.color = color;
            return result;
        }
        float4 PSMain(PSInput input) : SV_TARGET 
        {
            return input.color;
        }
    )";

    #if defined(_DEBUG)
        UINT ShaderCompilationFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
    #else
        UINT ShaderCompilationFlags = 0;
    #endif

    // Pipeline
    Microsoft::WRL::ComPtr<ID3D12PipelineState> PSO;
    #pragma endregion

    uint8_t ResX = 1280;
    uint8_t ResY = 720;

    UINT CurrentFrameIndex = 0;
};

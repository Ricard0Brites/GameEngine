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

    void TestFunc();
private:
    ~Engine();
    bool IsRunning = false;

    void OnMessageReceived(MSG InMessage) override;
    
    #pragma region DirectX 12

    void InitializeD3D12();
    void CreateD3DDevice();

    //Pipeline State Object (Where everything links)
    void CreatePSO();

    //Commands
    void CreateCommandQueue();
    void CreateCommandAllocator();
    void CreateCommandsList();

    //Shaders
    void CreateVertexShader();
    void CreatePixelShader();

    void CreateSwapChain();
    void CreateRendertargets();
    void CreateRootSignature();
    void CreateVertexBuffer();
    void CreateFence();

    void OnRender();
    void AwaitFrameRender();
    void GetAdapterInformation(const Microsoft::WRL::ComPtr<IDXGIAdapter4>& Adapter, DXGI_ADAPTER_DESC3& Desc);

    // Interface Factory
    Microsoft::WRL::ComPtr<IDXGIFactory7> Factory;

    // GPU
    Microsoft::WRL::ComPtr<ID3D12Device> DeviceRef; 

    // Command Queue
    Microsoft::WRL::ComPtr<ID3D12CommandQueue> CommandQ; 
    
    // Command Allocator
    Microsoft::WRL::ComPtr<ID3D12CommandAllocator> CommandAllocator;

    // Command List
    Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> CommandList;

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

    UINT ShaderCompilationFlags = 0;

    // Pipeline
    Microsoft::WRL::ComPtr<ID3D12PipelineState> PSO;
    
    // Vertex Buffer
    Microsoft::WRL::ComPtr<ID3D12Resource> VertexBuffer;
    D3D12_VERTEX_BUFFER_VIEW VertexBufferView;

    // Fence
    Microsoft::WRL::ComPtr<ID3D12Fence> Fence;
    UINT FenceValue;
    HANDLE FenceEvent;

    D3D12_VIEWPORT Viewport;
    D3D12_RECT ScissorRect;

    #pragma endregion

    UINT ResX = 1280;
    UINT ResY = 720;

    UINT CurrentFrameIndex = 0;
protected:
    void OnWindowResize(int X, int Y) override;

};
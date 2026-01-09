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

    void InitializeD3D12();
    void CreateD3DDevice();
    void CreateCommandQueue();
    void CreateSwapChain();
    void CreateDescriptorHeap(); // WIP
    void CreateRendertarget(); // WIP
    void CreateCommandAllocator(); // WIP
    void CreateRootSignature(); // WIP
    void CreateVertexShader(); // WIP
    void CreatePixelShader(); // WIP
    void CreatePSO(); // WIP
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

    // Swap Chain Interface
    Microsoft::WRL::ComPtr<IDXGISwapChain4> SwapChain;

    uint8_t ResX = 1280;
    uint8_t ResY = 720;

    UINT CurrentFrameIndex = 0;
};

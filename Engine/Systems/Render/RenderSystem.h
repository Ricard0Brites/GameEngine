#pragma once
#include "Core/ThreadedTask.hpp"

// DirectX 12
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN            // Exclude rarely-used stuff from Windows headers.
#endif

#include <windows.h>

#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl/client.h>
#include <unordered_map>



class ENGINE_API RenderSystem final : public ThreadedTask
{

public:
    RenderSystem(class WindowBase* InAssociatedWindow);
    void OnWindowResizedEvent(FVector2 NewResolution);


private:
	FORCEINLINE bool IsDebugEnabled();

	void AsyncTick(float Delta) override;
	void AsyncInit() override;

	struct FDX12Data
	{
		/* 
		This struct holds all the data for the main thread.
			- Device
			- Command Queue
			- Fence
		*/
		FDX12Data() = default;

		// Returns true if all resources have been successfully created
		bool Init();
		bool GetIsValid() { return IsValid; }

		Microsoft::WRL::ComPtr<ID3D12Device14> GetDevice() { return Device; }
		Microsoft::WRL::ComPtr<ID3D12CommandQueue>& GetCommandQueue(D3D12_COMMAND_LIST_TYPE Type) { return CommandQueues[Type]; }
		Microsoft::WRL::ComPtr<ID3D12Fence1> GetFence() { return Fence; }
		const Microsoft::WRL::ComPtr<IDXGISwapChain4> GetSwapChain() { return SwapChain; }

		WindowBase* AssociatedWindow = nullptr;

	private:		
		// DX12
		Microsoft::WRL::ComPtr<ID3D12Device14> Device = nullptr;
		Microsoft::WRL::ComPtr<ID3D12Fence1> Fence = nullptr;
		std::unordered_map<D3D12_COMMAND_LIST_TYPE, Microsoft::WRL::ComPtr<ID3D12CommandQueue>> CommandQueues = 
		{ 
			{D3D12_COMMAND_LIST_TYPE_DIRECT, nullptr},
			{D3D12_COMMAND_LIST_TYPE_COMPUTE, nullptr},
			{D3D12_COMMAND_LIST_TYPE_COPY, nullptr}
		};
		
		bool IsValid = false;

		#pragma region Helpers

		bool SupportsDX12();
		bool CreateDX12Device();
		bool CreateCommandQueues();
		bool CreateFence();
		bool CreateSwapchain(const HWND* WindowHandle);

		#pragma endregion

		Microsoft::WRL::ComPtr<IDXGISwapChain4> SwapChain = nullptr;
		uint32_t FrameIndex = 0;
	};
	static FDX12Data DX12Data;

};
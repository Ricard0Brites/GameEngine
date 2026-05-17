#pragma once
#include "Core/ThreadedTask.hpp"
#include "DX12TypeWrappers.h"

// DirectX 12
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN            // Exclude rarely-used stuff from Windows headers.
#endif

#include <windows.h>

#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl/client.h>
#include <unordered_map>
#include <string>
#include <memory>
#include <map>
#include <queue>
#include <stdint.h>

class RenderComponent;

class RenderSystem final : 
	public ThreadedTask,
	public std::enable_shared_from_this<RenderSystem>
{

public:
    RenderSystem(std::shared_ptr<class SpawnableWindow> InAssociatedWindow);
    void OnWindowResizedEvent(FVector2 NewResolution);

private:
	#pragma region Debug

	static const std::string LogCategory;
	FORCEINLINE bool IsDebugEnabled();

	#pragma endregion

	void AsyncTick(float Delta) override;
	void AsyncInit() override;

public:
	struct FDX12Data
	{
		/* 
		This struct holds all the data for the main thread.
			- Device
			- Command Queue
			- Fence
		*/
		FDX12Data() = default;
		~FDX12Data() = default;

		// Returns true if all resources have been successfully created
		bool Init();
		bool GetIsValid() { return IsValid; }
		void ResizeSwapChain(FVector2 NewResolution);

		Microsoft::WRL::ComPtr<ID3D12Device14> GetDevice() { return Device; }
		Microsoft::WRL::ComPtr<ID3D12CommandQueue>& GetCommandQueue(D3D12_COMMAND_LIST_TYPE Type) { return CommandQueues[Type]; }
		Microsoft::WRL::ComPtr<ID3D12Fence1> GetFence() { return Fence; }
		const Microsoft::WRL::ComPtr<IDXGISwapChain4> GetSwapChain() { return SwapChain; }

		std::shared_ptr<SpawnableWindow> AssociatedWindow = nullptr;

	private:
		void WaitForGPU(D3D12_COMMAND_LIST_TYPE CommandListType);
		void CreateRTVs();

		// DX12
		Microsoft::WRL::ComPtr<ID3D12Device14> Device = nullptr;
		Microsoft::WRL::ComPtr<ID3D12Fence1> Fence = nullptr;
		UINT64 FenceValue = 0;
		std::unordered_map<D3D12_COMMAND_LIST_TYPE, Microsoft::WRL::ComPtr<ID3D12CommandQueue>> CommandQueues =
		{
			{D3D12_COMMAND_LIST_TYPE_DIRECT, nullptr},
			{D3D12_COMMAND_LIST_TYPE_COMPUTE, nullptr},
			{D3D12_COMMAND_LIST_TYPE_COPY, nullptr}
		};

		bool IsValid = false;

		#pragma region Swapchain

		Microsoft::WRL::ComPtr<IDXGISwapChain4> SwapChain = nullptr;
		uint32_t FrameIndex = 0;
		static const UINT BufferCount = 3; // 0 - Being Shown 1 & 2 switch turns being rendered

		Microsoft::WRL::ComPtr<ID3D12Resource> BackBuffers[BufferCount];
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> RTVHeap = nullptr;
		UINT RTVDescriptorSize = 0;
		
		#pragma endregion

		#pragma region Helpers

		bool SupportsDX12();
		bool CreateDX12Device();
		bool CreateCommandQueues();
		bool CreateFence();
		bool CreateSwapchain(const HWND* WindowHandle);

		#pragma endregion

		#pragma region Pipeline State Object

	private:
		std::map<FPipelineStateObject, std::vector<Microsoft::WRL::ComPtr<ID3D12PipelineState>>> PSOCache;

		#pragma endregion

	};
	static FDX12Data DX12Data;
	/*			Global ID | Render Component Instance */
	static std::vector<RenderComponent> RenderComponentsCache;

	#pragma region Rendering
private:
	std::queue<uint64_t> RenderQueue = {};

	static uint64_t ComponentIDCounter;

public:
	static RenderComponent* CreateRenderComponent(class Object* Owner);
	#pragma endregion
};
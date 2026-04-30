#pragma once
#include <string>
#include <wrl/client.h>
#include "d3d12.h"


class RenderComponent
{
public:
	RenderComponent(std::string ShaderPath);
	~RenderComponent();
private:
	static const std::string LogCategory;
	std::string Shader = "";

	void CreateCommandList();

	struct FDX12Data
	{
		// Command Allocator
		Microsoft::WRL::ComPtr<ID3D12CommandAllocator> CommandAllocator;

		// Command List
		Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> CommandList;
	};
	FDX12Data DX12Data;
};
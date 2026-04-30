#pragma once
#include "Core/DataTypes.h"
#include <iostream>
#include <Vector>
#include "d3d12.h"

#pragma region Root Signature

	enum class EShaderVisibility : uint64_t
	{
		All = 0,
		Vertex = 1,
		Hull = 2,
		Domain = 3,
		Geometry = 4,
		Pixel = 5,
		Amplification = 6,
		Mesh = 7
	}; ENUM_CLASS_FLAGS(EShaderVisibility)

	#pragma region Root Parameter
		
		enum class EDescriptorRangeType : uint64_t
		{
			SRV = 0,
			UAV,
			CBV,
			SAMPLER
		}; ENUM_CLASS_FLAGS(EDescriptorRangeType)

		enum class ERootParameterType : uint64_t
		{
			Descriptor_Table = 0,
			Constants_32Bit,
			CBV,
			SRV,
			UAV
		}; ENUM_CLASS_FLAGS(ERootParameterType)

		struct FDescriptorRange
		{
			EDescriptorRangeType RangeType;
			uint64_t NumDescriptors;
			uint64_t BaseShaderRegister;
			uint64_t RegisterSpace;
			uint64_t OffsetInDescriptorsFromTableStart;
		};

		struct FDescriptorTable
		{
			uint64_t NumDescriptorRanges;
			const FDescriptorRange pDescriptorRanges;
		};

		struct FRootConstants
		{
			uint64_t ShaderRegister;
			uint64_t RegisterSpace;
			uint64_t Num32BitValues;
		};

		struct FRootDescriptor 
		{
			uint64_t ShaderRegister;
			uint64_t RegisterSpace;
		};

		struct FRootParameter
		{
			ERootParameterType ParameterType;
			union 
			{
				FDescriptorTable DescriptorTable;
				FRootConstants Constants;
				FRootDescriptor Descriptor;
			};
			EShaderVisibility ShaderVisibility;
		};

	#pragma endregion

	enum class EFilter : uint64_t
	{
		Min_Mag_Mip_Point = 0,
		Min_Mag_Point_Mip_Linear = 0x1,
		Min_Point_Mag_Linear_Mip_Point = 0x4,
		Min_Point_Mag_Mip_Linear = 0x5,
		Min_Linear_Mag_Mip_Point = 0x10,
		Min_Linear_Mag_Point_Mip_Linear = 0x11,
		Min_Mag_Linear_Mip_Point = 0x14,
		Min_Mag_Mip_Linear = 0x15,
		Min_Mag_Anisotropic_Mip_Point,
		Anisotropic = 0x55,
		Comparison_Min_Mag_Mip_Point = 0x80,
		Comparison_Min_Mag_Point_Mip_Linear = 0x81,
		Comparison_Min_Point_Mag_Linear_Mip_Point = 0x84,
		Comparison_Min_Point_Mag_Mip_Linear = 0x85,
		Comparison_Min_Linear_Mag_Mip_Point = 0x90,
		Comparison_Min_Linear_Mag_Point_Mip_Linear = 0x91,
		Comparison_Min_Mag_Linear_Mip_Point = 0x94,
		Comparison_Min_Mag_Mip_Linear = 0x95,
		Comparison_Min_Mag_Anisotropic_Mip_Point,
		Comparison_Anisotropic = 0xd5,
		Minimum_Min_Mag_Mip_Point = 0x100,
		Minimum_Min_Mag_Point_Mip_Linear = 0x101,
		Minimum_Min_Point_Mag_Linear_Mip_Point = 0x104,
		Minimum_Min_Point_Mag_Mip_Linear = 0x105,
		Minimum_Min_Linear_Mag_Mip_Point = 0x110,
		Minimum_Min_Linear_Mag_Point_Mip_Linear = 0x111,
		Minimum_Min_Mag_Linear_Mip_Point = 0x114,
		Minimum_Min_Mag_Mip_Linear = 0x115,
		Minimum_Min_Mag_Anisotropic_Mip_Point,
		Minimum_Anisotropic = 0x155,
		Maximum_Min_Mag_Mip_Point = 0x180,
		Maximum_Min_Mag_Point_Mip_Linear = 0x181,
		Maximum_Min_Point_Mag_Linear_Mip_Point = 0x184,
		Maximum_Min_Point_Mag_Mip_Linear = 0x185,
		Maximum_Min_Linear_Mag_Mip_Point = 0x190,
		Maximum_Min_Linear_Mag_Point_Mip_Linear = 0x191,
		Maximum_Min_Mag_Linear_Mip_Point = 0x194,
		Maximum_Min_Mag_Mip_Linear = 0x195,
		Maximum_Min_Mag_Anisotropic_Mip_Point,
		Maximum_Anisotropic = 0x1d5
	}; ENUM_CLASS_FLAGS(EFilter)
	
	enum class ETextureAddressMode : uint64_t
	{
		Wrap = 1,
		Mirror = 2,
		Clamp = 3,
		Border = 4,
		Mirror_Once = 5
	}; ENUM_CLASS_FLAGS(ETextureAddressMode)

	enum class EComparisonFunc : uint64_t 
	{
		None,
		Never = 1,
		Less = 2,
		Equal = 3,
		Less_Equal = 4,
		Greater = 5,
		Not_Equal = 6,
		Greater_Equal = 7,
		Always = 8
	}; ENUM_CLASS_FLAGS(EComparisonFunc)

	enum class EStaticBorderColor : uint64_t
	{
		Transparent_Black = 0,
		Opaque_Black,
		Opaque_White,
		Opaque_Balck_UINT,
		Opaque_White_UINT
	}; ENUM_CLASS_FLAGS(EStaticBorderColor)

	enum class ERootSignatureFlags : uint64_t 
	{
		None = 0,
		Allow_Input_Assembler_Input_Layout = 0x1,
		Dent_Vertex_Shader_Root_Access = 0x2,
		Deny_Hull_Shader_Root_Access = 0x4,
		Deny_Domain_Shader_Root_Access = 0x8,
		Deny_Geometry_Shader_Root_Access = 0x10,
		Deny_Pixel_Shader_Root_Access = 0x20,
		Allow_Stream_Output = 0x40,
		Local_Root_Signature = 0x80,
		Deny_Amplification_Shader_Root_Access = 0x100,
		Deny_Mesh_Shader_Root_Access = 0x200,
		CBV_SRV_UAV_HEAP_Directly_Indexed = 0x400,
		SAMPLER_HEAP_Directly_Indexed = 0x800

	}; ENUM_CLASS_FLAGS(ERootSignatureFlags)
	
	struct FStaticSampler
	{
		EFilter Filter;
		ETextureAddressMode AddressU;
		ETextureAddressMode AddressV;
		ETextureAddressMode AddressW;
		float MipLODBias;
		uint64_t MaxAnisotropy;
		EComparisonFunc ComparisonFunc;
		EStaticBorderColor  BorderColor;
		float MinLOD;
		float MaxLOD;
		uint64_t ShaderRegister;
		uint64_t RegisterSpace;
		EShaderVisibility ShaderVisibility;
	};

	struct FRootSignature
	{
		uint64_t NumParams = -1;
		const FRootParameter pParameters;
		uint64_t NumStaticSamplers;
		const FStaticSampler pStaticSamplers;
		ERootSignatureFlags Flags;
	};

#pragma endregion

#pragma region Shader

struct FShaderByteCode
{
	std::vector<uint8_t> data;

	D3D12_SHADER_BYTECODE GetRaw() const
	{
		return { data.data(), data.size() };
	}
};

#pragma endregion

#pragma region Stream Output

struct FStreamOutputDeclarationEntry 
{
	uint64_t Stream;
	LPCSTR SemanticName;
	uint64_t SemanticIndex;
	BYTE   StartComponent;
	BYTE   ComponentCount;
	BYTE   OutputSlot;
};

struct FStreamOutput
{
	const D3D12_SO_DECLARATION_ENTRY* pSODeclaration;
	UINT NumEntries;
	const UINT* pBufferStrides;
	UINT NumStrides;
	UINT RasterizedStream;
};

#pragma endregion

struct FPipelineStateObject
{
	FRootSignature RootSignature;
	FShaderByteCode VS, PS, DS, HS, GS;
	FStreamOutput StreamOutput;

	// TODO - Implement Wrappers for the types below
	D3D12_BLEND_DESC                   BlendState;
	UINT                               SampleMask;
	D3D12_RASTERIZER_DESC              RasterizerState;
	D3D12_DEPTH_STENCIL_DESC           DepthStencilState;
	D3D12_INPUT_LAYOUT_DESC            InputLayout;
	D3D12_INDEX_BUFFER_STRIP_CUT_VALUE IBStripCutValue;
	D3D12_PRIMITIVE_TOPOLOGY_TYPE      PrimitiveTopologyType;
	UINT                               NumRenderTargets;
	DXGI_FORMAT                        RTVFormats[8];
	DXGI_FORMAT                        DSVFormat;
	DXGI_SAMPLE_DESC                   SampleDesc;
	UINT                               NodeMask;
	D3D12_CACHED_PIPELINE_STATE        CachedPSO;
	D3D12_PIPELINE_STATE_FLAGS         Flags;
};
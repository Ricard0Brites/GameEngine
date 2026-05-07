#pragma once
#include "Core/DataTypes.h"
#include <iostream>
#include <Vector>
#include "d3d12.h"


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

struct FShaderByteCode
{
	std::vector<uint8_t> data;

	D3D12_SHADER_BYTECODE GetRaw() const
	{
		return { data.data(), data.size() };
	}
};

struct FStreamOutputDeclarationEntry 
{
	uint64_t Stream;
	std::string SemanticName;
	uint64_t SemanticIndex;
	BYTE   StartComponent;
	BYTE   ComponentCount;
	BYTE   OutputSlot;
};

struct FStreamOutput
{
	std::vector<FStreamOutputDeclarationEntry> SODeclaration;
	std::vector<uint64_t> BufferStrides;
	uint64_t RasterizedStream;
};

enum class EBlend : uint64_t
{
	Zero = 1,
	One = 2,
	Src_Color = 3,
	Inv_Src_Color = 4,
	Src_Alpha = 5,
	Inv_Src_Alpha = 6,
	Dest_Alpha = 7,
	Inv_Dest_Alpha = 8,
	Dest_Color = 9,
	Inv_Dest_Color = 10,
	Src_Alpha_Sat = 11,
	Blend_Factor = 14,
	Inv_Blend_Factor = 15,
	Src1_Color = 16,
	Inv_Src1_Color = 17,
	Src1_Alpha = 18,
	Inv_Src1_Alpha = 19
}; ENUM_CLASS_FLAGS(EBlend)

enum class EBlendOp : uint64_t
{
	Add = 1,
	Subtract = 2,
	Rev_Subtract = 3,
	Min = 4,
	Max = 5
}; ENUM_CLASS_FLAGS(EBlendOp)

enum class ELogicOp : uint64_t
{
	Clear = 0,
	Set = 1,
	Copy = 2,
	Copy_Inverted = 3,
	Noop = 4,
	Invert = 5,
	And = 6,
	Nand = 7,
	Or = 8,
	Nor = 9,
	Xor = 10,
	Equiv = 11,
	And_Reverse = 12,
	And_Inverted = 13,
	Or_Reverse = 14,
	Or_Inverted = 15
}; ENUM_CLASS_FLAGS(ELogicOp)

struct FRenderTargetBlendState
{
	bool BlendEnable;
	bool LogicOpEnable;
	EBlend SrcBlend;
	EBlend DestBlend;
	EBlendOp BlendOp;
	EBlend SrcBlendAlpha;
	EBlend DestBlendAlpha;
	EBlendOp BlendOpAlpha;
	ELogicOp LogicOp;
	uint8_t RenderTargetWriteMask;
};

struct FBlendState
{
	bool AlphaToCoverageEnable;
	bool IndependentBlendEnable;
	FRenderTargetBlendState RenderTarget[8];
};

enum class EFillMode : uint64_t
{
	Wireframe = 2,
	Solid = 3
}; ENUM_CLASS_FLAGS(EFillMode)

enum class ECullMode : uint64_t
{
	None = 1,
	Front = 2,
	Back = 3
}; ENUM_CLASS_FLAGS(ECullMode)

enum class EConservativeRasterizationMode : uint64_t
{
	Off = 0,
	On = 1
}; ENUM_CLASS_FLAGS(EConservativeRasterizationMode)

struct FRasterizerState
{
	EFillMode FillMode;
	ECullMode CullMode;
	bool FrontCounterClockwise;
	int DepthBias;
	float DepthBiasClamp;
	float SlopeScaledDepthBias;
	bool DepthClipEnable;
	bool MultisampleEnable;
	bool AntialiasedLineEnable;
	uint64_t ForcedSampleCount;
	EConservativeRasterizationMode ConservativeRaster;
};

enum class EDepthWriteMask : uint64_t
{
	Zero = 0,
	All = 1
}; ENUM_CLASS_FLAGS(EDepthWriteMask)

enum class EStencilOp : uint64_t
{
	Keep = 1,
	Zero = 2,
	Replace = 3,
	Incr_Sat = 4,
	Decr_Sat = 5,
	Invert = 6,
	Incr = 7,
	Decr = 8
}; ENUM_CLASS_FLAGS(EStencilOp)

struct FStencilOpState
{
	EStencilOp StencilFailOp;
	EStencilOp StencilDepthFailOp;
	EStencilOp StencilPassOp;
	EComparisonFunc StencilFunc;
};

struct FDepthStencilState
{
	bool DepthEnable;
	EDepthWriteMask DepthWriteMask;
	EComparisonFunc DepthFunc;
	bool StencilEnable;
	uint8_t StencilReadMask;
	uint8_t StencilWriteMask;
	FStencilOpState FrontFace;
	FStencilOpState BackFace;
};

enum class EInputClassification : uint64_t
{
	Per_Vertex_Data = 0,
	Per_Instance_Data = 1
}; ENUM_CLASS_FLAGS(EInputClassification)

struct FInputElementDesc
{
	std::string SemanticName;
	uint64_t SemanticIndex;
	DXGI_FORMAT Format;
	uint64_t InputSlot;
	uint64_t AlignedByteOffset;
	EInputClassification InputSlotClass;
	uint64_t InstanceDataStepRate;
};

struct FInputLayout
{
	std::vector<FInputElementDesc> InputElementDescs;
};

enum class EIndexBufferStripCutValue : uint64_t
{
	Disabled = 0,
	Value_0xFFFF = 1,
	Value_0xFFFFFFFF = 2
}; ENUM_CLASS_FLAGS(EIndexBufferStripCutValue)

enum class EPrimitiveTopologyType : uint64_t
{
	Undefined = 0,
	Point = 1,
	Line = 2,
	Triangle = 3,
	Patch = 4
}; ENUM_CLASS_FLAGS(EPrimitiveTopologyType)

struct FSampleDesc
{
	uint64_t Count;
	uint64_t Quality;
};

struct FCachedPSO
{
	std::vector<uint8_t> CachedBlob;
};

enum class EPipelineStateFlags : uint64_t
{
	None = 0,
	Tool_Debug = 1
}; ENUM_CLASS_FLAGS(EPipelineStateFlags)

struct FPipelineStateObject
{
	FRootSignature RootSignature;
	FShaderByteCode VS, PS, DS, HS, GS;
	FStreamOutput StreamOutput;
	FBlendState BlendState;
	uint64_t SampleMask;
	FRasterizerState RasterizerState;
	FDepthStencilState DepthStencilState;
	FInputLayout InputLayout;
	EIndexBufferStripCutValue IBStripCutValue;
	EPrimitiveTopologyType PrimitiveTopologyType;
	uint64_t NumRenderTargets;
	DXGI_FORMAT RTVFormats[8];
	DXGI_FORMAT DSVFormat;
	FSampleDesc SampleDesc;
	uint64_t NodeMask;
	FCachedPSO CachedPSO;
	EPipelineStateFlags Flags;
};
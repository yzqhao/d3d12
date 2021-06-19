
#pragma once


#include "../../common/d3dUtil.h"
#include "../../common/UploadBuffer.h"
#include "../../Graphic/GraphicsDefs.h"

struct D3D12GPUShaderStage final 
{
	D3D12GPUShaderStage(ShaderType t, std::string s, std::vector<std::string> d, std::string e, ID3DBlob* b = nullptr)
		: shaderType(t),
		source(std::move(std::move(s))),
		defines(std::move(d)),
		entrypoint(std::move(e)),
		byteCode(b) {}
	ShaderType shaderType = ShaderType::ST_NONE;
	std::string source;
	std::vector<std::string> defines;
	std::string entrypoint;
	ID3DBlob* byteCode = nullptr;
};
using D3D12GPUShaderStageList = std::vector<D3D12GPUShaderStage>;

class D3D12GPUShader final
{
public:
	D3D12GPUShaderStageList gpuStages;
};
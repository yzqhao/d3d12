#include "D3D12Api.h"

void d3d12ApiFuncCreateShader(D3D12GPUShader* gpuShader)
{
	for (D3D12GPUShaderStage& stage : gpuShader->gpuStages) 
	{
		stage.byteCode = d3dUtil::CompileShader(AnsiToWString(stage.source), nullptr, stage.entrypoint, "ps_5_0");
	}
}

void d3d12ApiFuncDestroyShader(D3D12GPUShader* gpuShader)
{
	for (D3D12GPUShaderStage& stage : gpuShader->gpuStages)
	{
		stage.byteCode->Release();
		stage.byteCode = nullptr;
	}
}

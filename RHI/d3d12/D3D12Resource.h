
#pragma once

#include "../../common/d3dUtil.h"
#include "../RHIBind.h"

#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p) if(p){p->Release(); p = NULL;}
#endif

class RHIPipelineStateID final : public RHIResourceIdentifier
{
public:
	virtual ~RHIPipelineStateID()
	{
		SAFE_RELEASE(m_pPipelineState)
	}
	ID3D12PipelineState* m_pPipelineState{};
};

class RHIShaderID final : public RHIResourceIdentifier
{
public:
	virtual ~RHIShaderID()
	{
		SAFE_RELEASE(m_pShader)
	}
	ID3DBlob* m_pShader{};
};



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

class RHIVBufferFormatID : public RHIResourceIdentifier
{
public:
	virtual ~RHIVBufferFormatID()
	{
	}
	std::vector<D3D12_INPUT_ELEMENT_DESC> m_pDeclaration;
};

class RHIVBufferID : public RHIResourceIdentifier
{
public:
	virtual ~RHIVBufferID()
	{
		SAFE_RELEASE(VertexBufferCPU);
		SAFE_RELEASE(VertexBufferGPU);
		SAFE_RELEASE(VertexBufferUploader);
	}

	ID3DBlob* VertexBufferCPU = nullptr;
	ID3D12Resource* VertexBufferGPU = nullptr;
	ID3D12Resource* VertexBufferUploader = nullptr;
};

class RHIIBufferID : public RHIResourceIdentifier
{
public:
	virtual ~RHIIBufferID()
	{
		SAFE_RELEASE(IndexBufferCPU);
		SAFE_RELEASE(IndexBufferGPU);
		SAFE_RELEASE(IndexBufferUploader);
	}
	
	ID3DBlob* IndexBufferCPU  = nullptr;
	ID3D12Resource* IndexBufferGPU = nullptr;
	ID3D12Resource* IndexBufferUploader = nullptr;
};

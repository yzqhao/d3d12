#include "RHID3D12.h"
#include "../RHIVSShader.h"
#include "../RHIPSShader.h"
#include "../RHIVertexBuffer.h"

/*
typedef struct D3D12_INPUT_ELEMENT_DESC
    {
    LPCSTR SemanticName;
    UINT SemanticIndex;
    DXGI_FORMAT Format;
    UINT InputSlot;
    UINT AlignedByteOffset;
    D3D12_INPUT_CLASSIFICATION InputSlotClass;
    UINT InstanceDataStepRate;
    } 	D3D12_INPUT_ELEMENT_DESC;
	*/

static const D3D12_INPUT_ELEMENT_DESC s_attrib[] =
{
	{ "POSITION",     0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
	{ "NORMAL",       0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
	{ "TANGENT",      0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
	{ "BITANGENT",    0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
	{ "COLOR",        0, DXGI_FORMAT_R8G8B8A8_UINT,   0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
	{ "COLOR",        1, DXGI_FORMAT_R8G8B8A8_UINT,   0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
	{ "COLOR",        2, DXGI_FORMAT_R8G8B8A8_UINT,   0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
	{ "COLOR",        3, DXGI_FORMAT_R8G8B8A8_UINT,   0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
	{ "BLENDINDICES", 0, DXGI_FORMAT_R8G8B8A8_UINT,   0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
	{ "BLENDWEIGHT",  0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",     0, DXGI_FORMAT_R32G32_FLOAT,    0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",     1, DXGI_FORMAT_R32G32_FLOAT,    0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",     2, DXGI_FORMAT_R32G32_FLOAT,    0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",     3, DXGI_FORMAT_R32G32_FLOAT,    0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",     4, DXGI_FORMAT_R32G32_FLOAT,    0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",     5, DXGI_FORMAT_R32G32_FLOAT,    0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",     6, DXGI_FORMAT_R32G32_FLOAT,    0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",     7, DXGI_FORMAT_R32G32_FLOAT,    0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
};

DXGI_FORMAT s_dwVertexFormatDateType[] = 
{
	DXGI_FORMAT_R32_FLOAT,
	DXGI_FORMAT_R32G32_FLOAT,
	DXGI_FORMAT_R32G32B32_FLOAT,
	DXGI_FORMAT_R32G32B32A32_FLOAT,

	DXGI_FORMAT_R16_UINT,
	DXGI_FORMAT_R32_UINT,

	DXGI_FORMAT_R8G8B8A8_UINT,
	DXGI_FORMAT_R8G8B8A8_UNORM,

	DXGI_FORMAT_R16G16_SINT,
	DXGI_FORMAT_R16G16_SNORM,
	DXGI_FORMAT_R16G16_UNORM,

	DXGI_FORMAT_R16G16B16A16_SINT,
	DXGI_FORMAT_R16G16B16A16_SNORM,
	DXGI_FORMAT_R16G16B16A16_UNORM,

	DXGI_FORMAT_R16G16_FLOAT,
	DXGI_FORMAT_R16G16B16A16_FLOAT,

	DXGI_FORMAT_R8G8B8A8_UNORM
};

bool RHID3D12::CreateInputLayout(const std::vector<RHIVertexFormat::VERTEXFORMAT_TYPE>& FormatArray, RHIVBufferFormatID * pVBufferFormatID)
{
	HRESULT hResult = NULL;
	std::vector<D3D12_INPUT_ELEMENT_DESC> Elements;
	D3D12_INPUT_ELEMENT_DESC Element;

	for (unsigned int i = 0; i < FormatArray.size(); i++)
	{
		const RHIVertexFormat::VERTEXFORMAT_TYPE& Format = FormatArray[i];
		Element = s_attrib[Format.Semantics];
		Element.AlignedByteOffset = Format.OffSet;
		Element.Format = s_dwVertexFormatDateType[Format.DateType];
		Element.SemanticIndex = Format.SemanticsIndex;
		Elements.push_back(Element);
	}
	
	return true;
}

void RHID3D12::OnLoadVertexBufferEx(uint uiOneVextexSize, uint uiTotalSize, RHIVertexBuffer* pVBuffer, RHIVBufferID * pVBufferID)
{
	unsigned char* pTempBufferUsed = new unsigned char[uiTotalSize];

	RHIDataBuffer* pDate = nullptr;

	unsigned int iVertexSize = 0;
	for (unsigned int i = 0; i < pVBuffer->GetPositionLevel(); i++)
	{
		pDate = pVBuffer->GetPositionDate(i);
		if (pDate)
		{
			for (unsigned int j = 0; j < pDate->GetNum(); j++)
			{
				memcpy(pTempBufferUsed + uiOneVextexSize * j + iVertexSize,
					(unsigned char *)pDate->GetDate() + j * pDate->GetStride(), pDate->GetStride());

			}
			iVertexSize += pDate->GetStride();
		}
	}

	for (unsigned int i = 0; i < pVBuffer->GetTexCoordLevel(); i++)
	{
		pDate = pVBuffer->GetTexCoordDate(i);
		if (pDate)
		{
			for (unsigned int j = 0; j < pDate->GetNum(); j++)
			{
				memcpy(pTempBufferUsed + uiOneVextexSize * j + iVertexSize,
					(unsigned char *)pDate->GetDate() + j * pDate->GetStride(), pDate->GetStride());

			}
			iVertexSize += pDate->GetStride();
		}
	}

	for (unsigned int i = 0; i < pVBuffer->GetNormalLevel(); i++)
	{
		pDate = pVBuffer->GetNormalDate(i);
		if (pDate)
		{
			for (unsigned int j = 0; j < pDate->GetNum(); j++)
			{
				memcpy(pTempBufferUsed + uiOneVextexSize * j + iVertexSize,
					(unsigned char *)pDate->GetDate() + j * pDate->GetStride(), pDate->GetStride());

			}
			iVertexSize += pDate->GetStride();
		}
	}
	
	pDate = pVBuffer->GetTangentDate();
	if (pDate)
	{
		for (unsigned int j = 0; j < pDate->GetNum(); j++)
		{
			memcpy(pTempBufferUsed + uiOneVextexSize * j + iVertexSize,
				(unsigned char *)pDate->GetDate() + j * pDate->GetStride(), pDate->GetStride());

		}
		iVertexSize += pDate->GetStride();
	}

	pDate = pVBuffer->GetBinormalDate();
	if (pDate)
	{
		for (unsigned int j = 0; j < pDate->GetNum(); j++)
		{
			memcpy(pTempBufferUsed + uiOneVextexSize * j + iVertexSize,
				(unsigned char *)pDate->GetDate() + j * pDate->GetStride(), pDate->GetStride());

		}
		iVertexSize += pDate->GetStride();
	}

	for (unsigned int i = 0; i < pVBuffer->GetColorLevel(); i++)
	{
		pDate = pVBuffer->GetColorDate(i);
		if (pDate)
		{
			for (unsigned int j = 0; j < pDate->GetNum(); j++)
			{
				memcpy(pTempBufferUsed + uiOneVextexSize * j + iVertexSize,
					(unsigned char *)pDate->GetDate() + j * pDate->GetStride(), pDate->GetStride());

			}
			iVertexSize += pDate->GetStride();
		}
	}

	pDate = pVBuffer->GetBlendWeightDate();
	if (pDate)
	{
		for (unsigned int j = 0; j < pDate->GetNum(); j++)
		{
			memcpy(pTempBufferUsed + uiOneVextexSize * j + iVertexSize,
				(unsigned char *)pDate->GetDate() + j * pDate->GetStride(), pDate->GetStride());

		}
		iVertexSize += pDate->GetStride();
	}

	pDate = pVBuffer->GetBlendIndicesDate();
	if (pDate)
	{
		for (unsigned int j = 0; j < pDate->GetNum(); j++)
		{
			memcpy(pTempBufferUsed + uiOneVextexSize * j + iVertexSize,
				(unsigned char *)pDate->GetDate() + j * pDate->GetStride(), pDate->GetStride());

		}
		iVertexSize += pDate->GetStride();
	}

	ThrowIfFailed(D3DCreateBlob(uiTotalSize, &pVBufferID->VertexBufferCPU));
	CopyMemory(pVBufferID->VertexBufferCPU->GetBufferPointer(), pTempBufferUsed, uiTotalSize);

	pVBufferID->VertexBufferGPU = d3dUtil::CreateDefaultBuffer(md3dDevice,
		mCommandList, pTempBufferUsed, uiTotalSize, pVBufferID->VertexBufferUploader);

	SAFE_DELETE_ARRAY(pTempBufferUsed);
}

void RHID3D12::OnLoadVertexBufferEx(uint uiTotalSize, RHIIndexBuffer* pVBuffer, RHIIBufferID * pVBufferID)
{
	ThrowIfFailed(D3DCreateBlob(uiTotalSize, &pVBufferID->IndexBufferCPU));
	CopyMemory(pVBufferID->IndexBufferCPU->GetBufferPointer(), pVBuffer->GetIndexDate(), uiTotalSize);

	pVBufferID->IndexBufferGPU = d3dUtil::CreateDefaultBuffer(md3dDevice,
		mCommandList, pVBuffer->GetIndexDate(), uiTotalSize, pVBufferID->IndexBufferUploader);
}

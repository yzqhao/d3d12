#include "RHID3D12.h"
#include "../RHIVSShader.h"
#include "../RHIPSShader.h"
#include "../RHITexture.h"

static const char* s_shadersVersion[] =
{
	"vs_5_0",
	"ps_5_0",
};

D3D_SHADER_MACRO* __GetDefine(const std::unordered_map<std::string, std::string>& Define)
{
	if (Define.empty())
	{
		return nullptr;
	}

	D3D_SHADER_MACRO* pMacro = new D3D_SHADER_MACRO[Define.size() + 1];
	int idx = 0;
	for (auto& it : Define)
	{
		pMacro[idx].Name = it.first.c_str();
		pMacro[idx].Definition = it.second.c_str();
		idx++;
	}
	pMacro[idx].Name = nullptr;
	pMacro[idx].Definition = nullptr;
	return pMacro;
}

static void __LoadShaderFromString(RHIShader* pShaderProgram, RHIShaderID* pID, ShaderType st)
{
	HRESULT hResult = NULL;
	ID3DBlob* pCode = NULL;
	ID3DBlob* pErrors = NULL;
	DWORD Flags = D3DCOMPILE_WARNINGS_ARE_ERRORS | D3DCOMPILE_ENABLE_STRICTNESS;
	Flags = D3DCOMPILE_OPTIMIZATION_LEVEL3;

	std::unordered_map<std::string, std::string> Define;
	pShaderProgram->GetShaderKey().GetDefine(Define);
	D3D_SHADER_MACRO* pMacro = __GetDefine(Define);
	if (pShaderProgram->GetBuffer().length())
	{
		hResult = D3DCompile((LPCSTR)pShaderProgram->GetBuffer().data(), pShaderProgram->GetBuffer().length(),
			NULL, pMacro, D3D_COMPILE_STANDARD_FILE_INCLUDE, pShaderProgram->GetMainFunName().data(),
			s_shadersVersion[(int)st], Flags, 0, &pCode, &pErrors);
	}
	/*else if (pShaderProgram->GetResourceName().GetLength())
	{
		std::string Path = VSResourceManager::ms_ShaderPath + RenderAPIPre + pShaderProgram->GetResourceName().GetString();
		hResult = D3DCompileFromFile(Path.c_str(), pMacro, D3D_COMPILE_STANDARD_FILE_INCLUDE, pShaderProgram->GetMainFunName().c_str(),
			s_shadersVersion[(int)ShaderType::ST_VS], Flags, 0, &pCode, &pErrors);
	}*/
	else
	{
		JY_ASSERT(0);
	}

	SAFE_DELETE_ARRAY(pMacro);
#ifdef _DEBUG
	if (pErrors && pErrors->GetBufferPointer())
	{
		JYERROR((const char*)pErrors->GetBufferPointer());
	}
#endif
	JY_ASSERT(!FAILED(hResult));
	JY_ASSERT(pCode);


	ID3D12ShaderReflection* pReflector = NULL;
	hResult = D3DReflect(pCode->GetBufferPointer(), pCode->GetBufferSize(), IID_ID3D11ShaderReflection, (void**)&pReflector);
	JY_ASSERT(!FAILED(hResult));
	D3D12_SHADER_DESC Desc;
	hResult = pReflector->GetDesc(&Desc);
	JY_ASSERT(!FAILED(hResult));
	const std::vector<RHIUserConstant*>& constants = pShaderProgram->GetConstant();
	if (!pShaderProgram->GetConstant().size())
	{
		for (unsigned int uiSamplerIndex = 0; uiSamplerIndex < Desc.BoundResources; uiSamplerIndex++)
		{
			D3D12_SHADER_INPUT_BIND_DESC resDesc;
			HRESULT hr = pReflector->GetResourceBindingDesc(uiSamplerIndex, &resDesc);
			JY_ASSERT(!FAILED(hResult));
			if (resDesc.Type == D3D_SIT_TEXTURE)
			{
				unsigned int uiType = 0;
				if (resDesc.Dimension == D3D10_SRV_DIMENSION_TEXTURE1D)
				{
					uiType = RHITexture::TT_1D;
				}
				else if (resDesc.Dimension == D3D10_SRV_DIMENSION_TEXTURE2D)
				{
					uiType = RHITexture::TT_2D;
				}
				else if (resDesc.Dimension == D3D10_SRV_DIMENSION_TEXTURE3D)
				{
					uiType = RHITexture::TT_3D;
				}
				else if (resDesc.Dimension == D3D10_SRV_DIMENSION_TEXTURECUBE)
				{
					uiType = RHITexture::TT_CUBE;
				}
				else
				{
					JY_ASSERT(0);
				}

			}
			else if (resDesc.Type == D3D_SIT_SAMPLER)
			{

			}
			else if (resDesc.Type == D3D_CT_CBUFFER)
			{
				ID3D12ShaderReflectionConstantBuffer* pRCB = pReflector->GetConstantBufferByName(resDesc.Name);
				D3D12_SHADER_BUFFER_DESC cbDesc;
				pRCB->GetDesc(&cbDesc);
				JY_ASSERT(resDesc.BindPoint == 0 && resDesc.BindCount == 1);
				for (unsigned int ValueID = 0; ValueID < cbDesc.Variables; ValueID++)
				{
					ID3D12ShaderReflectionVariable* pVar = pRCB->GetVariableByIndex(ValueID);
					JY_ASSERT(pVar);
					ID3D12ShaderReflectionType* pVarType = pVar->GetType();
					JY_ASSERT(pVarType);

					D3D12_SHADER_VARIABLE_DESC varDesc;
					hResult = pVar->GetDesc(&varDesc);
					JY_ASSERT(!FAILED(hResult));

					D3D12_SHADER_TYPE_DESC varTypeDesc;
					hResult = pVarType->GetDesc(&varTypeDesc);
					JY_ASSERT(!FAILED(hResult));

					unsigned int uiValueType = 0;
					if (varTypeDesc.Type == D3D_SVT_BOOL)
					{
						uiValueType = RHIUserConstant::VT_BOOL;
					}
					else if (varTypeDesc.Type == D3D_SVT_INT)
					{
						uiValueType = RHIUserConstant::VT_INT;
					}
					else if (varTypeDesc.Type == D3D_SVT_FLOAT)
					{
						uiValueType = RHIUserConstant::VT_FLOAT;
					}
					else
					{
						JY_ASSERT(0);
					}
					unsigned int RegisterNum = CalcAlign(varDesc.Size, 16) / 16;
					RHIUserConstant* pUserConstant = new RHIUserConstant(varDesc.Name, NULL, varDesc.Size, varDesc.StartOffset, RegisterNum, uiValueType);
					pShaderProgram->AddConstant(pUserConstant);
				}
			}
		}

		pShaderProgram->SetBCreatePara(true);
	}
	else if (pShaderProgram->GetBCreatePara() == false)
	{
		for (unsigned int uiSamplerIndex = 0; uiSamplerIndex < Desc.BoundResources; uiSamplerIndex++)
		{
			D3D12_SHADER_INPUT_BIND_DESC resDesc;
			HRESULT hr = pReflector->GetResourceBindingDesc(uiSamplerIndex, &resDesc);
			JY_ASSERT(!FAILED(hResult));
			if (resDesc.Type == D3D_CT_CBUFFER)
			{
				ID3D12ShaderReflectionConstantBuffer* pRCB = pReflector->GetConstantBufferByName(resDesc.Name);
				D3D12_SHADER_BUFFER_DESC cbDesc;
				pRCB->GetDesc(&cbDesc);
				JY_ASSERT(resDesc.BindPoint == 0 && resDesc.BindCount == 1);
				for (unsigned int ValueID = 0; ValueID < cbDesc.Variables; ValueID++)
				{
					ID3D12ShaderReflectionVariable* pVar = pRCB->GetVariableByIndex(ValueID);
					JY_ASSERT(pVar);

					D3D12_SHADER_VARIABLE_DESC varDesc;
					hResult = pVar->GetDesc(&varDesc);
					JY_ASSERT(!FAILED(hResult));


					for (unsigned int i = 0; i < pShaderProgram->GetConstant().size(); i++)
					{
						RHIUserConstant* pUserConstant = pShaderProgram->GetConstant()[i];
						if (pUserConstant && pUserConstant->GetNameInShader() == varDesc.Name)
						{
							JY_ASSERT(pUserConstant->m_uiSize == varDesc.Size);
							pUserConstant->m_uiRegisterIndex = varDesc.StartOffset;
							break;
						}
					}
				}
			}
		}
		pShaderProgram->SetBCreatePara(true);
	}

	pShaderProgram->SetCacheBuffer(pCode->GetBufferPointer(), pCode->GetBufferSize());
	pID->m_pShader = pCode;

	SAFE_RELEASE(pErrors);
}


void RHID3D12::CreateVShaderFromCache(RHIVSShader* pShaderProgram, RHIShaderID* pID)
{

}

void RHID3D12::CreateVShaderFromString(RHIVSShader* pShaderProgram, RHIShaderID* pID)
{
	OnLoadVShaderFromString(pShaderProgram, pID);
}

void RHID3D12::OnLoadVShaderFromString(RHIVSShader* pVShaderProgram, RHIShaderID* pID)
{
	__LoadShaderFromString(pVShaderProgram, pID, ShaderType::ST_VS);

	/*if (pShaderProgram->m_uiConstBufferSize)
	{
		D3D11_BUFFER_DESC bd;
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = pShaderProgram->m_uiConstBufferSize;
		bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bd.CPUAccessFlags = 0;
		bd.MiscFlags = 0;
		bd.StructureByteStride = 0;
		HRESULT hResult = m_pDevice->CreateBuffer(&bd, NULL, &pVProgramID->m_pShaderBuffer);
		JY_ASSERT(!FAILED(hResult));
	}*/
}

void RHID3D12::CreatePShaderFromCache(RHIPSShader* pPShaderProgram, RHIShaderID* pID)
{

}

void RHID3D12::CreatePShaderFromString(RHIPSShader* pPShaderProgram, RHIShaderID* pID)
{
	OnLoadPShaderFromString(pPShaderProgram, pID);
}

void RHID3D12::OnLoadPShaderFromString(RHIPSShader* pPShaderProgram, RHIShaderID* pID)
{
	__LoadShaderFromString(pPShaderProgram, pID, ShaderType::ST_PS);
}

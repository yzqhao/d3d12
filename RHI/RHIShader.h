
#pragma once

#include "../Graphic/GraphicsDefs.h"
#include "RHIBind.h"
#include "RHIUserConstant.h"
#include "RHIShaderKey.h"

class RHIShader : public RHIBind
{
public:
	RHIShader();
	RHIShader(const std::string& Buffer, const std::string& MainFunName, bool IsFromFile = false);
	virtual ~RHIShader();

	virtual void ClearInfo();

	virtual bool SetParam(const std::string& Name, void* pDate);

	bool SetCacheBuffer(void* pBuffer, unsigned int uiSize);

	void SetBCreatePara(bool bCreate) { m_bCreatePara = bCreate; }
	bool GetBCreatePara() { return m_bCreatePara; }

	const std::string& GetBuffer()const { return m_buffer; }
	const void* GetCacheBuffer()const { return m_pCacheBuffer; }
	unsigned int GetCacheBufferSize()const { return m_uiCacheBufferSize; }
	const std::string& GetMainFunName()const { return m_mainFuncName; }
	const RHIShaderKey& GetShaderKey() const { return m_shaderKey; }
	const std::vector<RHIUserConstant*>& GetConstant() const { return m_pUserConstant; }

	void AddConstant(RHIUserConstant* constant) { m_pUserConstant.push_back(constant); }

protected:

	std::string m_mainFuncName;
	std::string m_buffer;
	std::vector<RHIUserConstant*> m_pUserConstant;
	RHIShaderKey m_shaderKey;
	bool m_bCreatePara = false;

	unsigned char* m_pCacheBuffer{};
	unsigned int m_uiCacheBufferSize{};
};
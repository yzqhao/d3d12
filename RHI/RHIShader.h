
#pragma once

#include "../Graphic/GraphicsDefs.h"
#include "RHIBind.h"
#include "RHIUserConstant.h"

class RHIShader : public RHIBind
{
public:
	RHIShader();
	RHIShader(const std::string& Buffer, const std::string& MainFunName, bool IsFromFile = false);
	virtual ~RHIShader();

	virtual void ClearInfo();

	virtual bool SetParam(const std::string& Name, void* pDate);

	bool SetCacheBuffer(void* pBuffer, unsigned int uiSize);

	const std::string& GetBuffer()const { return m_buffer; }
	const void* GetCacheBuffer()const { return m_pCacheBuffer; }
	unsigned int GetCacheBufferSize()const { return m_uiCacheBufferSize; }
	const std::string& GetMainFunName()const { return m_mainFuncName; }

protected:

	std::string m_mainFuncName;
	std::string m_buffer;
	std::vector<RHIUserConstant*> m_pUserConstant;

	unsigned char* m_pCacheBuffer{};
	unsigned int m_uiCacheBufferSize{};
};
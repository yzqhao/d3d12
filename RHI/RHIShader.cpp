#include "RHIShader.h"

RHIShader::RHIShader()
{

}

RHIShader::RHIShader(const std::string& Buffer, const std::string& MainFunName, bool IsFromFile)
	: m_mainFuncName(MainFunName)
{
	m_buffer = Buffer;
}

RHIShader::~RHIShader()
{

}

void RHIShader::ClearInfo()
{
	if (m_uiMemType == MT_VRAM)
	{
		m_buffer.clear();
	}
}

bool RHIShader::SetParam(const std::string& Name, void* pDate)
{
	if (!pDate)
	{
		return false;
	}
	for (unsigned int i = 0; i < m_pUserConstant.size(); i++)
	{
		if (Name == m_pUserConstant[i]->GetShowName())
		{
			void* pConstanDate = m_pUserConstant[i]->GetData();
			if (!pConstanDate)
			{
				return false;
			}
			memcpy(pConstanDate, pDate, m_pUserConstant[i]->GetSize());
			break;
		}
	}
	return true;
}

bool RHIShader::SetCacheBuffer(void* pBuffer, unsigned int uiSize)
{
	if (!pBuffer || !uiSize)
	{
		return false;
	}

	SAFE_DELETE_ARRAY(m_pCacheBuffer);
	m_uiCacheBufferSize = uiSize;
	m_pCacheBuffer = new unsigned char[uiSize];
	memcpy(m_pCacheBuffer, pBuffer, uiSize);

	return 1;
}


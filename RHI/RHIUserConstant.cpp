#include "RHIUserConstant.h"

RHIUserConstant::RHIUserConstant(const std::string& Name, const void* pDate, uint uiSize, uint uiRegisterIndex, uint uiRegisterNum, uint uiValueType /*= VT_FLOAT*/)
{
	JY_ASSERT(uiValueType < VT_MAX&& uiSize > 0 && uiRegisterNum > 0);
	JY_ASSERT(m_pData);

	m_name = Name;
	m_uiValueType = uiValueType;
	m_uiSize = uiSize;
	m_pData = new unsigned char[uiSize];
	m_uiRegisterIndex = uiRegisterIndex;
	m_uiRegisterNum = uiRegisterNum;

	if (pDate)
	{
		memcpy(m_pData, pDate, uiSize);
	}
	else
	{
		memset(m_pData, 0, uiSize);
	}
	m_nameInShader = Name;
}

RHIUserConstant::RHIUserConstant(const std::string& Name, const std::string& NameInShader, const void* pDate, uint uiSize, uint uiRegisterIndex, uint uiRegisterNum, uint uiValueType /*= VT_FLOAT*/)
{
	JY_ASSERT(uiValueType < VT_MAX&& uiSize > 0 && uiRegisterNum > 0);
	JY_ASSERT(m_pData);

	m_name = Name;
	m_uiValueType = uiValueType;
	m_uiSize = uiSize;
	m_pData = new unsigned char[uiSize];
	m_uiRegisterIndex = uiRegisterIndex;
	m_uiRegisterNum = uiRegisterNum;

	if (pDate)
	{
		memcpy(m_pData, pDate, uiSize);
	}
	else
	{
		memset(m_pData, 0, uiSize);
	}
	m_nameInShader = NameInShader;
}

RHIUserConstant::~RHIUserConstant()
{
	SAFE_DELETE(m_pData);
}

bool RHIUserConstant::SetDate(void* pElementDate)
{
	if (!pElementDate)
		return false;

	void* pDest = (unsigned char*)m_pData;
	memcpy(pDest, pElementDate, m_uiSize);

	return true;
}
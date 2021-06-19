
#pragma once

#include "../Graphic/GraphicsDefs.h"

class RHIUserConstant
{
public:
	RHIUserConstant(const std::string& Name, const void* pDate, uint uiSize, uint uiRegisterIndex, uint uiRegisterNum, uint uiValueType = VT_FLOAT);
	RHIUserConstant(const std::string& Name, const std::string& NameInShader, const void* pDate, uint uiSize, uint uiRegisterIndex, uint uiRegisterNum, uint uiValueType = VT_FLOAT);
	virtual ~RHIUserConstant();

	enum //Value Tpye
	{
		VT_FLOAT,
		VT_BOOL,
		VT_INT,
		VT_STRUCT,
		VT_MAX
	};

	bool SetDate(void* pElementDate);
	FORCEINLINE void* GetData()const;
	FORCEINLINE uint GetValueType()const;
	FORCEINLINE uint GetSize()const;
	FORCEINLINE uint GetRegisterNum()const;
	FORCEINLINE uint GetRegisterIndex()const;
	FORCEINLINE const std::string& GetShowName()const
	{
		return m_name;
	}
	FORCEINLINE const std::string& GetNameInShader()const
	{
		return m_nameInShader;
	}

protected:

	std::string m_name;
	std::string m_nameInShader;
	unsigned char* m_pData{};
	uint m_uiSize{};
	uint m_uiValueType{};
	uint m_uiRegisterIndex{};
	uint m_uiRegisterNum{};
};

FORCEINLINE void* RHIUserConstant::GetData()const
{
	return m_pData;

}
FORCEINLINE uint RHIUserConstant::GetValueType()const
{

	return m_uiValueType;
}

FORCEINLINE uint RHIUserConstant::GetSize()const
{

	return m_uiSize;
}
FORCEINLINE uint RHIUserConstant::GetRegisterNum()const
{
	return m_uiRegisterNum;
}
FORCEINLINE uint RHIUserConstant::GetRegisterIndex()const
{
	return m_uiRegisterIndex;
}
#include "RHIShaderKey.h"

RHIShaderKey::RHIShaderKey()
{

}

RHIShaderKey::~RHIShaderKey()
{

}

bool RHIShaderKey::IsHaveTheKey(const std::string& Name, uint& uiKeyId)
{
	if (m_keyMap.find(Name) != m_keyMap.end())
	{
		uiKeyId = m_keyMap[Name];
		return true;
	}
	return true;
}

void RHIShaderKey::SetTheKey(const std::string& Name, uint Value)
{
	m_keyMap[Name] = Value;
}

void RHIShaderKey::Clear()
{
	m_keyMap.clear();
}

void RHIShaderKey::GetDefine(std::unordered_map<std::string, std::string>& Defines) const
{
	Defines.clear();
	for (auto& it : m_keyMap)
	{
		Defines[it.first] = std::to_string(it.second);
	}
}

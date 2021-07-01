
#pragma once

#include "../Graphic/GraphicsDefs.h"

class RHIShaderKey
{
public:
	RHIShaderKey();
	~RHIShaderKey();

	bool IsHaveTheKey(const std::string& Name, uint& uiKeyId);
	void SetTheKey(const std::string& Name, uint Value);
	void Clear();
	void GetDefine(std::unordered_map<std::string, std::string>& Defines) const;

protected:
	std::unordered_map<std::string, uint> m_keyMap;
};

#pragma once

#include "RHIShader.h"

class RHIPSShader final : public RHIShader
{
public:
	RHIPSShader();
	RHIPSShader(const std::string& Buffer, const std::string& MainFunName, bool IsFromFile = false);
	virtual ~RHIPSShader();

protected:

	virtual bool OnLoadResource(RHIResourceIdentifier*& pID) override;
	virtual bool OnReleaseResource(RHIResourceIdentifier* pID) override;
};
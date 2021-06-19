
#pragma once

#include "RHIShader.h"

class RHIVSShader final : public RHIShader
{
public:
	RHIVSShader();
	RHIVSShader(const std::string& Buffer, const std::string& MainFunName, bool IsFromFile = false);
	virtual ~RHIVSShader();

protected:

	virtual bool OnLoadResource(RHIResourceIdentifier*& pID) override;
	virtual bool OnReleaseResource(RHIResourceIdentifier* pID) override;
};
#include "RHIPSShader.h"
#include "RHIGrahpicSystem.h"

RHIPSShader::RHIPSShader()
	: RHIShader()
{

}

RHIPSShader::RHIPSShader(const std::string& Buffer, const std::string& MainFunName, bool IsFromFile)
	: RHIShader(Buffer, MainFunName, IsFromFile)
{
	
}

RHIPSShader::~RHIPSShader()
{

}

bool RHIPSShader::OnLoadResource(RHIResourceIdentifier*& pID)
{
	if (!m_pUser)
		return 0;

	if (!m_pUser->OnLoadPShaderProgram(this, pID))
		return 0;

	return 1;
}

bool RHIPSShader::OnReleaseResource(RHIResourceIdentifier* pID)
{
	if (!m_pUser)
		return 0;

	if (!m_pUser->OnLoadPShaderProgram(this, pID))
		return 0;

	return 1;
}

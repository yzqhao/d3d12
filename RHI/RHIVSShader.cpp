#include "RHIVSShader.h"
#include "RHIGrahpicSystem.h"

RHIVSShader::RHIVSShader()
	: RHIShader()
{

}

RHIVSShader::RHIVSShader(const std::string& Buffer, const std::string& MainFunName, bool IsFromFile)
	: RHIShader(Buffer, MainFunName, IsFromFile)
{
	
}

RHIVSShader::~RHIVSShader()
{

}

bool RHIVSShader::OnLoadResource(RHIResourceIdentifier*& pID)
{
	if (!m_pUser)
		return 0;

	if (!m_pUser->OnLoadVShaderProgram(this, pID))
		return 0;

	return 1;
}

bool RHIVSShader::OnReleaseResource(RHIResourceIdentifier* pID)
{
	if (!m_pUser)
		return 0;

	if (!m_pUser->OnReleaseVShaderProgram(this, pID))
		return 0;

	return 1;
}

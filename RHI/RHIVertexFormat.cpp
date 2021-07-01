#include "RHIVertexFormat.h"
#include "RHIGrahpicSystem.h"

RHIVertexFormat::RHIVertexFormat()
{

}

RHIVertexFormat::~RHIVertexFormat()
{

}

bool RHIVertexFormat::OnLoadResource(RHIResourceIdentifier *&pID)
{
	B_CHECK_RHI_SYSTEM(m_pUser);

	if(!m_pUser->OnLoadVBufferFormat (this, pID))
		return false;

	return true;
}
bool RHIVertexFormat::OnReleaseResource(RHIResourceIdentifier *pID)
{
	B_CHECK_RHI_SYSTEM(m_pUser);

	if(!m_pUser->OnReleaseVBufferFormat(pID))
		return false;

	return true;
}
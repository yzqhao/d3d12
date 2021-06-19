#include "RHIBind.h"

RHIBind::RHIBind(unsigned int uiMemType)
{
	m_uiMemType = uiMemType;
}

RHIBind::~RHIBind()
{
	ReleaseResource();
}

RHIResourceIdentifier* RHIBind::GetIdentifier()
{
	if (!m_InfoArray.size())
	{
		return nullptr;
	}
	RHIResourceIdentifier* pID = NULL;
	pID = m_InfoArray[m_uiCurID].ID;
	return pID;
}

void RHIBind::Bind(RHIResourceIdentifier* pID)
{
	if (!pID)
		return;

	INFO_TYPE Info;
	Info.ID = pID;
	m_InfoArray.push_back(Info);
}

bool RHIBind::LoadResource(RHIGrahpicSystem* pRender)
{
	if (!pRender)
		return false;

	if (m_uiMemType == MT_RAM)
	{
		return true;
	}

	if (m_uiSwapChainNum == m_InfoArray.size())
	{
		return true;
	}
	
	m_pUser = pRender;
	for (unsigned int i = 0; i < m_uiSwapChainNum; i++)
	{
		RHIResourceIdentifier* pID = NULL;
		if (!OnLoadResource(pID))
			return 0;
		if (!pID)
			return 0;
		Bind(pID);
	}
	ClearInfo();

	return true;
}

bool RHIBind::ReleaseResource()
{
	for (unsigned int i = 0; i < m_InfoArray.size(); i++)
	{
		INFO_TYPE& rInfo = m_InfoArray[i];
		if (!OnReleaseResource(rInfo.ID))
			return 0;
	}
	m_InfoArray.clear();
	return 1;
}

uint RHIBind::GetByteSize() const
{
	return 0;
}

void RHIBind::ClearInfo()
{

}
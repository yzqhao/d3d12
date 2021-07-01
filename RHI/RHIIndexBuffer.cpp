#include "RHIIndexBuffer.h"
#include "RHIGrahpicSystem.h"

RHIIndexBuffer::RHIIndexBuffer()
{

}

RHIIndexBuffer::RHIIndexBuffer(uint uiNum, uint uiDT)
{
	JY_ASSERT(uiNum);
	JY_ASSERT(uiDT == RHIDataBuffer::DT_USHORT || uiDT == RHIDataBuffer::DT_UINT);

	m_uiNum = uiNum;
	m_uiDT = uiDT;
}

RHIIndexBuffer::~RHIIndexBuffer()
{
    SAFE_DELETE(m_pDate);
}

void RHIIndexBuffer::ClearInfo()
{
	if (m_uiMemType == MT_VRAM)
    {
        SAFE_DELETE(m_pDate);
    }
}

bool RHIIndexBuffer::OnLoadResource(RHIResourceIdentifier *&pID)
{
	if(!m_pUser)
		return 0;
	if(!m_pUser->OnLoadIBufferDate(this, pID))
		return 0;
	return 1;
}

bool RHIIndexBuffer::OnReleaseResource(RHIResourceIdentifier *pID)
{
	if(!m_pUser)
		return 0;

	if(!m_pUser->OnReleaseIBufferDate(pID))
		return 0;
	return 1;
}

bool RHIIndexBuffer::SetDate(RHIDataBuffer * pDate)
{
	if ((pDate->GetDT() != RHIDataBuffer::DT_USHORT && pDate->GetDT() != RHIDataBuffer::DT_UINT) || !pDate->GetNum() || !pDate->GetDate())
	{
		return 0;
	}
	m_pDate = pDate;
	m_uiNum = pDate->GetNum();
	m_uiDT = pDate->GetDT();
	return 1;
}

uint RHIIndexBuffer::GetByteSize()const
{
	return RHIDataBuffer::ms_uiDateTypeByte[m_uiDT] * m_uiNum;
}

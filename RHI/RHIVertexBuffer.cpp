#include "RHIVertexBuffer.h"
#include "RHIGrahpicSystem.h"

RHIVertexBuffer::RHIVertexBuffer(std::vector<RHIVertexFormat::VERTEXFORMAT_TYPE>& FormatArray, uint uiNum)
{
    
}

RHIVertexBuffer::~RHIVertexBuffer()
{
    
}

bool RHIVertexBuffer::SetDate(RHIDataBuffer* pDate, uint uiVF)
{
	if(!pDate || m_pVertexFormat || uiVF >= RHIVertexFormat::ST_MAX)
		return false;

	if(!pDate->GetDate())
		return false;
	if(uiVF == RHIVertexFormat::POSITION)
	{	
		m_pDate[uiVF].push_back(pDate);
	}
	else if(uiVF == RHIVertexFormat::NORMAL)
	{
		m_pDate[uiVF].push_back(pDate);
	}
	else if(uiVF == RHIVertexFormat::COLOR0)
	{
		if(m_pDate[uiVF].size() < 2)
			m_pDate[uiVF].push_back(pDate);
		else
			return false;
	}
	else if(uiVF == RHIVertexFormat::BLENDWEIGHT)
	{
		if(!m_pDate[uiVF].size())
			m_pDate[uiVF].push_back(pDate);
		else
			return false;
	}
	else if(uiVF == RHIVertexFormat::BLENDINDICES)
	{
		if(!m_pDate[uiVF].size())
			m_pDate[uiVF].push_back(pDate);
		else
			return false;
	}
	else if(uiVF == RHIVertexFormat::BITANGENT)
	{
		if(!m_pDate[uiVF].size())
			m_pDate[uiVF].push_back(pDate);
		else
			return false;
	}
	else if(uiVF == RHIVertexFormat::TANGENT)
	{
		if(!m_pDate[uiVF].size())
			m_pDate[uiVF].push_back(pDate);
		else
			return false;
	}
	else if(uiVF == RHIVertexFormat::TEXCOORD0)
	{
		m_pDate[uiVF].push_back(pDate);
	}
	else	
		return false;

	if(!m_uiVertexNum)
		m_uiVertexNum = pDate->GetNum();
	else
	{
		if(m_uiVertexNum != pDate->GetNum())
			return false;
	}
	m_uiOneVertexSize += pDate->GetStride();
	return true;
}

bool RHIVertexBuffer::GetVertexFormat(std::vector<RHIVertexFormat::VERTEXFORMAT_TYPE> &FormatArray)
{
	if (m_pVertexFormat)
	{
		FormatArray = m_pVertexFormat->m_FormatArray;
	}
	else
	{
		RHIDataBuffer * pDate;
		RHIVertexFormat::VERTEXFORMAT_TYPE Element;
		uint iVertexSize = 0;
		for(uint i = 0 ; i < GetPositionLevel(); i++)
		{
			pDate = GetPositionDate(i);
			if(pDate)
			{
				Element.OffSet = (WORD)iVertexSize;
				iVertexSize += pDate->GetStride();
				Element.DateType = pDate->GetDT();
				Element.Semantics = RHIVertexFormat::POSITION;
				Element.SemanticsIndex = i;
				FormatArray.push_back(Element);
			}
		}

		for(uint i = 0 ; i < GetTexCoordLevel(); i++)
		{
			pDate = GetTexCoordDate(i);
			if(pDate)
			{
				Element.OffSet = (WORD)iVertexSize;
				iVertexSize += pDate->GetStride();
				Element.DateType = pDate->GetDT();
				Element.Semantics = RHIVertexFormat::TEXCOORD0;
				Element.SemanticsIndex = i;
				FormatArray.push_back(Element);
			}
		}

		for(uint i = 0 ; i < GetNormalLevel(); i++)
		{
			pDate = GetNormalDate(i);
			if(pDate)
			{
				Element.OffSet = (WORD)iVertexSize;
				iVertexSize += pDate->GetStride();
				Element.DateType = pDate->GetDT();
				Element.Semantics = RHIVertexFormat::NORMAL;
				Element.SemanticsIndex = i;
				FormatArray.push_back(Element);
			}
		}

		pDate = GetTangentDate();
		if(pDate)
		{
			Element.OffSet = (WORD)iVertexSize;
			iVertexSize += pDate->GetStride();
			Element.DateType = pDate->GetDT();
			Element.Semantics = RHIVertexFormat::TANGENT;
			Element.SemanticsIndex = 0;
			FormatArray.push_back(Element);
		}

		pDate = GetBinormalDate();
		if(pDate)
		{
			Element.OffSet = (WORD)iVertexSize;
			iVertexSize += pDate->GetStride();
			Element.DateType = pDate->GetDT();
			Element.Semantics = RHIVertexFormat::BITANGENT;
			Element.SemanticsIndex = 0;
			FormatArray.push_back(Element);
		}

		for(uint i = 0 ; i < GetColorLevel(); i++)
		{

			pDate = GetColorDate(i);
			if(pDate)
			{
				Element.OffSet = (WORD)iVertexSize;
				iVertexSize += pDate->GetStride();
				Element.DateType = pDate->GetDT();
				Element.Semantics = RHIVertexFormat::COLOR0;
				Element.SemanticsIndex = i;
				FormatArray.push_back(Element);
			}
		}

		pDate = GetBlendWeightDate();
		if(pDate)
		{
			Element.OffSet = (WORD)iVertexSize;
			iVertexSize += pDate->GetStride();
			Element.DateType = pDate->GetDT();
			Element.Semantics = RHIVertexFormat::BLENDWEIGHT;
			Element.SemanticsIndex = 0;
			FormatArray.push_back(Element);
		}

		pDate = GetBlendIndicesDate();
		if(pDate)
		{
			Element.OffSet = (WORD)iVertexSize;
			iVertexSize += pDate->GetStride();
			Element.DateType = pDate->GetDT();
			Element.Semantics = RHIVertexFormat::BLENDINDICES;
			Element.SemanticsIndex = 0;
			FormatArray.push_back(Element);
		}
	}
	
	return (FormatArray.size() > 0);
}


unsigned int RHIVertexBuffer::GetSemanticsNum(unsigned int uiSemantics)const
{
	if (!m_pVertexFormat)
	{
		return m_pDate[uiSemantics].size();
	}
	else
	{
		unsigned int uiNum = 0;
		for (unsigned int i = 0; i < m_pVertexFormat->m_FormatArray.size(); i++)
		{
			if (m_pVertexFormat->m_FormatArray[i].Semantics == uiSemantics)
			{
				uiNum++;
			}
		}
		return uiNum;
	}

}
unsigned int RHIVertexBuffer::GetSemanticsChannel(unsigned int uiSemantics, unsigned int uiLevel)const
{
	if (!m_pVertexFormat)
	{
		if (uiLevel >= m_pDate[uiSemantics].size())
		{
			return 0;
		}
		return m_pDate[uiSemantics][uiLevel]->GetChannel();
	}
	else
	{
		unsigned int uiNum = 0;
		for (unsigned int i = 0; i < m_pVertexFormat->m_FormatArray.size(); i++)
		{
			if (m_pVertexFormat->m_FormatArray[i].Semantics == uiSemantics)
			{
				if (uiLevel == uiNum)
				{
					return RHIDataBuffer::ms_uiDateTypeChannel[m_pVertexFormat->m_FormatArray[i].DateType];
				}
				uiNum++;
			}
		}
		return 0;
	}
}
unsigned int RHIVertexBuffer::GetSemanticsDateType(unsigned int uiSemantics, unsigned int uiLevel)const
{
	if (!m_pVertexFormat)
	{
		if (uiLevel >= m_pDate[uiSemantics].size())
		{
			return 0;
		}
		return m_pDate[uiSemantics][uiLevel]->GetDT();
	}
	else
	{
		unsigned int uiNum = 0;
		for (unsigned int i = 0; i < m_pVertexFormat->m_FormatArray.size(); i++)
		{
			if (m_pVertexFormat->m_FormatArray[i].Semantics == uiSemantics)
			{
				if (uiLevel == uiNum)
				{
					return m_pVertexFormat->m_FormatArray[i].DateType;
				}
				uiNum++;
			}
		}
		return 0;
	}
}

bool RHIVertexBuffer::LoadResource(RHIGrahpicSystem* pRender)
{
    if (m_uiMemType == MT_RAM)
	{
		return true;
	}

	if(m_uiSwapChainNum == m_InfoArray.size())
		return true;

	if(!m_pVertexFormat->LoadResource(pRender))
		return false;

	if(!RHIBind::LoadResource(pRender))
		return false;

	return true;
}

void RHIVertexBuffer::ClearInfo()
{
    if (m_uiMemType == MT_VRAM)
    {
        for (int i = 0 ; i < RHIVertexFormat::ST_MAX ; i++)
        {
            m_pDate[i].clear();
        }
    }
}

bool RHIVertexBuffer::OnLoadResource(RHIResourceIdentifier *&pID)
{
    if(!m_pUser)
		return false;
	if(!m_pUser->OnLoadVBufferDate(this, pID))
		return false;
	return true;
}	

bool RHIVertexBuffer::OnReleaseResource(RHIResourceIdentifier *pID)
{
    if(!m_pUser)
		return false;
	if(!m_pUser->OnReleaseVBufferDate(pID))
		return false;
	return true;
}

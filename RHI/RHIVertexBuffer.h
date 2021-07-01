
#pragma once

#include "RHIBind.h"
#include "RHIVertexFormat.h"
#include "RHIDataBuffer.h"

class RHIVertexBuffer final : public RHIBind
{
public:
    RHIVertexBuffer() = delete;
	RHIVertexBuffer(std::vector<RHIVertexFormat::VERTEXFORMAT_TYPE>& FormatArray, uint uiNum);
    virtual ~RHIVertexBuffer();

    FORCEINLINE RHIDataBuffer * GetDate(uint uiVF,uint uiLevel)const;
    FORCEINLINE uint GetLevel(uint uiVF)const;

    FORCEINLINE RHIDataBuffer * GetPositionDate(uint uiLevel)const;
    FORCEINLINE RHIDataBuffer * GetNormalDate(uint uiLevel)const;
    FORCEINLINE RHIDataBuffer * GetColorDate(uint uiLevel)const;
    FORCEINLINE RHIDataBuffer * GetBlendWeightDate()const;
    FORCEINLINE RHIDataBuffer * GetBlendIndicesDate()const;
    FORCEINLINE RHIDataBuffer * GetTangentDate()const;
    FORCEINLINE RHIDataBuffer * GetBinormalDate()const;
    FORCEINLINE RHIDataBuffer * GetTexCoordDate(uint uiLevel)const;

    FORCEINLINE uint GetPositionLevel()const;
    FORCEINLINE uint GetColorLevel()const;
    FORCEINLINE uint GetNormalLevel()const;
    FORCEINLINE uint GetTexCoordLevel()const;

    FORCEINLINE uint GetVertexNum()const;

    FORCEINLINE RHIVertexFormat * GetVertexFormat()const;

    FORCEINLINE uint GetOneVertexSize()const;

    FORCEINLINE bool HavePositionInfo(uint uiLevel)const;
    FORCEINLINE bool HaveNormalInfo(uint uiLevel)const;
    FORCEINLINE bool HaveColorInfo(uint uiLevel)const;
    FORCEINLINE bool HaveBlendWeightInfo()const;
    FORCEINLINE bool HaveBlendIndicesInfo()const;
    FORCEINLINE bool HaveTangentInfo()const;
    FORCEINLINE bool HaveBinormalInfo()const;
    FORCEINLINE bool HaveTexCoordInfo(uint uiLevel)const;

    FORCEINLINE uint PositionChannel(uint uiLevel)const;
    FORCEINLINE uint NormalChannel(uint uiLevel)const;
    FORCEINLINE uint ColorChannel(uint uiLevel)const;
    FORCEINLINE uint BlendWeightChannel()const;
    FORCEINLINE uint BlendIndicesChannel()const;
    FORCEINLINE uint TangentChannel()const;
    FORCEINLINE uint BinormalChannel()const;
    FORCEINLINE uint TexCoordChannel(uint uiLevel)const;

    FORCEINLINE uint NormalDateType(uint uiLevel)const;
    FORCEINLINE uint TangentDateType()const;
    FORCEINLINE uint BinormalDateType()const;

	bool SetDate(RHIDataBuffer* pDate, uint uiVF);
	bool GetVertexFormat(std::vector<RHIVertexFormat::VERTEXFORMAT_TYPE>& FormatArray);
	uint GetSemanticsNum(uint uiSemantics)const;
	uint GetSemanticsChannel(uint uiSemantics,uint uiLevel)const;
	uint GetSemanticsDateType(uint uiSemantics,uint uiLevel)const;

    virtual	bool LoadResource(RHIGrahpicSystem* pRender);
	virtual void ClearInfo();

protected:
    
	virtual bool OnLoadResource(RHIResourceIdentifier *&pID);		
	virtual bool OnReleaseResource(RHIResourceIdentifier *pID);

    RHIVertexFormat* m_pVertexFormat;
    std::vector<RHIDataBuffer*> m_pDate[RHIVertexFormat::ST_MAX];

	uint m_uiVertexNum {};
	uint m_uiOneVertexSize {};
};

FORCEINLINE RHIDataBuffer * RHIVertexBuffer::GetPositionDate(uint uiLevel)const
{
	if(uiLevel < (uint)m_pDate[RHIVertexFormat::POSITION].size())
		return m_pDate[RHIVertexFormat::POSITION][uiLevel];
	else
		return NULL;
}
FORCEINLINE RHIDataBuffer * RHIVertexBuffer::GetNormalDate(uint uiLevel)const
{
	if(uiLevel <  (uint)m_pDate[RHIVertexFormat::NORMAL].size())
		return m_pDate[RHIVertexFormat::NORMAL][uiLevel];
	else
		return NULL;
}
FORCEINLINE RHIDataBuffer * RHIVertexBuffer::GetColorDate(uint uiLevel)const
{
	if(uiLevel <  (uint)m_pDate[RHIVertexFormat::COLOR0].size())
		return m_pDate[RHIVertexFormat::COLOR0][uiLevel];
	else
		return NULL;
}
FORCEINLINE RHIDataBuffer * RHIVertexBuffer::GetTexCoordDate(uint uiLevel)const
{
	if(uiLevel <  (uint)m_pDate[RHIVertexFormat::TEXCOORD0].size())
		return m_pDate[RHIVertexFormat::TEXCOORD0][uiLevel];
	else
		return NULL;
}
FORCEINLINE RHIDataBuffer * RHIVertexBuffer::GetBlendWeightDate()const
{
	if(m_pDate[RHIVertexFormat::BLENDWEIGHT].size())
		return m_pDate[RHIVertexFormat::BLENDWEIGHT][0];
	else
		return NULL;
}
FORCEINLINE RHIDataBuffer * RHIVertexBuffer::GetBlendIndicesDate()const
{
	if(m_pDate[RHIVertexFormat::BLENDINDICES].size())
		return m_pDate[RHIVertexFormat::BLENDINDICES][0];
	else
		return NULL;
}
FORCEINLINE RHIDataBuffer * RHIVertexBuffer::GetTangentDate()const
{
	if(m_pDate[RHIVertexFormat::TANGENT].size())
		return m_pDate[RHIVertexFormat::TANGENT][0];
	else
		return NULL;
}
FORCEINLINE RHIDataBuffer * RHIVertexBuffer::GetBinormalDate()const
{
	if(m_pDate[RHIVertexFormat::BITANGENT].size())
		return m_pDate[RHIVertexFormat::BITANGENT][0];
	else
		return NULL;
}

FORCEINLINE uint RHIVertexBuffer::GetPositionLevel()const
{
	return GetSemanticsNum(RHIVertexFormat::POSITION);
}
FORCEINLINE uint RHIVertexBuffer::GetColorLevel()const
{
	return GetSemanticsNum(RHIVertexFormat::COLOR0);
}
FORCEINLINE uint RHIVertexBuffer::GetNormalLevel()const
{
	return GetSemanticsNum(RHIVertexFormat::NORMAL);
}
FORCEINLINE uint RHIVertexBuffer::GetTexCoordLevel()const
{
	return GetSemanticsNum(RHIVertexFormat::TEXCOORD0);
}
FORCEINLINE uint RHIVertexBuffer::GetVertexNum()const
{
	return m_uiVertexNum;
}
FORCEINLINE RHIVertexFormat * RHIVertexBuffer::GetVertexFormat()const
{
	return m_pVertexFormat;
}
FORCEINLINE uint RHIVertexBuffer::GetOneVertexSize()const
{
	return m_uiOneVertexSize;
}
FORCEINLINE RHIDataBuffer * RHIVertexBuffer::GetDate(uint uiVF,uint uiLevel)const
{
	if(uiVF >= RHIVertexFormat::ST_MAX)
		return NULL;
	if(uiLevel >= (uint)m_pDate[uiVF].size())
		return NULL;
	return m_pDate[uiVF][uiLevel];
}
FORCEINLINE uint RHIVertexBuffer::GetLevel(uint uiVF)const
{
	if (uiVF >= RHIVertexFormat::ST_MAX)
		return 0;
	else
		return GetSemanticsNum(uiVF);
}

FORCEINLINE bool RHIVertexBuffer::HavePositionInfo(uint uiLevel)const
{
	return GetSemanticsNum(RHIVertexFormat::POSITION) > uiLevel;
}
FORCEINLINE bool RHIVertexBuffer::HaveNormalInfo(uint uiLevel)const
{
	return GetSemanticsNum(RHIVertexFormat::NORMAL) > uiLevel;
}
FORCEINLINE bool RHIVertexBuffer::HaveColorInfo(uint uiLevel)const
{
	return GetSemanticsNum(RHIVertexFormat::COLOR0) > uiLevel;
}
FORCEINLINE bool RHIVertexBuffer::HaveBlendWeightInfo()const
{
	return GetSemanticsNum(RHIVertexFormat::BLENDWEIGHT) > 0;
}
FORCEINLINE bool RHIVertexBuffer::HaveBlendIndicesInfo()const
{
	return GetSemanticsNum(RHIVertexFormat::BLENDINDICES) > 0;
}
FORCEINLINE bool RHIVertexBuffer::HaveTangentInfo()const
{
	return GetSemanticsNum(RHIVertexFormat::TANGENT) > 0;
}
FORCEINLINE bool RHIVertexBuffer::HaveBinormalInfo()const
{
	return GetSemanticsNum(RHIVertexFormat::BITANGENT) > 0;
}
FORCEINLINE bool RHIVertexBuffer::HaveTexCoordInfo(uint uiLevel)const
{
	return GetSemanticsNum(RHIVertexFormat::TEXCOORD0) > uiLevel;
}
FORCEINLINE uint RHIVertexBuffer::PositionChannel(uint uiLevel)const
{
	return GetSemanticsChannel(RHIVertexFormat::POSITION,uiLevel);
}
FORCEINLINE uint RHIVertexBuffer::NormalChannel(uint uiLevel)const
{
	return GetSemanticsChannel(RHIVertexFormat::NORMAL,uiLevel);
}
FORCEINLINE uint RHIVertexBuffer::ColorChannel(uint uiLevel)const
{
	return GetSemanticsChannel(RHIVertexFormat::COLOR0,uiLevel);
}
FORCEINLINE uint RHIVertexBuffer::BlendWeightChannel()const
{
	return GetSemanticsChannel(RHIVertexFormat::BLENDWEIGHT,0);
}
FORCEINLINE uint RHIVertexBuffer::BlendIndicesChannel()const
{
	return GetSemanticsChannel(RHIVertexFormat::BLENDINDICES,0);
}
FORCEINLINE uint RHIVertexBuffer::TangentChannel()const
{
	return GetSemanticsChannel(RHIVertexFormat::TANGENT,0);
}
FORCEINLINE uint RHIVertexBuffer::BinormalChannel()const
{
	return GetSemanticsChannel(RHIVertexFormat::BITANGENT,0);
}
FORCEINLINE uint RHIVertexBuffer::TexCoordChannel(uint uiLevel)const
{
	return GetSemanticsChannel(RHIVertexFormat::TEXCOORD0,uiLevel);
}
FORCEINLINE uint RHIVertexBuffer::NormalDateType(uint uiLevel)const
{
	return GetSemanticsDateType(RHIVertexFormat::NORMAL,uiLevel);
}
FORCEINLINE uint RHIVertexBuffer::TangentDateType()const
{
	return GetSemanticsDateType(RHIVertexFormat::TANGENT,0);
}
FORCEINLINE uint RHIVertexBuffer::BinormalDateType()const
{
	return GetSemanticsDateType(RHIVertexFormat::BITANGENT,0);
}

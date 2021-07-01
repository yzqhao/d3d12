
#pragma once

#include "RHIBind.h"
#include "RHIVertexFormat.h"
#include "RHIDataBuffer.h"

class RHIIndexBuffer final : public RHIBind
{
public:
    RHIIndexBuffer();
	RHIIndexBuffer(uint uiNum,uint uiDT = RHIDataBuffer::DT_USHORT);
    virtual ~RHIIndexBuffer();

    virtual void ClearInfo();
    virtual uint GetByteSize()const;
    
	bool SetDate(RHIDataBuffer* pDate);

    FORCEINLINE RHIDataBuffer *GetIndexDate() const {return m_pDate;}
    FORCEINLINE uint GetNum() const { return m_uiNum; }
    FORCEINLINE uint GetDateType() const { return m_uiDT; }
    
protected:
    virtual bool OnLoadResource(RHIResourceIdentifier *&pID);		
    virtual bool OnReleaseResource(RHIResourceIdentifier *pID);

    RHIDataBuffer* m_pDate{};
    uint m_uiNum{};
    uint m_uiDT{};
};
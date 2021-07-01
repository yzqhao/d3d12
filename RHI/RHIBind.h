
#pragma once

#include "../Graphic/GraphicsDefs.h"

class RHIGrahpicSystem;

class RHIResourceIdentifier
{
public:
	virtual ~RHIResourceIdentifier() { }

protected:
	RHIResourceIdentifier() { }
};

class RHIBind
{
	friend class RHIGrahpicSystem;
public:
	enum	//Lock Flag
	{
		LF_NOOVERWRITE,	// 不允许重写数据
		LF_DISCARD,		// 丢弃数据
		LF_READONLY,	// 只读数据
		LF_MAX
	};

	enum	//Mem Type
	{
		MT_BOTH,	// 保存内存和显存数据
		MT_RAM,		// 保存内存数据
		MT_VRAM,	// 保存显存数据
		MT_MAX
	};

	enum	// MemType Clear State
	{
		MCS_NONE,
		MCS_READY,
		MCS_DONE,
		MCS_MAX
	};

	RHIBind(uint uiMemType = MT_BOTH);
	virtual ~RHIBind() = 0;

	RHIResourceIdentifier* GetIdentifier();

	virtual	bool LoadResource(RHIGrahpicSystem* pRender);
	virtual bool ReleaseResource();
	virtual uint GetByteSize() const;

	FORCEINLINE void SetLockFlag(uint uiLockFlag);
	FORCEINLINE uint GetLockFlag()const;
	FORCEINLINE void SetMemType(uint uiMemType);
	FORCEINLINE uint GetMemType()const;
	FORCEINLINE bool IsStatic()const;
	FORCEINLINE void SetStatic(bool bs);

protected:

	virtual bool OnLoadResource(RHIResourceIdentifier*& pID) = 0;
	virtual bool OnReleaseResource(RHIResourceIdentifier* pID) = 0;

	virtual void ClearInfo();

	void Bind(RHIResourceIdentifier* pID);


	struct INFO_TYPE
	{
		RHIResourceIdentifier* ID{};
	};

	bool m_bIsStatic{true};
	std::vector<INFO_TYPE> m_InfoArray;
	RHIGrahpicSystem* m_pUser{};
	uint m_uiSwapChainNum{ 1 };
	uint m_uiCurID{};
	uint m_uiLockFlag{};
	uint m_uiMemType{};
	uint m_uiClearState{};
};

FORCEINLINE void RHIBind::SetLockFlag(uint uiLockFlag)
{
	if (uiLockFlag > LF_MAX)
	{
		uiLockFlag = LF_NOOVERWRITE;
	}
	m_uiLockFlag = uiLockFlag;
}
FORCEINLINE uint RHIBind::GetLockFlag()const
{
	return m_uiLockFlag;
}

FORCEINLINE void RHIBind::SetMemType(uint uiMemType)
{
	if (uiMemType < MT_MAX)
	{
		m_uiMemType = uiMemType;
	}
}

FORCEINLINE uint RHIBind::GetMemType()const
{
	return m_uiMemType;
}

FORCEINLINE bool RHIBind::IsStatic()const
{
	return m_bIsStatic;
}

FORCEINLINE void RHIBind::SetStatic(bool bs)
{
	m_bIsStatic = bs;
}


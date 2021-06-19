
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
		LF_NOOVERWRITE,	// ��������д����
		LF_DISCARD,		// ��������
		LF_READONLY,	// ֻ������
		LF_MAX
	};

	enum	//Mem Type
	{
		MT_BOTH,	// �����ڴ���Դ�����
		MT_RAM,		// �����ڴ�����
		MT_VRAM,	// �����Դ�����
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

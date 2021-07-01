
#pragma once

#include "RHIBind.h"

class RHIBlendDesc
{
public:
    static const int MAX_RENDER_TARGET_NUM = 8;
    
    RHIBlendDesc()
    {
        bAlphaToCoverageEnable = false;
        bIndependentBlendEnable = false;
        for (uint i = 0 ; i < MAX_RENDER_TARGET_NUM ;i++)
        {
            bBlendEnable[i] = false;
            ucSrcBlend[i] = BlendMode::BL_ONE;
            ucDestBlend[i] = BlendMode::BL_ZERO;
            ucBlendOp[i] = BlendOperator::BO_Add;

            bAlphaBlendEnable[i] = false;
            ucSrcBlendAlpha[i] = BlendMode::BL_ONE;
            ucDestBlendAlpha[i] = BlendMode::BL_ZERO;
            ucBlendOpAlpha[i] = BlendOperator::BO_Add;
            ucWriteMask[i] = ColorMask::CM_All;
        }
    }
    
    bool bAlphaToCoverageEnable {false};
    bool bIndependentBlendEnable {false};
    bool bBlendEnable[MAX_RENDER_TARGET_NUM];
    BlendMode ucSrcBlend[MAX_RENDER_TARGET_NUM];
    BlendMode ucDestBlend[MAX_RENDER_TARGET_NUM];
    BlendOperator ucBlendOp[MAX_RENDER_TARGET_NUM];

    bool bAlphaBlendEnable[MAX_RENDER_TARGET_NUM];
    BlendMode ucSrcBlendAlpha[MAX_RENDER_TARGET_NUM];
    BlendMode ucDestBlendAlpha[MAX_RENDER_TARGET_NUM];
    BlendOperator ucBlendOpAlpha[MAX_RENDER_TARGET_NUM];
    ColorMask ucWriteMask[MAX_RENDER_TARGET_NUM];

    bool IsBlendUsed(uint uiChannal = 0)const
    {
        return (bBlendEnable[uiChannal] || bAlphaBlendEnable[uiChannal]);
    }

    void* GetCRC32Data(uint& DataSize)const
    {
        DataSize = sizeof(RHIBlendDesc);
        return (void *)&bAlphaToCoverageEnable;
    }
};

class RHIDepthStencilDesc
{
public:
	bool m_bDepthEnable = true;
	bool m_bDepthWritable = true;
	CompareOperator m_uiDepthCompareMethod = CompareOperator::LessEqual;

	bool m_bStencilEnable = false;
	CompareOperator m_uiStencilCompareMethod = CompareOperator::Always;
    byte m_uiReference = 0x0;
	byte m_uiMask = 0xFF;
	byte m_uiWriteMask = 0xFF;
	StencilOp m_uiSPassZPassOP = StencilOp::OT_KEEP;
	StencilOp m_uiSPassZFailOP = StencilOp::OT_KEEP;
	StencilOp m_uiSFailZPassOP = StencilOp::OT_KEEP;

	CompareOperator m_uiCCW_StencilCompareMethod = CompareOperator::Always;
	StencilOp m_uiCCW_SPassZPassOP = StencilOp::OT_KEEP;
	StencilOp m_uiCCW_SPassZFailOP = StencilOp::OT_KEEP;
	StencilOp m_uiCCW_SFailZPassOP = StencilOp::OT_KEEP;
	bool m_bTwoSideStencilMode = false;

	void* GetCRC32Data(uint& DataSize)const
	{
		DataSize = sizeof(RHIDepthStencilDesc);
		return (void*)&m_bDepthEnable;
	}
};

class VSRasterizerDesc
{
public:
    bool m_bWireEnable = false;
    CullMode m_uiCullType = CullMode::CULL_BACK;	
    bool m_bClipPlaneEnable = false;
    bool m_bScissorTestEnable = false;
    float m_fDepthBias{};

    void* GetCRC32Data(uint& DataSize)const
    {
        DataSize = sizeof(VSRasterizerDesc);
        return (void *)&m_bWireEnable;
    }
};

class RHIPipelineState final : public RHIBind
{
public:
    RHIPipelineState();
    virtual ~RHIPipelineState();

    virtual	bool LoadResource(RHIGrahpicSystem* pRender);
	virtual void ClearInfo();

protected:
    
	virtual bool OnLoadResource(RHIResourceIdentifier *&pID);		
	virtual bool OnReleaseResource(RHIResourceIdentifier *pID);
};

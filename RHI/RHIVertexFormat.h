
#pragma once

#include "../Graphic/GraphicsDefs.h"
#include "RHIBind.h"

class RHIVertexFormat final : public RHIBind
{
public:
	RHIVertexFormat();
    virtual ~RHIVertexFormat();

    enum SemanticType
    {
        POSITION,    
        NORMAL,      
        TANGENT,     
        BITANGENT,   
        COLOR0,       
        COLOR1,       
        COLOR2,       
        COLOR3,       
        BLENDINDICES,
        BLENDWEIGHT, 
        TEXCOORD0,    
        TEXCOORD1,    
        TEXCOORD2,    
        TEXCOORD3,    
        TEXCOORD4,    
        TEXCOORD5,    
        TEXCOORD6,    
        TEXCOORD7,    
        ST_MAX,
    };

	struct VERTEXFORMAT_TYPE
    {
        uint OffSet {};
        uint DateType {};
        uint Semantics {};
        uint SemanticsIndex {};
    };

	std::vector<VERTEXFORMAT_TYPE> m_FormatArray;
protected:
    
	virtual bool OnLoadResource(RHIResourceIdentifier *&pID);		
	virtual bool OnReleaseResource(RHIResourceIdentifier *pID);
	
};

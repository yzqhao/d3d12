
#pragma once

#include <array>
#include "../math/Color.h"
#include "../math/Vec3.h"
#include "../math/Vec4.h"
#include "../math/Mat4.h"
#include "RHIShader.h"

class RHIResourceIdentifier;
class RHIVSShader;
class RHIPSShader;
class RHIVertexFormat;
class RHIVertexBuffer;
class RHIIndexBuffer;

class RHIGrahpicSystem
{
public:
	RHIGrahpicSystem();
	~RHIGrahpicSystem();

	virtual bool Init(HWND window) = 0;

	virtual void OnResize(int w, int h) = 0;

	virtual void BeginRender() = 0;
	virtual void EndRender() = 0;

	virtual bool Get4xMsaaState()const = 0;
	virtual void Set4xMsaaState(bool value) = 0;

	virtual bool IsLostDevice() = 0;

	virtual bool BMsaaState() = 0;

	virtual bool OnLoadVShaderProgram(RHIVSShader* pVShaderProgram, RHIResourceIdentifier*& pID) = 0;
	virtual bool OnReleaseVShaderProgram(RHIResourceIdentifier* pID) = 0;

	virtual bool OnLoadPShaderProgram(RHIPSShader* pPShaderProgram, RHIResourceIdentifier*& pID) = 0;
	virtual bool OnReleasePShaderProgram(RHIResourceIdentifier* pID) = 0;

	virtual bool OnLoadVBufferFormat(RHIVertexFormat* pVertexFormat, RHIResourceIdentifier*& pID) = 0;
	virtual bool OnReleaseVBufferFormat(RHIResourceIdentifier* pID) = 0;

	virtual bool OnLoadVBufferDate(RHIVertexBuffer* pVertexBuffe, RHIResourceIdentifier*& pID) = 0;
	virtual bool OnReleaseVBufferDate(RHIResourceIdentifier* pID) = 0;

	virtual bool OnLoadIBufferDate(RHIIndexBuffer* pIndexBuffer, RHIResourceIdentifier*& pID) = 0;
	virtual bool OnReleaseIBufferDate(RHIResourceIdentifier* pID) = 0;

protected:


};

#define B_CHECK_RHI_SYSTEM(ptr) if (ptr) { return false; }
#define V_CHECK_RHI_SYSTEM(ptr) if (ptr) { return ; }

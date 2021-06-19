
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
	virtual bool OnReleaseVShaderProgram(RHIVSShader* pVShaderProgram, RHIResourceIdentifier*& pID) = 0;

	virtual bool OnLoadPShaderProgram(RHIPSShader* pVShaderProgram, RHIResourceIdentifier*& pID) = 0;
	virtual bool OnReleasePShaderProgram(RHIPSShader* pVShaderProgram, RHIResourceIdentifier*& pID) = 0;

protected:


};
#include "RHID3D12.h"

void RHID3D12::CreateVShaderFromCache(RHIVSShader* pVShaderProgram, RHIShaderID* pID)
{

}

void RHID3D12::CreateVShaderFromString(RHIVSShader* pVShaderProgram, RHIShaderID* pID)
{
	OnLoadVShaderFromString(pVShaderProgram, pID);
}

void RHID3D12::OnLoadVShaderFromString(RHIVSShader* pVShaderProgram, RHIShaderID* pID)
{

}

void RHID3D12::CreatePShaderFromCache(RHIPSShader* pPShaderProgram, RHIShaderID* pID)
{

}

void RHID3D12::CreatePShaderFromString(RHIPSShader* pPShaderProgram, RHIShaderID* pID)
{
	OnLoadPShaderFromString(pPShaderProgram, pID);
}

void RHID3D12::OnLoadPShaderFromString(RHIPSShader* pPShaderProgram, RHIShaderID* pID)
{

}

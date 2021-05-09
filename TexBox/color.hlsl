//***************************************************************************************
// color.hlsl by Frank Luna (C) 2015 All Rights Reserved.
//
// Transforms and colors geometry.
//***************************************************************************************

Texture2D    gDiffuseMap : register(t0);

SamplerState gsamPointWrap        : register(s0);
SamplerState gsamPointClamp       : register(s1);
SamplerState gsamLinearWrap       : register(s2);
SamplerState gsamLinearClamp      : register(s3);
SamplerState gsamAnisotropicWrap  : register(s4);
SamplerState gsamAnisotropicClamp : register(s5);

cbuffer cbPerObject : register(b0)
{
	float4x4 gView;
	float4x4 gProj;
};

struct VertexIn
{
	float3 PosL  : POSITION;
    float2 TexC  : TEXCOORD;
};

struct VertexOut
{
	float4 PosH  : SV_POSITION;
	float2 TexC  : TEXCOORD;
};

VertexOut VS(VertexIn vin)
{
	VertexOut vout;
	
	// Transform to homogeneous clip space.
	vout.PosH = mul(mul(float4(vin.PosL, 1.0f), gView), gProj);
	//vout.PosH = mul(gProj, mul(gView, float4(vin.PosL, 1.0f)));
	
	// Just pass vertex color into the pixel shader.
    vout.TexC = vin.TexC;
    
    return vout;
}

float4 PS(VertexOut pin) : SV_Target
{
	return gDiffuseMap.Sample(gsamAnisotropicWrap, pin.TexC);
    //return float4(0.5, 0.5, 0.5, 1.0);
}



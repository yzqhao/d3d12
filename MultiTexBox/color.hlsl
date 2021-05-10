//***************************************************************************************
// color.hlsl by Frank Luna (C) 2015 All Rights Reserved.
//
// Transforms and colors geometry.
//***************************************************************************************

Texture2D    gDiffuseMap : register(t0);
Texture2D    gDiffuseMap2 : register(t1);

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
	float gTotalTime;
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
	
	vin.TexC -= float2(0.5, 0.5);
	float sint = sin(gTotalTime);
	float cost = cos(gTotalTime);
	float2x2 rotate = float2x2(sint, cost, -cost, sint);
    vout.TexC = mul(vin.TexC, rotate) + float2(0.5, 0.5);
    
    return vout;
}

float4 PS(VertexOut pin) : SV_Target
{
	float4 tex1 = gDiffuseMap.Sample(gsamAnisotropicWrap, pin.TexC);
	float4 tex2 = gDiffuseMap2.Sample(gsamAnisotropicWrap, pin.TexC);
	return tex1 * tex2;
}



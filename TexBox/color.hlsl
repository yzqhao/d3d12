//***************************************************************************************
// color.hlsl by Frank Luna (C) 2015 All Rights Reserved.
//
// Transforms and colors geometry.
//***************************************************************************************

cbuffer cbPerObject : register(b0)
{
	float4x4 gView;
	float4x4 gProj;
};

struct VertexIn
{
	float3 PosL  : POSITION;
	float2 TexC    : TEXCOORD;
};

struct VertexOut
{
	float4 PosH  : SV_POSITION;
	float2 TexC    : TEXCOORD;
};

Texture2D    gDiffuseMap : register(t0);
SamplerState gsamLinear  : register(s0);

VertexOut VS(VertexIn vin)
{
	VertexOut vout;
	
	// Transform to homogeneous clip space.
	vout.PosH = mul(mul(float4(vin.PosL, 1.0f), gView), gProj);
	//vout.PosH = mul(gProj, mul(gView, float4(vin.PosL, 1.0f)));
	
	vout.TexC = vin.TexC;
    
    return vout;
}

float4 PS(VertexOut pin) : SV_Target
{
    return gDiffuseMap.Sample(gsamLinear, pin.TexC);
}



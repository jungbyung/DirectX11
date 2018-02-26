#include "Light.fx"

cbuffer cbPerObject
{
	float4x4 gWorldViewProj;
	Material gMaterial;
};

Texture2D gDiffuseMap;

SamplerState samLinear
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = WRAP;
	AddressV = WRAP;
};

struct VertexIn
{
	float3 Pos : POSITION;
	float3 Normal : NORMAL;
	float2 Tex : TEXCOORD;
};

struct VertexOut
{
	float4 Pos : SV_POSITION;
	float3 Normal : NORMAL;
	float2 Tex : TEXCOORD;
};

VertexOut VS(VertexIn vin)
{
	VertexOut vout;
	vout.Pos = mul(float4(vin.Pos, 1.0f), gWorldViewProj);
	vout.Normal = float3(0, 0, 0);
	vout.Tex = vin.Tex;

	return vout;
}
float4 PS(VertexOut pin) : SV_Target
{
	float4 color = gDiffuseMap.Sample(samLinear, pin.Tex);
	//color = float4(1, 1, 1, 1);
	return color;
}
technique11 BasicTech
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, PS()));
	}
}
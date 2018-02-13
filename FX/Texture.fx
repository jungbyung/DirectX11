cbuffer cbPerObject
{
	float4x4 gWorldViewProj;
	float4x4 gTexTransform;
};
Texture2D gDiffuseMap;

struct VertexIn
{
	float3 Pos : POSITION;
	float2 Tex : TEXCOORD;
};

struct VertexOut
{
	float4 Pos : POSITION;
	float2 Tex : TEXCOORD;
};

SamplerState samLinear
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = WRAP;
	AddressV = WRAP;
};

VertexOut VS(VertexIn vin)
{
	VertexOut vout;

	vout.Pos = mul(float4(vin.Pos, 1.0f), gWorldViewProj);
	//vout.Tex = mul(float4(vin.Tex, 0.0f, 1.0f), gTexTransform).xy;
	vout.Tex = float2(0, 0);
	return vout;
}

float4 PS(VertexOut pin) : SV_Target
{
	float4 color = gDiffuseMap.Sample(samLinear, pin.Tex);
	//color = float4(1, 1, 1, 1);
	return color;
}

technique11 TextureTech
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, PS()));
	}
}
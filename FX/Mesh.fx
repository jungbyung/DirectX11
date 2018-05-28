#include "Light.fx"

cbuffer dbPerFrame
{
	DirectionalLight gDirLights[3];
	float3 gEyePosW;
};

cbuffer cbPerObject
{
	float4x4 gWorld;
	float4x4 gWorldInvTranspose;
	float4x4 gWorldViewProj;
	float4x4 gTexTransform;
	Material gMaterial;
};

cbuffer cbSkinned
{
	float4x4 gBoneTransforms[148];
};

Texture2D gDiffuseMap;
Texture2D gNormalMap;

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
	float4 Tangent : TAANGENT;
};

struct SkinnedVertexIn
{
	float3 Pos : POSITION;
	float3 Normal : NORMAL;
	float2 Tex : TEXCOORD;
	float4 Tangent : TANGENT;
	float3 Weights : WEIGHTS;
	uint4 BoneIndices : BONEINDICES;
};

struct VertexOut
{
	float4 PosH : SV_POSITION;
	float3 PosW : POSITION;
	float3 Normal : NORMAL;
	float4 Tangent : TANGENT;
	float2 Tex : TEXCOORD0;
};

VertexOut aVS(VertexIn vin)
{
	VertexOut vout;

	vout.PosW = mul(float4(vin.Pos, 1.0f), gWorld).xyz;
	vout.Normal = mul(vin.Normal, (float3x3)gWorldInvTranspose);
	vout.Tangent = mul(vin.Tangent, gWorld);

	vout.PosH = mul(float4(vin.Pos, 1.0f), gWorldViewProj);
	vout.Tex = vin.Tex;

	return vout;
}

VertexOut SkinnedVS(SkinnedVertexIn vin)
{
	VertexOut vout;

	float weights[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	weights[0] = vin.Weights.x;
	weights[1] = vin.Weights.y;
	weights[2] = vin.Weights.z;
	weights[3] = 1.0f - weights[0] - weights[1] - weights[2];

	float3 pos = float3(0.0f, 0.0f, 0.0f);
	float3 normal = float3(0.0f, 0.0f, 0.0f);
	float3 tangent = float3(0.0f, 0.0f, 0.0f);
	for (int i = 0; i < 4; ++i)
	{
		pos += weights[i] * mul(float4(vin.Pos, 1.0f), gBoneTransforms[vin.BoneIndices[i]]).xyz;
		normal += weights[i] * mul(vin.Normal, (float3x3)gBoneTransforms[vin.BoneIndices[i]]);
		tangent += weights[i] * mul(vin.Tangent.xyz, (float3x3) gBoneTransforms[vin.BoneIndices[i]]);
	}

	vout.PosW = mul(float4(pos, 1.0f), gWorld).xyz;
	vout.Normal = mul(normal, (float3x3)gWorldInvTranspose);
	vout.Tangent = float4(mul(tangent, (float3x3)gWorld), vin.Tangent.w);

	vout.PosH = mul(float4(pos, 1.0f), gWorldViewProj);
	vout.Tex = mul(float4(vin.Tex, 0.0f, 1.0f), gTexTransform).xy;
	//vout.Tex = vin.Tex;
	//vout.PosW = mul(float4(vin.Pos, 1.0f), gWorld).xyz;
	//vout.Normal = mul(vin.Normal, (float3x3)gWorldInvTranspose);
	//vout.Tangent = mul(vin.Tangent, gWorld);
	//
	//vout.PosH = mul(float4(vin.Pos, 1.0f), gWorldViewProj);
	//vout.Tex = vin.Tex;
	return vout;
}

float4 PS(VertexOut pin, uniform int gLightCount) : SV_Target
{
	pin.Normal = normalize(pin.Normal);

	float3 toEye = gEyePosW - pin.PosW;
	float distToEye = length(toEye);

	toEye /= distToEye;

	float4 texColor = gDiffuseMap.Sample(samLinear, pin.Tex);

	float3 normalMapSample = gNormalMap.Sample(samLinear, pin.Tex).rgb;
	float3 bumpedNormal = NormalSampleToWorldSpace(normalMapSample, pin.Normal, pin.Tangent);

	float4 litColor = texColor;// *float4(bumpedNormal, 1.0f);
	if (gLightCount > 0)
	{
		float4 ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
		float4 diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
		float4 spec = float4(0.0f, 0.0f, 0.0f, 0.0f);

		[unroll]
		for (int i = 0; i < gLightCount; ++i)
		{
			float4 A, D, S;
			ComputeDirectionalLight(gMaterial, gDirLights[i], pin.Normal, toEye, A, D, S);
			ambient += A;
			diffuse += D;
			spec += S;
		}

		//ambient = gMaterial.Ambient;
		//diffuse = gMaterial.Diffuse;
		//spec = gMaterial.Specular;
		litColor = texColor * (ambient + diffuse) + spec;
	}

	litColor.a = 1.0f;

	return litColor;
}

technique11 Skinned
{
	pass p0
	{
		SetVertexShader(CompileShader(vs_5_0, SkinnedVS()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, PS(1)));
	}
}

technique11 v
{
	pass p0
	{
		SetVertexShader(CompileShader(vs_5_0, aVS()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, PS(1)));
	}
}
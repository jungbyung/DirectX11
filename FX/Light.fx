struct DirectionLight
{
	float4 Ambient;
	float4 Diffuse;
	float4 Specular;
	float3 Direction;
};

struct  PointLight
{
	float4 Ambient;
	float4 Diffuse;
	float4 Specular;

	float3 Position;
	float Range;

	float3 Att;
};

struct Material
{
	float4 Ambient;
	float4 Diffuse;
	float4 Specular;
	float4 Reflact;
};

void ComputeDirectionLight(Material mat, DirectionLight L, float3 normal, float3 toEye,
	out float4 ambient, out float4 diffuse, out float4 spec)
{
	ambient = float4(.0f, .0f, .0f, .0f);
	diffuse = float4(.0f, .0f, .0f, .0f);
	spec = float4(.0f, .0f, .0f, .0f);

	float3 lightVec = -L.Direction;

	ambient = mat.Ambient * L.Ambient;

	float diffuseFactor = dot(lightVec, normal);

	[flatten]
	if (diffuseFactor > 0.0f)
	{
		float3 v = reflect(-lightVec, normal);
		float specFactor = pow(max(dot(v, toEye), 0.0f), mat.Specular.w);

		diffuse = diffuseFactor * mat.Diffuse * L.Diffuse;
		spec = specFactor * mat.Specular * L.Specular;
	}
}
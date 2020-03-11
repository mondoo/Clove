#define MAX_LIGHTS 10

struct PointLight{
	float3 position;
	float constant;
	float3 ambient;
	float linearV;
	float3 diffuse;
	float quadratic;
	float3 specular;
	float farplane;
};
cbuffer PointLightBuffer : register(b2){
	PointLight lights[MAX_LIGHTS];
};

cbuffer lightNumBuffer : register(b10){
	int currentLightIndex;
}

float main(float4 vertPosition : VertPos) : SV_Depth{
	const float3 lightPosition = lights[currentLightIndex].position;
	const float farplane = lights[currentLightIndex].farplane;
	const float lightDistance = length(vertPosition.xyz - lightPosition.xyz) / farplane;
	return lightDistance;
}

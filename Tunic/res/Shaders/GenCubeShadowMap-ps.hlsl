#define MAX_LIGHTS 10

struct DirectionalLightData{
	float3 direction;

	float3 ambient;
	float3 diffuse;
	float3 specular;
};
struct PointLightData{
	float3 position;
	
	float constant;
	float3 ambient;
	float linearV;
	float3 diffuse;
	float quadratic;
	float3 specular;
	
	float farplane;
};
cbuffer LightBuffer : register(b2){
	DirectionalLightData directionalLights[MAX_LIGHTS];
	PointLightData pointLights[MAX_LIGHTS];
};

cbuffer lightNumBuffer : register(b10){
	int currentLightIndex;
}

float main(float4 vertPosition : VertPos) : SV_Depth{
	const float3 lightPosition = pointLights[currentLightIndex].position;
	const float farplane = pointLights[currentLightIndex].farplane;
	const float lightDistance = length(vertPosition.xyz - lightPosition.xyz) / farplane;
	return lightDistance;
}

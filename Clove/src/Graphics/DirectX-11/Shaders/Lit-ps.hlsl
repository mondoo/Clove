Texture2D albedoTexture : register(t1);
SamplerState albedoSampler : register(s1);

Texture2D specularTexture : register(t2);
SamplerState specularSampler : register(s2);

TextureCube shadowDepthMap : register(t3);
SamplerState shadowDepthSampler : register(s3);

struct PointLight{
	float3 position;
	float constant;
	float3 ambient;
	float linearV;
	float3 diffuse;
	float quadratic;
	float3 specular;

};
cbuffer PointLightBuffer : register(b1){
	//int numLights;
	//PointLight lights[10]; //Temp 10 max
	PointLight light; //doing one for now
};

cbuffer ViewBuffer : register(b2){
    float3 viewPos;
}

cbuffer MaterialBuffer : register(b4){
    float shininess;
}

//TODO: This will need  to be updated for multiple lights
cbuffer lightPosBuffer : register(b7){
	float3 lightPosition;
	float farplane;
}

float3 calculatePointLight(PointLight light, float3 normal, float3 fragPos, float3 viewDirection, float2 texCoord);

float shadowCalculation(float3 fragPos);

float4 main(float2 texCoord : TexCoord, float3 vertPos : VertPos, float3 vertNormal : VertNormal) : SV_Target{
    float3 normal       = normalize(vertNormal);
    float3 viewDir      = normalize(viewPos - vertPos);
    
	float3 lighting = float3(0.0f, 0.0f, 0.0f);
	//for(int i = 0; i < numLights; ++i){
	//	lighting += calculatePointLight(lights[i], normal, vertPos, viewDir, texCoord);
	//}

	lighting += calculatePointLight(light, normal, vertPos, viewDir, texCoord);

    return float4(lighting, 1.0f);
}

float3 calculatePointLight(PointLight light, float3 normal, float3 fragPos, float3 viewDirection, float2 texCoord){
	float3 lightDir = normalize(light.position - fragPos);

	//Ambient
	float3 ambient = light.ambient * (float3)albedoTexture.Sample(albedoSampler, texCoord);

	//Diffuse
	float3 diff = max(dot(normal, lightDir), 0.0f);
	float3 diffuse = light.diffuse * diff * (float3)albedoTexture.Sample(albedoSampler, texCoord);

	//Specular
	float3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDirection, reflectDir), 0.0f), shininess);
	float3 specular = light.specular * spec * (float3)specularTexture.Sample(specularSampler, texCoord);

	//Attenuation
	float distance = length(light.position - fragPos);
	float attenuation = 1.0f / (light.constant + (light.linearV * distance) + (light.quadratic * (distance * distance)));

	ambient		*= attenuation;
	diffuse		*= attenuation;
	specular	*= attenuation;

	//Shadow
	float shadow = shadowCalculation(fragPos);

	return (ambient + (1.0f - shadow) * (diffuse + specular));
}

float shadowCalculation(float3 fragPos){
	float3 fragToLight = fragPos - lightPosition;
	float currentDepth = length(fragToLight);

	float closestDepth = shadowDepthMap.Sample(shadowDepthSampler, fragToLight).r;
	closestDepth *= farplane;

	float bias = 0.05;
	float shadow = currentDepth - bias > closestDepth ? 1.0 : 0.0;

	return shadow;
}
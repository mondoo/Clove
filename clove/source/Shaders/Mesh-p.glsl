#version 450

#include "Constants.glsl"

layout(set = SET_MESH, binding = 0) uniform sampler2D texSampler;
layout(set = SET_LIGHTING, binding = 3) uniform sampler2D directionalDepthSampler[MAX_LIGHTS];
layout(set = SET_LIGHTING, binding = 4) uniform samplerCube pointLightDepthSampler[MAX_LIGHTS];

layout(set = SET_VIEW, binding = 1) uniform ViewPosition{
	vec3 viewPos;
};

struct DirectionalLightData{
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};
struct PointLightData{
	vec3 position;
	
	float constant;
	vec3 ambient;
	float linearV;
	vec3 diffuse;
	float quadratic;
	vec3 specular;
	
	float farplane;
};
layout(std140, set = SET_LIGHTING, binding = 0) uniform Lights{
	DirectionalLightData directionalLights[MAX_LIGHTS];
	PointLightData pointLights[MAX_LIGHTS];
};
layout(std140, set = SET_LIGHTING, binding = 1) uniform NumLights{
	int numDirLights;
	int numPointLights;
};

layout(std140, set = SET_MESH, binding = 3) uniform Colour{
	vec4 colour;
};

layout(location = 0) in vec3 fragColour;
layout(location = 1) in vec2 fragTexCoord;
layout(location = 2) in vec3 fragPos;
layout(location = 3) in vec3 fragNorm;
layout(location = 4) in vec4 fragPosLightSpaces[MAX_LIGHTS];

layout(location = 0) out vec4 outColour;

//Adjusts the bias to be in between min and max based off of the angle to the light
float adjustBias(float minBias, float maxBias, vec3 normal, vec3 lightDir){
	return max(maxBias * (1.0f - dot(normal, lightDir)), minBias);
}

void main(){
	vec3 texelColour = vec3(texture(texSampler, fragTexCoord));

	vec3 viewDir = normalize(viewPos - fragPos);

	vec3 normal = normalize(fragNorm);

	vec3 totalAmbient = vec3(0);
	vec3 totalDiffuse = vec3(0);
	vec3 totalSpecular = vec3(0);

	const float shiniess =  32.0f; //TODO: Add shiniess as a material param

	const float minBias = 0.0005f;
	const float maxBias = 0.05f;

	float shadow = 0.0f;

	//Directional lighting
	for(int i = 0; i < numDirLights; ++i){
		const vec3 lightDir = normalize(-directionalLights[i].direction); //Gets the direction towards the light
		
		//Ambient
		totalAmbient += directionalLights[i].ambient;
	
		//Diffuse
		const float diffIntensity = max(dot(normal, lightDir), 0.0f);
		totalDiffuse += directionalLights[i].diffuse * diffIntensity;

		//Specular
		const vec3 reflectDir = reflect(-lightDir, normal);
		const float specIntensity = pow(max(dot(viewDir, reflectDir), 0.0f), shiniess);
		totalSpecular += directionalLights[i].specular * specIntensity;

		//Shadow
		vec3 projCoords = fragPosLightSpaces[i].xyz / fragPosLightSpaces[i].w;
		projCoords.xy = projCoords.xy * 0.5f + 0.5f;

		const float currentDepth = projCoords.z;
		const float closetDepth = texture(directionalDepthSampler[i], projCoords.xy).r;

		shadow += currentDepth - adjustBias(minBias, maxBias, normal, lightDir) > closetDepth ? 1.0f : 0.0f;
	}

	//Point lighting
	for(int i = 0; i < numPointLights; ++i){
		const vec3 lightDir = normalize(pointLights[i].position - fragPos);

		//Ambient
		vec3 ambient = pointLights[i].ambient;

		//Diffuse
		const float diffIntensity = max(dot(normal, lightDir), 0.0f);
		vec3 diffuse = pointLights[i].diffuse * diffIntensity;

		//Specular
		const vec3 reflectDir = reflect(-lightDir, normal);
		const float specIntensity = pow(max(dot(viewDir, reflectDir), 0.0f), shiniess);
		vec3 specular = pointLights[i].specular * specIntensity;

		//Attenuation
		float dist = length(pointLights[i].position - fragPos);
		float attenuation = 1.0f / (pointLights[i].constant + (pointLights[i].linearV * dist) + (pointLights[i].quadratic * (dist * dist)));

		ambient		*= attenuation;
		diffuse		*= attenuation;
		specular	*= attenuation;

		totalAmbient += ambient;
		totalDiffuse += diffuse;
		totalSpecular += specular;

		//Shadow
		const vec3 lightToFrag = fragPos - pointLights[i].position;

		const float currentDepth = length(lightToFrag);
		const float closetDepth = texture(pointLightDepthSampler[i], lightToFrag).r * pointLights[i].farplane;

		shadow += currentDepth - adjustBias(minBias, maxBias, normal, lightDir) > closetDepth ? 1.0f : 0.0f;
	}

	shadow /= (numDirLights + numPointLights);
	const vec3 lighting = (totalAmbient + ((1.0f - shadow) * (totalDiffuse + totalSpecular))) * texelColour;

	outColour = vec4(lighting, 1.0) * colour;
}
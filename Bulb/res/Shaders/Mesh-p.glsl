#version 450

#include "Constants.glsl"

layout(set = SET_MATERIAL, binding = 0) uniform sampler2D texSampler;
layout(set = SET_LIGHTING, binding = 3) uniform sampler2D directionalDepthSampler[MAX_LIGHTS];

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

layout(location = 0) in vec3 fragColour;
layout(location = 1) in vec2 fragTexCoord;
layout(location = 2) in vec3 vertPos;
layout(location = 3) in vec3 vertNorm;
layout(location = 4) in vec4 vertPosLightSpaces[MAX_LIGHTS];

layout(location = 0) out vec4 outColour;

void main(){
	vec3 colour = vec3(texture(texSampler, fragTexCoord));

	vec3 viewDir = normalize(viewPos - vertPos);

	vec3 normal = normalize(vertNorm);

	vec3 totalAmbient = vec3(0);
	vec3 totalDiffuse = vec3(0);
	vec3 totalSpecular = vec3(0);

	const float shiniess =  32.0f; //TODO: Add shiniess as a material param

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
	}

	//Point lighting
	for(int i = 0; i < numPointLights; ++i){
		const vec3 lightDir = normalize(pointLights[i].position - vertPos);

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
		float dist = length(pointLights[i].position - vertPos);
		float attenuation = 1.0f / (pointLights[i].constant + (pointLights[i].linearV * dist) + (pointLights[i].quadratic * (dist * dist)));

		ambient		*= attenuation;
		diffuse		*= attenuation;
		specular	*= attenuation;

		totalAmbient += ambient;
		totalDiffuse += diffuse;
		totalSpecular += specular;
	}

	vec3 lighting = vec3(0, 0, 0);

	//Calculate shadow: Directional
	for(int i = 0; i < numDirLights; ++i){
		vec3 projCoords = vertPosLightSpaces[i].xyz / vertPosLightSpaces[i].w;
		projCoords.xy = projCoords.xy * 0.5f + 0.5f;

		const float currentDepth = projCoords.z;
		const float closetDepth = texture(directionalDepthSampler[i], projCoords.xy).r;

		const float bias = 0.005f;
		const float shadow = currentDepth - bias > closetDepth ? 1.0f : 0.0f;

		lighting += (totalAmbient + ((1.0f - shadow) * (totalDiffuse + totalSpecular)) * colour);
	}
	outColour = vec4(lighting, 1.0);
}
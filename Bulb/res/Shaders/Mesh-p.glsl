#version 450

#include "Constants.glsl"

layout(set = SET_MATERIAL, binding = 0) uniform sampler2D texSampler;

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
		totalAmbient += directionalLights[i].ambient * colour;
	
		//Diffuse
		const float diffIntensity = max(dot(normal, lightDir), 0.0f);
		totalDiffuse += directionalLights[i].diffuse * colour * diffIntensity;

		//Specular
		const vec3 reflectDir = reflect(-lightDir, normal);
		const float specIntensity = pow(max(dot(viewDir, reflectDir), 0.0f), shiniess);
		totalSpecular += directionalLights[i].specular * colour * specIntensity;
	}

	//Point lighting
	for(int i = 0; i < numPointLights; ++i){
		const vec3 lightDir = normalize(pointLights[i].position - vertPos);

		//Ambient
		vec3 ambient = pointLights[i].ambient * colour;

		//Diffuse
		const float diffIntensity = max(dot(normal, lightDir), 0.0f);
		vec3 diffuse = pointLights[i].diffuse * diffIntensity * colour;

		//Specular
		const vec3 reflectDir = reflect(-lightDir, normal);
		const float specIntensity = pow(max(dot(viewDir, reflectDir), 0.0f), shiniess);
		vec3 specular = pointLights[i].specular * specIntensity * colour;

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

	outColour = vec4(totalAmbient + totalDiffuse + totalSpecular, 1.0);
}
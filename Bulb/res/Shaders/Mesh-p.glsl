#version 450

#define MAX_LIGHTS 10

layout(set = 0, binding = 1) uniform sampler2D texSampler;

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
layout(std140, set = 0, binding = 2) uniform Lights{
	DirectionalLightData directionalLights[MAX_LIGHTS];
	PointLightData pointLights[MAX_LIGHTS]; //Unused: Just keeping here for padding for now
} lights;

layout(std140, set = 0, binding = 3) uniform NumLights{
	int numDirLights;
	int numPointLights; //Unused: Just keeping here for padding for now
} numLights;

layout(location = 0) in vec3 fragColour;
layout(location = 1) in vec2 fragTexCoord;
layout(location = 2) in vec3 vertPos;
layout(location = 3) in vec3 vertNorm;

layout(location = 0) out vec4 outColour;

void main(){
	vec3 colour = vec3(texture(texSampler, fragTexCoord));

	vec3 normal = normalize(vertNorm);
	vec3 lightDir = normalize(-lights.directionalLights[0].direction);

	//TODO: Assuming there's always one light for now - will need updating to a loop

	//Ambient
	vec3 ambient = lights.directionalLights[0].ambient * colour;
	
	//Diffuse
	float diffIntensity = max(dot(normal, lightDir), 0.0);
	vec3 diffuse = lights.directionalLights[0].diffuse * colour * diffIntensity;

	//Specular
	//TODO

	outColour = vec4(ambient + diffuse, 1.0);
}
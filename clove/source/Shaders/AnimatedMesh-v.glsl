#version 450

#include "Constants.glsl"

layout(std140, set = SET_MESH, binding = 2) uniform Model{
	mat4 model;
	mat4 normalMatrix;
};

layout(std140, set = SET_MESH, binding = 3) uniform Skeleton{
	mat4 matrixPalet[MAX_JOINTS];
};

layout(std140, set = SET_VIEW, binding = 0) uniform ViewProj{
	mat4 view;
	mat4 proj;
};

layout(std140, set = SET_LIGHTING, binding = 1) uniform NumLights{
	int numDirLights;
	int numPointLights;
};
layout(std140, set = SET_LIGHTING, binding = 2) uniform LightMatrix{
	mat4 lightSpaceMatrices[MAX_LIGHTS];
};

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoord;
layout(location = 3) in vec3 colour;
layout(location = 4) in ivec4 jointIds;
layout(location = 5) in vec4 weights;

layout(location = 0) out vec3 fragColour;
layout(location = 1) out vec2 fragTexCoord;
layout(location = 2) out vec3 vertPos;
layout(location = 3) out vec3 vertNorm;
layout(location = 4) out vec4 vertPosLightSpaces[MAX_LIGHTS];

void main(){
	const mat4 mvp = proj * view * model;
    
    vec4 animatedPos = vec4(0.0f);
    vec4 animatedNormal = vec4(0.0f);
    
    for(int i = 0; i < 4; ++i){ //4 is the max amount of weights
        const mat4 jointTransform = matrixPalet[jointIds[i]];
        const float weight = weights[i];
        
        animatedPos += (jointTransform * vec4(position, 1.0f)) * weight;
        animatedNormal += (jointTransform * vec4(normal, 0.0f)) * weight;
    }

	gl_Position = mvp * animatedPos;
	
	fragColour = colour;
	fragTexCoord = texCoord;

	vertPos = vec3(model * animatedPos);
	vertNorm = vec3(normalMatrix * animatedNormal);

	for(int i = 0; i < numDirLights; ++i){
		vertPosLightSpaces[i] = lightSpaceMatrices[i] * vec4(vertPos, 1.0f);
	}
}
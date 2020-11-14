#version 450

#include "Constants.glsl"

layout(std140, set = SET_MESH, binding = 1) uniform Model{
	mat4 model;
	mat4 normalMatrix;
};

layout(std140, set = SET_MESH, binding = 2) uniform Skeleton{
	mat4 matrixPalet[MAX_JOINTS];
};

layout(std140, push_constant) uniform LightSpace{
	mat4 lightSpaceMatrix;
};

layout(location = 0) in vec3 position;
layout(location = 4) in ivec4 jointIds;
layout(location = 5) in vec4 weights;

void main(){
    const mat4 mvp = lightSpaceMatrix * model;
    
    vec4 animatedPos = vec4(0.0f);
    
    for(int i = 0; i < 4; ++i){ //4 is the max amount of weights
        const mat4 jointTransform = matrixPalet[jointIds[i]];
        const float weight = weights[i];
        
        animatedPos += (jointTransform * vec4(position, 1.0f)) * weight;
    }

	gl_Position = mvp * animatedPos;
}
#version 450

#include "Constants.glsl"

layout(std140, push_constant) uniform Model{
	mat4 model;
	mat4 lightSpaceMatrix;
};

layout(std140, set = SET_MESH, binding = 1) uniform Skeleton{
	mat4 matrixPalet[MAX_JOINTS];
};

layout(location = 0) in vec3 position;
layout(location = 4) in ivec4 jointIds;
layout(location = 5) in vec4 weights;

layout(location = 0) out vec4 vertPos;

void main(){
    vec4 animatedPos = vec4(0.0f);
    
    for(int i = 0; i < 4; ++i){ //4 is the max amount of weights
        const mat4 jointTransform = matrixPalet[jointIds[i]];
        const float weight = weights[i];
        
        animatedPos += (jointTransform * vec4(position, 1.0f)) * weight;
    }

    vertPos = model * animatedPos;
    gl_Position = lightSpaceMatrix * vertPos;
}
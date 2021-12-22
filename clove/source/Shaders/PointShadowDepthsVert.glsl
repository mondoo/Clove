#version 450

#include "Constants.glsl"

layout(std140, set = 0, binding = 0) uniform Model{
	mat4 model;
	mat4 normalMatrix;
};

//TODO: Push constant
layout(std140, set = 0, binding = 1) uniform LightSpace{
	mat4 lightSpaceMatrix;
};

layout(location = 0) in vec3 position;

layout(location = 0) out vec4 vertPos;

void main(){
    vertPos = model * vec4(position, 1.0f);
    gl_Position = lightSpaceMatrix * vertPos;
}
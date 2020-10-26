#version 450

#include "Constants.glsl"

layout(std140, push_constant) uniform Model{
	mat4 model;
	mat4 lightSpaceMatrix;
};

layout(location = 0) in vec3 position;

void main(){
    gl_Position = lightSpaceMatrix * model * vec4(position, 1.0f);
}
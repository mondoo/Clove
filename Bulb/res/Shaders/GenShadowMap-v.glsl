#version 450

#include "Constants.glsl"

//TODO: Putting the light space as a push constant but may be better as a buffer
//because this shader gets called per mesh
layout(std140, push_constant) uniform Model{
	mat4 model;
	mat4 lightSpaceMatrix;
};

layout(location = 0) in vec3 position;

void main(){
    gl_Position = lightSpaceMatrix * model * vec4(position, 1.0f);
}
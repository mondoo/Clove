#version 450

layout(std140, push_constant) uniform Model{
	mat4 model;
	mat4 lightSpaceMatrix;
};

layout(location = 0) in vec3 position;

layout(location = 0) out vec4 vertPos;

void main(){
    vertPos = model * vec4(position, 1.0f);
    gl_Position = lightSpaceMatrix * vertPos;
}
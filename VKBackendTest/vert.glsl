#version 450

layout(binding = 0) uniform ModelViewProj{
	mat4 model;
	mat4 view;
	mat4 proj;
} mvp;

layout(location = 0) in vec2 position;
layout(location = 1) in vec3 colour;
layout(location = 2) in vec2 texCoord;

layout(location = 0) out vec3 fragColour;
layout(location = 1) out vec2 fragTexCoord;

void main(){
	gl_Position = mvp.proj * mvp.view * mvp.model * vec4(position, 0.0f, 1.0f);
	fragColour = colour;
	fragTexCoord = texCoord;
}
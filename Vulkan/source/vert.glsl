#version 450
#extension GL_KHR_vulkan_glsl : enable

layout(location = 0) in vec2 vertPos;
layout(location = 1) in vec3 vertColour;

layout(location = 0) out vec3 fragColour;

void main(){
	gl_Position = vec4(vertPos, 0.0f, 1.0f);
	fragColour = vertColour;
}
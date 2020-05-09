#version 450

//We can also define which descriptor set we sample from
layout(set = 0, binding = 0) uniform UniformBufferObject{
	mat4 model;
	mat4 view;
	mat4 proj;
} ubo;

layout(location = 0) in vec2 vertPos;
layout(location = 1) in vec3 vertColour;

layout(location = 0) out vec3 fragColour;

void main(){
	gl_Position = ubo.proj * ubo.view * ubo.model * vec4(vertPos, 0.0f, 1.0f);
	fragColour = vertColour;
}
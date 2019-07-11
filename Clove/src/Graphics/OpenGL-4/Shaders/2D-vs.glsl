#version 460 core

layout(location = 0) in vec2 position;
layout(location = 1) in vec2 texCoord;

layout(std140, binding = 5) uniform modelBuffer {
	mat4 modelProjection;
};

out vec2 vertTexCoord;

void main(){
	gl_Position = modelProjection * vec4(position, 0.0f, 1.0f);

	vertTexCoord = texCoord;
}
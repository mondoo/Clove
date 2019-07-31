#version 460 core

in vec2 vertTexCoord;

layout(binding = 4) uniform sampler2D frameBufferSampler;

layout(location = 0) out vec4 fragmentColour;

void main(){
	fragmentColour = texture(frameBufferSampler, vertTexCoord);
}
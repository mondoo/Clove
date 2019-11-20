#version 460 core

in vec2 vertTexCoord;

layout(binding = 1) uniform sampler2D albedoSampler;

layout(location = 0) out vec4 fragmentColour;

void main(){
	fragmentColour = vec4(1.0, 1.0, 1.0, texture(albedoSampler, vertTexCoord).r);
}

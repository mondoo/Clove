#version 460 core

in vec2 vertTexCoord;

layout(binding = 3) uniform sampler2D spriteSampler;

layout(location = 0) out vec4 fragmentColour;

void main(){
	fragmentColour = vec4(texture(spriteSampler, vertTexCoord));
}

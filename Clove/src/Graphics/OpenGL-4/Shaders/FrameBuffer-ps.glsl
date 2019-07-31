#version 460 core

in vec2 vertTexCoord;

layout(binding = 4) uniform sampler2D frameBufferSampler;

layout(location = 0) out vec4 fragmentColour;

void main(){
	//fragmentColour = texture(frameBufferSampler, vertTexCoord);
	//fragmentColour = vec4(vec3(1.0 - texture(frameBufferSampler, vertTexCoord)), 1.0);

	fragmentColour = texture(frameBufferSampler, vertTexCoord);
    float average = 0.2126 * fragmentColour.r + 0.7152 * fragmentColour.g + 0.0722 * fragmentColour.b;
    fragmentColour = vec4(average, average, average, 1.0);
}
#version 330 core

in vec2 vertTexCoord;
in vec3 vertPos;
in vec3 vertNormal;

layout(location = 0) out vec4 fragmentColour;

void main(){
	fragmentColour = vec4(1.0, 1.0, 1.0, 1.0);
};
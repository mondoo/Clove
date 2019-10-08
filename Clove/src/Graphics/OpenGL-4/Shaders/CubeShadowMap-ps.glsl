#version 460 core

in vec4 fragPos;

layout(std140, binding = 7) uniform lightPosBuffer {
	vec3 lightPosition;
	float farPlane;
};

void main(){
	const float lightDistance = (length(fragPos.xyz - lightPosition)) / farPlane;
	gl_FragDepth = lightDistance;
}
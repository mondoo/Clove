#version 450
#extension GL_ARB_sepearate_shader_objects : enable

//Doesn't need the same name, just the same layout location as the vert 'out'
layout(location = 0) in vec3 fragColour;

layout(location = 0) out vec4 outColour;

void main(){
	outColour = vec4(fragColour, 1.0f);
}
#version 450
#extension GL_ARB_sepearate_shader_objects : enable

layout(set = 0, binding = 1) uniform sampler2D texSampler;

//Doesn't need the same name, just the same layout location as the vert 'out'
//layout(location = 0) in vec3 fragColour;
layout(location = 0) in vec2 fragTexCoord;

layout(location = 0) out vec4 outColour;

void main(){
	outColour = texture(texSampler, fragTexCoord);
}
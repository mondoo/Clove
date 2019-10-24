#version 460 core

layout(triangles) in;
layout(triangle_strip, max_vertices=18) out; //out putting 6 triangles (one for each face)

layout(std140, binding = 6) uniform shadowMatBuffer {
	mat4 shadowMatrices[6];
};

layout(std140, binding = 9) uniform lightNumBuffer {
	unsigned int currentLightIndex;
};

out vec4 fragPos;

void main(){
	for(int face = 0; face < 6; ++face){
		gl_Layer = face + int(currentLightIndex); //GL built-in, tells opengl which face to put the data on if we attach a cubemap
		for(int i = 0; i < 3; ++i){ //triagles in so loop through each vertex
			fragPos = gl_in[i].gl_Position;
			gl_Position = shadowMatrices[face] * fragPos; //transform the input vertex into the light world space
			EmitVertex();
		}
		EndPrimitive();
	}
}
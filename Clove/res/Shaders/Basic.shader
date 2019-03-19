#shader vertex
#version 330 core

layout(location = 0) in vec4 position; //location 0 here refers to the position in the vertex for where to get attribute from (what specified in glVertexAttribPointer)

void main(){
   gl_Position = position;
};

#shader fragment
#version 330 core

layout(location = 0) out vec4 color; //0 is less important here because this is a out vector and not an in vector

uniform vec4 u_Color;

void main(){
	color = u_Color;
};
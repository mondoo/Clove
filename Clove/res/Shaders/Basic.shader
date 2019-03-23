#shader vertex
#version 330 core
//Runs once per vertex

layout(location = 0) in vec4 position; //location 0 here refers to the position in the vertex for where to get attribute from (what specified in glVertexAttribPointer)
layout(location = 1) in vec2 texCoord;

out vec2 v_TexCoord; //This doesn't use a location because we use it to send data to another shader (the fragment shader in this case)

uniform mat4 u_MVP;

void main(){
   gl_Position = u_MVP * position;
   v_TexCoord = texCoord;
};

#shader fragment
#version 330 core
//Runs once per pixel

layout(location = 0) out vec4 color; //0 is less important here because this is a out vector and not an in vector

in vec2 v_TexCoord; //not using a location here (same reason for the out in the vertex shader)

uniform vec4 u_Color;
uniform sampler2D u_Texture;

void main(){
	vec4 texColour = texture(u_Texture, v_TexCoord);
	color = texColour;
};
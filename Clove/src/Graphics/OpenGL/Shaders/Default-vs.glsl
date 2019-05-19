#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texCoord;
layout(location = 2) in vec3 normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec2 vertTexCoord;
out vec3 vertPos;
out vec3 vertNormal;

void main(){
   gl_Position = projection * view * model * position;
  
   vertTexCoord = texCoord;

   //Convert frag and normal to world space
   vertPos		= vec3(model * position);
   vertNormal	= mat3(transpose(inverse(model))) * normal;
};
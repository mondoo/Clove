#version 460 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texCoord;
layout(location = 2) in vec3 normal;

uniform mat4 model;
uniform mat4 normalMatrix;

layout(std140, binding = 0) uniform CameraMatrices {
	mat4 view;
	mat4 projection;
};

out vec2 vertTexCoord;
out vec3 vertPos;
out vec3 vertNormal;

void main(){
	const vec4 pos4D = vec4(position, 1.0f);

   gl_Position = projection * view * model * pos4D;
  
   vertTexCoord = texCoord;

   //Convert frag and normal to world space
   vertPos		= vec3(model * pos4D);
   vertNormal	= mat3(normalMatrix) * normal;
};
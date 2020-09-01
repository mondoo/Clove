#version 450

layout(binding = 0) uniform ModelViewProj{
	mat4 model;
	mat4 view;
	mat4 proj;
} mvp;

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoord;
layout(location = 3) in vec3 colour;

layout(location = 0) out vec3 fragColour;
layout(location = 1) out vec2 fragTexCoord;
layout(location = 2) out vec3 vertPos;
layout(location = 3) out vec3 vertNorm;

void main(){
	gl_Position = mvp.proj * mvp.view * mvp.model * vec4(position, 1.0f);
	
	fragColour = colour;
	fragTexCoord = texCoord;

	vertPos = vec3(mvp.model * vec4(position, 1.0f));
	vertNorm = mat3(transpose(inverse(mvp.model))) * normal; //TODO: inverse / transpose on CPU
}
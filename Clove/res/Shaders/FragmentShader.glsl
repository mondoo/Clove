#version 330 core

layout(location = 0) out vec4 fragmentColour;

in vec2 vertTexCoord;
in vec3 vertPos;
in vec3 vertNormal;

uniform vec4 objectColour;
uniform vec3 lightColour;

uniform sampler2D textureSample;

uniform float ambientStrength;

uniform vec3 lightPos;
uniform vec3 viewPos;

void main(){
	vec4 texColour = texture(textureSample, vertTexCoord);
	
	//ambient lighting
	vec4 ambientLight = vec4(ambientStrength * lightColour, 1.0);

	//diffuse lighting
	vec3 fragNorm		= normalize(vertNormal);
	vec3 lightDir		= normalize(lightPos - vertPos);
	float diff			= max(dot(fragNorm, lightDir), 0.0);
	vec4 diffuseLight	= vec4(diff * lightColour, 1.0);

	//specular lighting
	float specularStrength	= 0.5;
	vec3 viewDir			= normalize(viewPos - vertPos);
	vec3 reflectDir			= reflect(-lightDir, fragNorm);
	float spec				= pow(max(dot(viewDir, reflectDir), 0.0), 32);
	vec4 specularLight		= vec4(specularStrength * spec * lightColour, 1.0);

	vec4 lightResult = (ambientLight + diffuseLight + specularLight) * objectColour;

	fragmentColour = texColour * lightResult;
};
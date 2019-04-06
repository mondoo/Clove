#version 330 core

in vec2 vertTexCoord;
in vec3 vertPos;
in vec3 vertNormal;

struct Material{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float shininess;
};

struct Light{
	vec3 position;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

uniform Material material;
uniform Light light;

uniform sampler2D textureSample;

uniform vec3 viewPos;

layout(location = 0) out vec4 fragmentColour;

void main(){
	vec4 texColour		= texture(textureSample, vertTexCoord);
	
	vec3 fragNorm		= normalize(vertNormal);
	vec3 lightDir		= normalize(light.position - vertPos); //vec pointing towards the light source
	
	vec3 viewDir		= normalize(viewPos - vertPos);
	vec3 reflectDir		= reflect(-lightDir, fragNorm);

	//ambient lighting
	vec3 ambientLight	= light.ambient * material.ambient;

	//diffuse lighting
	float diff			= max(dot(fragNorm, lightDir), 0.0);
	vec3 diffuseLight	= light.diffuse * (diff * material.diffuse);

	//specular lighting
	float spec			= pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specularLight	= light.specular * (spec * material.specular);

	vec3 lightResult = ambientLight + diffuseLight + specularLight;

	fragmentColour = texColour * vec4(lightResult, 1.0);
};
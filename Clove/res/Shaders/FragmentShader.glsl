#version 330 core

in vec2 vertTexCoord;
in vec3 vertPos;
in vec3 vertNormal;

struct Material{
	sampler2D	diffuse; //the diffuse map is basically the texture of our object
	sampler2D	specular;
	float		shininess;
};

struct Light{
	vec3 position;
	vec3 direction;
	float cutOff;
	float outerCutOff;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	//float constant;
	//float linear;
	//float quadratic;
};

uniform Material material;
uniform Light light;

uniform vec3 viewPos;

layout(location = 0) out vec4 fragmentColour;

void main(){
	vec3 fragNorm		= normalize(vertNormal);
	vec3 lightDir		= normalize(light.position - vertPos); //vec pointing towards the light source
	//vec3 lightDir		= normalize(-light.direction); //vec pointing away from the light source

	vec3 viewDir		= normalize(viewPos - vertPos);
	vec3 reflectDir		= reflect(-lightDir, fragNorm);


	//ambient lighting
	vec3 ambientLight	= light.ambient * vec3(texture(material.diffuse, vertTexCoord));

	//diffuse lighting
	float diff			= max(dot(fragNorm, lightDir), 0.0);
	vec3 diffuseLight	= light.diffuse * diff * vec3(texture(material.diffuse, vertTexCoord));

	//specular lighting
	float spec			= pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specularLight	= light.specular * spec * vec3(texture(material.specular, vertTexCoord));

	//attenuation
	//float dist			= length(light.position - vertPos);
	//float attenuation	= 1.0f / (light.constant + light.linear * dist + light.quadratic * (dist * dist));
	
	//ambientLight *= attenuation;
	//diffuseLight *= attenuation;
	//specularLight *= attenuation;

	float theta = dot(lightDir, normalize(-light.direction)); //flip the light direction (because it points from the fragment) around then get the cosine of the angles between them
	float epsilon = light.cutOff - light.outerCutOff;
	float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

	diffuseLight *= intensity;
	specularLight *= intensity;
	
	vec3 lightResult = ambientLight + diffuseLight + specularLight;
	fragmentColour = vec4(lightResult, 1.0);
};
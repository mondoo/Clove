#shader vertex
#version 330 core
//Runs once per vertex

layout(location = 0) in vec4 position; //location 0 here refers to the position in the vertex for where to get attribute from (what specified in glVertexAttribPointer)
layout(location = 1) in vec2 texCoord;
layout(location = 2) in vec3 normal;

out vec2 v_TexCoord; //This doesn't use a location because we use it to send data to another shader (the fragment shader in this case)
out vec3 v_fragPos;
out vec3 v_normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main(){
   gl_Position = projection * view * model * position;
  
   v_TexCoord = texCoord;
   v_fragPos = vec3(model * position); //this puts the fragment position in world space
   v_normal = mat3(transpose(inverse(model))) * normal; //This puts the normal in world space
};

#shader fragment
#version 330 core
//Runs once per pixel

layout(location = 0) out vec4 fragmentColour; //0 is less important here because this is a out vector and not an in vector

in vec2 v_TexCoord; //not using a location here (same reason for the out in the vertex shader)
in vec3 v_fragPos;
in vec3 v_normal;

uniform vec4 objectColour;
uniform vec4 lightColour;
uniform sampler2D u_Texture;
uniform float ambientStrength;
uniform vec3 lightPos;
uniform vec3 viewPos;

void main(){
	vec4 texColour = texture(u_Texture, v_TexCoord);
	
	//ambient lighting
	vec4 ambientLight = ambientStrength * lightColour;
	ambientLight.w = lightColour.w; //cheeky hack fix

	//diffuse lighting
	vec3 fragNorm = normalize(v_normal);
	vec3 lightDir = normalize(lightPos - v_fragPos);
	float diff = max(dot(fragNorm, lightDir), 0.0);
	vec4 diffuseLight = diff * lightColour;
	diffuseLight.w = lightColour.w; //cheeky hack fix

	//specular lighting
	float specularStrength = 0.5;
	vec3 viewDir = normalize(viewPos - v_fragPos);
	vec3 reflectDir = reflect(-lightDir, fragNorm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	vec4 specularLight = specularStrength * spec * lightColour;
	specularLight.w = lightColour.w; //cheeky hack fix

	vec4 lightResult = (ambientLight + diffuseLight + specularLight) * objectColour;

	fragmentColour = texColour * lightResult;
};
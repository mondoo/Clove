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

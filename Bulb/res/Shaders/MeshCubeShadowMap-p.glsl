#version 450

layout(std140, push_constant) uniform Light{
    layout(offset = 64) vec3 lightPos;
    float farPlane;
};

layout(location = 0) in vec4 fragPos;

void main(){
    gl_FragDepth = length(fragPos.xyz - lightPos.xyz) / farPlane;
}
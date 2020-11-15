#version 450

layout(std140, push_constant) uniform ModelProjection{
    mat4 modelProjection;
};

layout(location = 0) in vec2 position;
layout(location = 1) in vec2 texCoord;

layout(location = 0) out vec2 tex;

void main(){
    gl_Position = modelProjection * vec4(position, 0.0f, 1.0f);
    tex = texCoord;
}
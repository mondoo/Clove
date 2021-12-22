#version 450

//TODO: Push constant
layout(std140, set = 0, binding = 0) uniform ModelProjection{
    mat4 modelProjection;
};

layout(location = 0) in vec2 position;
layout(location = 1) in vec2 texCoord;

layout(location = 0) out vec2 tex;

void main(){
    gl_Position = modelProjection * vec4(position, 0.0f, 1.0f);
    tex = texCoord;
}
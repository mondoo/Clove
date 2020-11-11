#version 450

#include "Constants.glsl"

layout(std140, push_constant) uniform Colour{
    vec4 colour;
};

layout(set = SET_UI, binding = 0) uniform sampler2D texSampler;

layout(location = 0) in vec2 texCoord;

layout(location = 0) out vec4 outColour;

void main(){
    outColour = colour;
    outColour.a *= texture(texSampler, texCoord).r;
}
#version 450

#include "Constants.glsl"

layout(std140, push_constant) uniform Colour{
    layout(offset = 64) vec4 colour;
};

layout(set = 0, binding = 0) uniform sampler2D texSampler;

layout(location = 0) in vec2 texCoord;

layout(location = 0) out vec4 outColour;

void main(){
    outColour = colour;
    outColour.a *= texture(texSampler, texCoord).r;
}
//cmake generated header file

static const std::string shader_Unlit_ps = R"(
#version 460 core

in vec2 vertTexCoord;

layout(binding = 1) uniform sampler2D albedoSampler;

layout(location = 0) out vec4 fragmentColour;

void main(){
	fragmentColour = texture(albedoSampler, vertTexCoord);
};

)";

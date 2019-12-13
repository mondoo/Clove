#include <metal_stdlib>
using namespace metal;

struct viewBuffer{
	float4x4 modelProjection;
}

struct colourBuffer{
	float4 colour;
}

struct VSIn{
	float2 position [[ attribute(0) ]];
	float2 texCoord [[ attribute(1) ]];
}

struct VSOut{
	float2 position [[ position ]];
	float2 texCoord;
}

vertex VSOut vertexShader(const VSIn vertexIn, constant viewBuffer& viewBuffer [[ buffer(5) ]]){
	VSOut out;
	
	out.position = viewBuffer.modelProjection * float4(vertexIn.position.x, vertexIn.position.y, 0.0f, 1.0f);
	out.texCoord = vertexIn.texCoord;
	
	return out;
}

fragment half4 pixelShader(const VSOut vertexIn [[ stage_in ]], constant colourBuffer& colour [[ buffer(11) ]], texture2d<float> albedoTexture [[ texture(1) ]]){
	constexpr sampler defaultSampler;
	return texture.sample(defaultSampler, vertexIn.texCoord) * colour;
}

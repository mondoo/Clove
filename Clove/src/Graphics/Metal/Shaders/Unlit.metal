#include <metal_stdlib>
using namespace metal;

struct viewBuffer{
	float4x4 view;
	float4x4 projection;
}

struct modelBuffer{
	float4x4 model;
	float4x4 normalMatrix;
}

struct VSIn{
	float3 Position3D [[ attribute(0) ]];
	float2 TexCoord [[ attribute(1) ]];
}

struct VSOut{
	float4 position [[ position ]];
	float2 texCoord;
}

vertex VSOut vertexShader(const VSIn vertexIn, constant viewBuffer& viewBufferData [[ buffer(0) ]], const modelBuffer& modelBufferData [[ buffer(3) ]]){
	VSOut out;
	
	out.position = (projection * view * model) * float4(vertexIn.Position3D, 1.0f);
	out.texCoord = vertexIn.TexCoord;
	
	return out;
}

fragment half4 pixelShader(const VSOut vertexIn [[ stage_in ]], texture2d<float> albedoTexture [[ texture(1) ]]){
	constexpr sampler defaultSampler;
	return texture.sample(defaultSampler, vertexIn.texCoord);
}

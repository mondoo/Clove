#include <metal_stdlib>
using namespace metal;

struct viewBuffer{
	float4x4 view;
	float4x4 projection;
};

struct modelBuffer{
	float4x4 model;
	float4x4 normalMatrix;
};

struct VSIn{
	float3 Position3D [[ attribute(0) ]];
	float2 TexCoord [[ attribute(1) ]];
};

struct VSOut{
	float4 position [[ position ]];
	float2 texCoord;
};

vertex VSOut vertexShader(const VSIn vertexIn [[ stage_in ]], constant viewBuffer& viewBufferData [[ buffer(0) ]], constant modelBuffer& modelBufferData [[ buffer(3) ]]){
	VSOut out;
	
	out.position = (viewBufferData.projection * viewBufferData.view * modelBufferData.model) * float4(vertexIn.Position3D, 1.0f);
	out.texCoord = vertexIn.TexCoord;
	
	return out;
}

fragment float4 pixelShader(const VSOut vertexIn [[ stage_in ]], texture2d<float> albedoTexture [[ texture(1) ]]){
	constexpr sampler defaultSampler;
	return albedoTexture.sample(defaultSampler, vertexIn.texCoord);
}

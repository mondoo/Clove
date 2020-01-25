Texture2D albedoTexture : register(t1);
SamplerState albedoSampler : register(s1);

cbuffer colourBufferData : register(b12){
	float4 colour;
};

float4 main(float2 texCoord : TEXCOORD) : SV_TARGET{
	return albedoTexture.Sample(albedoSampler, texCoord) * colour;
}

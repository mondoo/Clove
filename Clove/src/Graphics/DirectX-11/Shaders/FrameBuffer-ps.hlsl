Texture2D frameBufferTexture : register(t4);
SamplerState frameBufferSampler : register(s4);

float4 main(float2 texCoord : TexCoord) : SV_TARGET{
	return frameBufferTexture.Sample(frameBufferSampler, texCoord);
}
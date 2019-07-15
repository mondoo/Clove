Texture2D spriteTexture : register(t3);
SamplerState spriteSampler : register(s3);

float4 main(float2 texCoord : TexCoord) : SV_TARGET{
	return spriteTexture.Sample(spriteSampler, texCoord);
}
Texture2D albedoTexture : register(t1);
SamplerState albedoSampler : register(s1);

float4 main(float2 texCoord : TexCoord) : SV_Target{
	return float4(1.0, 1.0, 1.0, albedoTexture.Sample(albedoSampler, texCoord).r);
}
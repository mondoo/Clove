Texture2D frameBufferTexture : register(t1);
SamplerState frameBufferSampler : register(s1);

float4 main(float2 texCoord : TexCoord) : SV_Target{
	return frameBufferTexture.Sample(frameBufferSampler, texCoord);

	/*float4 fragmentColour = frameBufferTexture.Sample(frameBufferSampler, texCoord);
	float average = 0.2126 * fragmentColour.r + 0.7152 * fragmentColour.g + 0.0722 * fragmentColour.b;
	return float4(average, average, average, 1.0);*/
}
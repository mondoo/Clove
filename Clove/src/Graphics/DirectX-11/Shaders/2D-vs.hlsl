float4 main(float2 pos : Position, float2 tex : TexCoord) : SV_POSITION{
	return float4(pos, 0.0f, 1.0f);
}
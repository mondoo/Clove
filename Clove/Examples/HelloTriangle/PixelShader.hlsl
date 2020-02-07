float4 main(float3 colour : VSOutColour) : SV_Target{
	return float4(colour, 1.0f);
}
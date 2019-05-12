cbuffer CBuff{
	float4 faceColours[6];
};

float4 main(uint tid : SV_PrimitiveID) : SV_Target{
	return faceColours[tid / 2];
}
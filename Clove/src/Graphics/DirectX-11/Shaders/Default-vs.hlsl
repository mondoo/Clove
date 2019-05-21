cbuffer Cbuff{
	matrix world;
	matrix view;
	matrix projection;
};

float4 main(float3 pos : Position) : SV_Position{
	matrix wvp = mul(projection, mul(view, world));
	return mul(wvp, float4(pos, 1.0f));
}
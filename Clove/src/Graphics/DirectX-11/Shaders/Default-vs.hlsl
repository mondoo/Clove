cbuffer Cbuff{
	matrix world;
	matrix view;
	matrix projection;
};

struct VSOut{
	float2 tex : TexCoord;
	float4 pos : SV_Position;
};

VSOut main(float3 pos : Position, float2 tex : TexCoord){
	VSOut vso;

	matrix wvp = mul(projection, mul(view, world));
	
	vso.tex = tex;
	vso.pos = mul(wvp, float4(pos, 1.0f));

	return vso;
}
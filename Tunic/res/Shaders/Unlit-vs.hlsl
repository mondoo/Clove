cbuffer viewBuffer : register(b1){
	matrix view;
	matrix projection;
};

cbuffer modelBuffer : register(b4){
	matrix model;
	matrix normalMatrix;
}

struct VSOut{
	float2 tex : TexCoord;
	float4 pos : SV_Position;
};

VSOut main(float3 pos : POSITION3D, float2 tex : TEXCOORD, float3 norm : NORMAL){
	VSOut vso;

	matrix mvp = mul(projection, mul(view, model));
	vso.pos = mul(mvp, float4(pos, 1.0f));

	vso.tex = tex;
	vso.tex.y = 1.0f - vso.tex.y; //In hlsl we slip the tex coords here so render targets behave the same across APIs

	return vso;
}

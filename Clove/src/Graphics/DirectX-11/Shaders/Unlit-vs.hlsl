cbuffer viewBuffer : register(b0){
	matrix view;
	matrix projection;
};

cbuffer modelBuffer : register(b3){
	matrix model;
	matrix normalMatrix;
}

struct VSOut{
	float2 tex : TexCoord;
	float4 pos : SV_Position;
};

VSOut main(float3 pos : Position, float2 tex : TexCoord, float3 norm : Normal){
	VSOut vso;

	matrix mvp = mul(projection, mul(view, model));
	vso.pos = mul(mvp, float4(pos, 1.0f));

	vso.tex = tex;

	return vso;
}
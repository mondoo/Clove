cbuffer viewBuffer : register(b5){
	matrix modelProjection;
};

struct VSOut{
	float2 tex : TexCoord;
	float4 pos : SV_Position;
};

VSOut main(float2 pos : Position2D, float2 tex : TexCoord){
	VSOut vso;

	vso.tex = tex;
	vso.pos = mul(modelProjection, float4(pos, 0.0f, 1.0f));

	return vso;
}
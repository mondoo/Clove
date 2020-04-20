cbuffer viewBuffer : register(b6){
	matrix modelProjection;
};

struct VSOut{
	float2 tex : TexCoord;
	float4 pos : SV_Position;
};

VSOut main(float2 pos : POSITION2D, float2 tex : TEXCOORD){
	VSOut vso;

	vso.tex = tex;
	#ifdef GLSL
		vso.tex.y = 1.0f - vso.tex.y;
	#endif
	vso.pos = mul(modelProjection, float4(pos, 0.0f, 1.0f));

	return vso;
}

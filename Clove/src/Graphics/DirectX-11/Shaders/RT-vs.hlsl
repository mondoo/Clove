struct VSOut{
	float2 tex : TexCoord;
	float4 pos : SV_Position;
};

VSOut main(float2 pos : Position, float2 tex : TexCoord){
	VSOut vsout;
	vsout.tex = tex;
	vsout.pos = float4(pos, 0.0f, 1.0f);
	return vsout;
}
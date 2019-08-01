struct VSOut{
	float2 tex : TexCoord;
	float4 pos : SV_Position;
};

VSOut main(float2 pos : Position, float2 tex : TexCoord){
	VSOut.tex = tex;
	VSOut.pos = pos;
	return VSOut;
}
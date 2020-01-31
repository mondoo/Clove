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
    float3 verp : VertPos;
    float3 vern : VertNormal;
	float4 pos : SV_Position;
};

VSOut main(float3 pos : POSITION3D, float2 tex : TEXCOORD, float3 norm : NORMAL){
	VSOut vso;

    matrix mvp = mul(projection, mul(view, model));
    vso.pos = mul(mvp, float4(pos, 1.0f));
	
	vso.tex = tex;

    //Convert frag and normal to world space
    vso.verp = (float3)mul(model, float4(pos, 1.0f));
    vso.vern = mul((float3x3)normalMatrix, norm);

	return vso;
}

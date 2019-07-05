cbuffer viewBuffer : register(b5){
	matrix modelProjection;
};

float4 main(float2 pos : Position, float2 tex : TexCoord) : SV_POSITION{
	return mul(modelProjection, float4(pos, 0.0f, 1.0f));
}
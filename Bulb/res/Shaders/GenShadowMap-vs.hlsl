cbuffer ModelBuffer : register(b4){
	matrix model;
	matrix normalMatrix;
}

cbuffer ShadowTransform : register(b11){
	matrix lightSpaceMatrix;
}

float4 main(float3 pos : POSITION3D) : SV_Position{
	return mul(lightSpaceMatrix, mul(model, float4(pos, 1.0)));
}

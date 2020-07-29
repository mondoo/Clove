cbuffer ModelBuffer : register(b5){
	matrix model;
	matrix normalMatrix;
}

float4 main(float3 pos : POSITION3D) : SV_Position{
	return mul(model, float4(pos, 1.0));
}

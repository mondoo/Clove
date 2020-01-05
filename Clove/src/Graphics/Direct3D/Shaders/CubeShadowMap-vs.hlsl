cbuffer ModelBuffer : register(b4){
	matrix model;
	matrix normalMatrix;
}

float4 main(float3 pos : Position3D) : SV_Position{
	return mul(model, float4(pos, 1.0));
}

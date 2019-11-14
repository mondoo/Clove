cbuffer shadowMatBuffer : register(b6){
	matrix shadowMatrices[6];
}

cbuffer lightNumBuffer : register(b9){
	unsigned int currentLightIndex;
}

struct GSOutPut{
	float4 vertPos : VertPos;
	float4 adjustedVertPos : SV_Position;
	uint face : SV_RenderTargetArrayIndex;
};

float4x4 scale(float4x4 m, float3 v){
	float x = v.x;
	float y = v.y;
	float z = v.z;

	m[0][0] *= x; m[1][0] *= y; m[2][0] *= z;
	m[0][1] *= x; m[1][1] *= y; m[2][1] *= z;
	m[0][2] *= x; m[1][2] *= y; m[2][2] *= z;
	m[0][3] *= x; m[1][3] *= y; m[2][3] *= z;

	return m;
}

[maxvertexcount(18)]
void main(triangle float4 inTriangle[3] : SV_Position, inout TriangleStream<GSOutPut> outTriangles){
	GSOutPut outPut;
	for(int face = 0; face < 6; face++){
		//We flip the Y axis here to account of the coord system DirectX uses for textures
		matrix faceMat = scale(shadowMatrices[face], float3(1.0, -1.0, 1.0)); 
		outPut.face = face + currentLightIndex;
		for(int i = 0; i < 3; i++){
			outPut.vertPos = inTriangle[i];
			outPut.adjustedVertPos = mul(faceMat, outPut.vertPos);
			outTriangles.Append(outPut);
		}
		outTriangles.RestartStrip();
	}
}
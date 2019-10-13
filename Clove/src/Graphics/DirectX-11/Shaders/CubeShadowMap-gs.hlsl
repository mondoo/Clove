cbuffer shadowMatBuffer : register(b6){
	matrix shadowMatrices[6];
}

struct GSOutPut{
	uint face : SV_RenderTargetArrayIndex;
	float4 vertPos : VertPos;
};

[maxvertexcount(18)]
void main(triangle float4 inTriangle[3] : SV_Position, inout TriangleStream<GSOutPut> outTriangles){
	GSOutPut outPut;
	
	for(int face = 0; face < 6; face++){
		outPut.face = face;
		for(int i = 0; i < 3; i++){
			outPut.vertPos = mul(shadowMatrices[face], inTriangle[i]);
			outTriangles.Append(outPut);
		}
		outTriangles.RestartStrip();
	}
}
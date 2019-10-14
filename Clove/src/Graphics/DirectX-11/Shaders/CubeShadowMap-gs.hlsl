cbuffer shadowMatBuffer : register(b6){
	matrix shadowMatrices[6];
}

struct GSOutPut{
	float4 vertPos : VertPos;
	float4 adjustedVertPos : SV_Position;
	uint face : SV_RenderTargetArrayIndex;
};

[maxvertexcount(18)]
void main(triangle float4 inTriangle[3] : SV_Position, inout TriangleStream<GSOutPut> outTriangles){
	GSOutPut outPut;
	for(int face = 0; face < 6; face++){
		outPut.face = face;
		for(int i = 0; i < 3; i++){
			outPut.vertPos = inTriangle[i];
			outPut.adjustedVertPos = mul(shadowMatrices[face], outPut.vertPos);
			outTriangles.Append(outPut);
		}
		outTriangles.RestartStrip();
	}
}
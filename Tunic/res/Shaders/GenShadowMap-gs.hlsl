cbuffer lightNumBuffer : register(b10){
	int currentLightIndex;
}

struct GSOutPut{
	float4 vertPos : SV_Position;
	uint face : SV_RenderTargetArrayIndex;
};

[maxvertexcount(3)]
void main(triangle float4 inTriangle[3] : SV_Position, inout TriangleStream<GSOutPut> outTriangles){
	GSOutPut outPut;
	outPut.face = currentLightIndex;
	for(int i = 0; i < 3; i++){
		outPut.vertPos = inTriangle[i];
		outTriangles.Append(outPut);
	}
	outTriangles.RestartStrip();
}
cbuffer lighthPosBuffer : register (b7){
	float3 lightPosition;
	float farplane;
}

float main(float4 vertPosition : SV_Position) : SV_Depth{
	const float lightDistance = length(vertPosition.xyz - lightPosition) / farplane;
	return lightDistance;
}
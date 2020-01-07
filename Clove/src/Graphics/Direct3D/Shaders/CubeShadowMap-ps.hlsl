cbuffer lightPosBuffer : register (b11){
	float3 lightPosition;
	float farplane;
}

float main(float4 vertPosition : VertPos) : SV_Depth{
	const float lightDistance = length(vertPosition.xyz - lightPosition.xyz) / farplane;
	return lightDistance;
}

#define MAX_LIGHTS 10
#define MAX_JOINTS 255 //std::numeric_limits<uint8_t>::max();

cbuffer ModelBuffer : register(b5){
	matrix model;
	matrix normalMatrix;
}

cbuffer ShadowTransform : register(b11){
	matrix lightSpaceMatrix;
}

cbuffer SkeletalData : register(b13){
    matrix jointTransforms[MAX_JOINTS];    
}

float4 main(float3 pos : POSITION3D, int4 jointIds : JOINTIDS, float4 weights : WEIGHTS) : SV_Position{   
    float4 animatedPos = float4(0.0f, 0.0f, 0.0f, 0.0f);
    float4 animatedNormal = float4(0.0f, 0.0f, 0.0f, 0.0f);
    
    for(int j = 0; j < 4; ++j){ //4 is the max amount of weights
        const matrix jointTransform = jointTransforms[jointIds[j]];
        const float weight = weights[j];
        
        animatedPos += (mul(jointTransform, float4(pos, 1.0f)) * weight);
        //animatedNormal += (mul(jointTransform, float4(norm, 0.0f)) * weight);
    }

	return mul(mul(lightSpaceMatrix, model), animatedPos);
}

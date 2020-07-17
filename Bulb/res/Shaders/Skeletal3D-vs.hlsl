#define MAX_LIGHTS 10
#define MAX_JOINTS 255 //std::numeric_limits<uint8_t>::max();

cbuffer viewBuffer : register(b2){
	matrix view;
	matrix projection;
};

cbuffer modelBuffer : register(b5){
    matrix model;
    matrix normalMatrix;
}

cbuffer LightCount : register(b9){
	int numDirectionalLight;
	int numPointLight;
}

cbuffer directionalLightTransform : register(b11){
    matrix directionalLightTransforms[MAX_LIGHTS]; 
}

cbuffer SkeletalData : register(b13){
    matrix jointTransforms[MAX_JOINTS];    
}

struct VSOut{
    float2 tex : TexCoord;
    float3 verp : VertPos;
    float3 vern : VertNormal;
    float4 vertPosLightSpace[MAX_LIGHTS] : VPLS;
	float4 pos : SV_Position;
};

VSOut main(float3 pos : POSITION3D, float2 tex : TEXCOORD, float3 norm : NORMAL, int4 jointIds : JOINTIDS, float4 weights : WEIGHTS){
	VSOut vso;
    
    const matrix mvp = mul(projection, mul(view, model));
    
    float4 animatedPos = float4(0.0f, 0.0f, 0.0f, 0.0f);
    float4 animatedNormal = float4(0.0f, 0.0f, 0.0f, 0.0f);
    
    for(int j = 0; j < 4; ++j){ //4 is the max amount of weights
        const matrix jointTransform = jointTransforms[jointIds[j]];
        const float weight = weights[j];
        
        animatedPos += (mul(jointTransform, float4(pos, 1.0f)) * weight);
        animatedNormal += (mul(jointTransform, float4(norm, 0.0f)) * weight);
    }

    vso.pos = mul(mvp, animatedPos);
	vso.tex = tex;

    //Convert frag and normal to world space
    vso.verp = (float3)mul(model, animatedPos);
    vso.vern = (float3)mul(normalMatrix, animatedNormal);
    
    //Convert frag into the light's view
    for(int i = 0; i < numDirectionalLight; ++i){
        vso.vertPosLightSpace[i] = mul(directionalLightTransforms[i], float4(vso.verp, 1.0f));
    }

	return vso;
}

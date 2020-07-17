#define MAX_LIGHTS 10

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

struct VSOut{
    float2 tex : TexCoord;
    float3 verp : VertPos;
    float3 vern : VertNormal;
    float4 vertPosLightSpace[MAX_LIGHTS] : VPLS;
	float4 pos : SV_Position;
};

VSOut main(float3 pos : POSITION3D, float2 tex : TEXCOORD, float3 norm : NORMAL){
	VSOut vso;

    matrix mvp = mul(projection, mul(view, model));
    vso.pos = mul(mvp, float4(pos, 1.0f));
	
	vso.tex = tex;

    //Convert frag and normal to world space
    vso.verp = (float3)mul(model, float4(pos, 1.0f));
    vso.vern = mul((float3x3)normalMatrix, norm);
    
    //Convert frag into the light's view
    for(int i = 0; i < numDirectionalLight; ++i){
        vso.vertPosLightSpace[i] = mul(directionalLightTransforms[i], float4(vso.verp, 1.0f));
    }

	return vso;
}

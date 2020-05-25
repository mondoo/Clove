static const uint poissonDiskSamples = 16;
static const float2 poissonDisk[poissonDiskSamples] = {
    float2( 0.0f, 0.0f ),
    float2( -0.38930751557801496f, 0.9200813880012744f ),
    float2( -0.6376234183115269f, -0.7613403150005352f ),
    float2( 0.7058340574915969f, 0.7016727846245917f ),
    float2( 0.38815768678720447f, -0.8597858039195242f ),
    float2( -0.9118393527876706f, 0.13113504554902722f ),
    float2( 0.9101300583520032f, -0.1543393834189305f ),
    float2( 0.1761816658753382f, 0.9577651087518873f ),
    float2( -0.4191944430424054f, 0.359228155567037f ),
    float2( -0.4573249714095428f, -0.2683961704824749f ),
    float2( 0.5216494478901522f, 0.18250631369911216f ),
    float2( 0.455530175273591f, -0.3227148635547987f ),
    float2( -0.14229982793732962f, -0.9653441384296594f ),
    float2( -0.8038965790783993f, 0.569709420414335f ),
    float2( -0.028624467518849367f, -0.48416464568413725f ),
    float2( 0.06466427815165597f, 0.44154714994828026f ),
};

float2 getAverageBlockerDistance(Texture2DArray tex, SamplerState state, float3 projectionCoords, float lightIndex, float2 searchSize){
    //Just going to do a simple search area for now
    const uint sampleSize = 5;
    const float2 sampleArea[sampleSize] = {
        float2( 0.0f,  0.0f),
        float2( 1.0f, -1.0f),
        float2( 1.0f,  1.0f),
        float2(-1.0f, -1.0f),
        float2(-1.0f,  1.0f),
    };
    
    int blockers = 0;
    float avgerageBlockerDist = 0.0f;
    
    const float currentDepth = projectionCoords.z;
    
    for(int i = 0; i < sampleSize; ++i){ //TODO: Make poisson disk
        const float2 sampleLocation = projectionCoords.xy + sampleArea[sampleSize] * searchSize;
        
        const float depth = tex.Sample(state, float3(sampleLocation, lightIndex)).r;
        if(depth < currentDepth){
            blockers++;
            avgerageBlockerDist += depth;
        }
    }
    
    avgerageBlockerDist /= blockers;
    
    return float2(avgerageBlockerDist, float(blockers));
}

float estimatePenumbraSize(float lightSize, float recieverDepth, float averageBlockerDepth){
    return lightSize * (recieverDepth - averageBlockerDepth) / averageBlockerDepth;    
}

float GenerateShadow_PCF(Texture2DArray tex, SamplerState state, float3 projectionCoords, float lightIndex, float shadowOffsetBias, float2 texelSize){
    float currentDepth = projectionCoords.z;
	
	float shadow = 0.0f;
	for(uint i = 0; i < poissonDiskSamples; ++i){
		const float2 sampleLocation = projectionCoords.xy + poissonDisk[i] * texelSize;
		float closestDepth = tex.Sample(state, float3(sampleLocation, lightIndex)).r; //TODO: Try tex.Gather to use bilinear filtering
		shadow += currentDepth - shadowOffsetBias > closestDepth ? 1.0f : 0.0f;
	}	
	shadow /= poissonDiskSamples;
    
    return shadow;
}
    
	float shadow = 0.0f;
	for(uint i = 0; i < poissonDiskSamples; ++i){
		const float2 sampleLocation = projectionCoords.xy + poissonDisk[i] * texelSize;
		float closestDepth = tex.Sample(state, float3(sampleLocation, lightIndex)).r; //TODO: Try tex.Gather to use bilinear filtering
		shadow += currentDepth - shadowOffsetBias > closestDepth ? 1.0f : 0.0f;
	}	
	shadow /= poissonDiskSamples;
    
    return shadow;
}
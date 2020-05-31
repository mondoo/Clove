static const uint poissonDiskSamples = 32;
static const float2 poissonDisk[poissonDiskSamples] = {
    float2(  0.0f, 0.0f ), 
    float2(  0.8048486986946776f,  -0.5852050849392686f   ),    
    float2( -0.9958393427837682f,   0.036113738734521734f ),  
    float2(  0.9072570718270919f,   0.40734700083389624f  ),    
    float2( -0.22789385343283575f,  0.9644404847581753f   ),   
    float2( -0.10689271592202236f, -0.954188338841789f    ),   
    float2( -0.48719337104285393f, -0.42854765804837486f  ), 
    float2(  0.4400381238393658f,   0.728434169786693f    ),      
    float2( -0.6387109841365166f,   0.562209120212132f    ),     
    float2(  0.634731997690473f,   -0.05839784444054089f  ),    
    float2(  0.20747687061341666f, -0.45928201536281393f  ),  
    float2(  0.4157658097156909f,   0.30202918239243165f  ),    
    float2( -0.1906360581448456f,   0.4826355463106642f   ),    
    float2(  0.3134312467124866f,  -0.8695656411272433f   ),    
    float2( -0.4621244966594415f,   0.034149766201863836f ),  
    float2( -0.9333946320259354f,  -0.35319339389923116f  ),  
    float2( -0.513692645101771f,   -0.8400632705059478f   ),    
    float2(  0.9507042255582802f,  -0.22996329319736644f  ),   
    float2( -0.1299055116233461f,  -0.45888900214621164f  ),  
    float2( -0.777115314506443f,   -0.6055347842708125f   ),
    float2(  0.15561938048113194f,  0.5497352288871444f   ),
    float2(  0.1712754063845962f,   0.9805812446301038f   ),
    float2(  0.49633530791493974f, -0.3394307591564362f   ),
    float2(  0.2909022130270505f,  -0.11290995348313707f  ),
    float2( -0.8982917058001596f,   0.3368474679600616f   ),
    float2( -0.25066771849259145f, -0.16150164929802732f  ),
    float2( -0.2334001875946011f,   0.20550212549089877f  ),
    float2( -0.3852498720717099f,   0.6910218613664254f   ),
    float2(  0.08092990179776498f, -0.720619256823693f    ),
    float2(  0.5518520067173671f,  -0.6806660242989737f   ),
    float2( -0.634849590405609f,    0.2817332624087997f   ),
    float2( -0.7317371150804994f,  -0.11790178286328051f  ),
};

float2 rotate(float2 pos, float angle){
    float2 rotated;
    rotated.x = (cos(angle) * pos.x) - (sin(angle) * pos.y);
    rotated.y = (sin(angle) * pos.x) + (cos(angle) * pos.y);
    return rotated;
}

float2 getAverageBlockerDistance(Texture2DArray tex, SamplerState state, float3 projectionCoords, float lightIndex, float offset){
    int blockers = 0;
    float avgerageBlockerDist = 0.0f;
    
    const float currentDepth = projectionCoords.z;
    
    for(int i = 0; i < poissonDiskSamples; ++i){
        const float2 rotatedOffset = rotate(poissonDisk[i] * offset, 0.785398f);
        const float2 sampleLocation = projectionCoords.xy + rotatedOffset;
        
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
    const float penumbra = lightSize * (recieverDepth - averageBlockerDepth) / averageBlockerDepth;  
    return penumbra;
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
    
float GenerateShadow_PCSS(Texture2DArray tex, SamplerState state, float3 projectionCoords, float lightIndex, float shadowOffsetBias, float2 texelSize){   
    const float offset = 1.0f / 50.0f; //Chosen by experiementation, maybe something we'd want to control with a constant buffer in future
    float averageBlockerDistance = getAverageBlockerDistance(tex, state, projectionCoords, lightIndex, offset);
    
    if(averageBlockerDistance == 0.0f){
        return 0.0f;
    }else{
        const float lightSize = 1.0f; //NOTE: Currently we only have a point and directional lights (an no area lights) so our light size is hard coded
        const float penumbraSize = estimatePenumbraSize(lightSize, projectionCoords.z, averageBlockerDistance.x);
        const float shadow = GenerateShadow_PCF(tex, state, projectionCoords, lightIndex, shadowOffsetBias, penumbraSize);
        
        return shadow;
    }
}
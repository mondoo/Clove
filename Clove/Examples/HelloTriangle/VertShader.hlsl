struct VSOut{
    float3 colour : VSOutColour;
    float4 pos : SV_Position;
};

VSOut main(float2 pos : POSITION2D, float3 colour : COLOUR3D){
    VSOut vsout;
    vsout.colour = colour;
    vsout.pos = float4(pos, 0.0f, 1.0f);
    return vsout;
}
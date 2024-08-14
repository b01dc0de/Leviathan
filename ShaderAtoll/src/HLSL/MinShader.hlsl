////////////////////
// MinShader.hlsl //
////////////////////

#ifndef ENABLE_VERTEX_COLOR
    #define ENABLE_VERTEX_COLOR 0
#endif

struct VS_INPUT
{
    float4 Pos : POSITION;
    float4 RGBA : COLOR;
};

#if ENABLE_VERTEX_COLOR
struct PS_INPUT
{
    float4 Pos : SV_POSITION;
    float4 RGBA : COLOR;
};

PS_INPUT VSMain(VS_INPUT Input)
{
    PS_INPUT Output;
    Output.Pos = Input.Pos;
    Output.RGBA = Input.RGBA;
    return Output;
}

float4 PSMain(PS_INPUT Input) : SV_Target
{
    return Input.RGBA;
}
#else // !ENABLE_VERTEX_COLOR
float4 VSMain(VS_INPUT Input) :SV_POSITION
{
    return Input.Pos;
}
float4 PSMain(float4 Pos:SV_POSITION:LinearNoInterpolationCentroid) :SV_Target
{
    return Pos;
}
#endif // ENABLE_VERTEX_COLOR



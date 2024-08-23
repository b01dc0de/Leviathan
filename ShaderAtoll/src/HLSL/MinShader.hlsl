////////////////////
// MinShader.hlsl //
////////////////////

#ifndef ENABLE_VERTEX_COLOR
    #define ENABLE_VERTEX_COLOR 0
#endif

cbuffer AtollGlobals : register(b0)
{
    uint FrameWidth;
    uint FrameHeight;
    float AppTime;
    float DeltaTime;
};

struct VS_INPUT
{
    float4 Pos : POSITION;
    float4 RGBA : COLOR;
};

struct PS_INPUT
{
    float4 Pos : SV_Position;
#if ENABLE_VERTEX_COLOR
    float4 RGBA : COLOR : qwer;
#endif //ENABLE_VERTEX_COLOR
};

PS_INPUT VSMain(VS_INPUT Input)
{
    PS_INPUT Output;
    Output.Pos = Input.Pos;
#if ENABLE_VERTEX_COLOR
    Output.RGBA = Input.RGBA;
#endif //ENABLE_VERTEX_COLOR
    return Output;
}

#if ENABLE_VERTEX_COLOR
float4 PSMain(PS_INPUT Input) : SV_Target
{
    return Input.Pos;
    //return Input.RGBA;
}
#else // ENABLE_VERTEX_COLOR
float4 PSMain(float4 Pos : SV_Position) : SV_Target
{
    return Pos;
}
#endif // ENABLE_VERTEX_COLOR


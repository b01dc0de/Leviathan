////////////////////
// MinShader.hlsl //
////////////////////

#ifndef ENABLE_VERTEX_COLOR
    #define ENABLE_VERTEX_COLOR 0
#endif

#define _PI (3.14159265359)

cbuffer AtollGlobals : register(b0)
{
    uint FrameWidth;
    uint FrameHeight;
    float AppTime;
    float DeltaTime;
};

float4 GetColorFromSVPos(float4 SVPos)
{
    const float PI_DIV_3 = _PI / 3.0;
    float Red = cos(AppTime) * (SVPos.x / FrameWidth);
    float Green = (cos(AppTime - _PI) + sin(AppTime + _PI)) / 2.0 * (SVPos.x + SVPos.y) / (FrameWidth + FrameHeight);
    float Blue = sin((AppTime * 2.0) - _PI) * SVPos.y / FrameHeight;
    float4 Result = { Red, Green, Blue, 1.0 };
    return Result;
}

struct VS_INPUT
{
    linear float4 Pos : POSITION;
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

float4 PSMain(PS_INPUT Input) : SV_Target
{
#if ENABLE_VERTEX_COLOR
    return Input.RGBA;
#else // ENABLE_VERTEX_COLOR
    return GetColorFromSVPos(Input.Pos);
#endif // ENABLE_VERTEX_COLOR
}


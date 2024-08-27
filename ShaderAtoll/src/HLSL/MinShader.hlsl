////////////////////
// MinShader.hlsl //
////////////////////

#ifndef ENABLE_VERTEX_COLOR
    #define ENABLE_VERTEX_COLOR 0
#endif

#define _PI (3.14159265359)

cbuffer AtollGlobals : register(b0)
{
    float AppTime;
    float DeltaTime;
    int FrameWidth;
    int FrameHeight;
    int MouseX;
    int MouseY;
};

float4 AtollMain_Ver0(float4 SVPos)
{
    const float PI_DIV_3 = _PI / 3.0;
    float Red = cos(AppTime) * (SVPos.x / FrameWidth);
    float Green = (cos(AppTime - _PI) + sin(AppTime + _PI)) / 2.0 * (SVPos.x + SVPos.y) / (FrameWidth + FrameHeight);
    float Blue = sin((AppTime * 2.0) - _PI) * SVPos.y / FrameHeight;
    float4 Result = { Red, Green, Blue, 1.0 };

    float DistToMouseX = (MouseX - SVPos.x);
    float DistToMouseY = (MouseY - SVPos.y);
    float DistFromMouse = sqrt(DistToMouseX*DistToMouseX + DistToMouseY*DistToMouseY);
    const float MouseDistThreshold = 50.0;
    if (DistFromMouse < MouseDistThreshold)
    {
        float BlendFactor = (MouseDistThreshold - DistFromMouse) / MouseDistThreshold;
        Result = lerp(Result, 1.0, BlendFactor);
    }
    return Result;
}
float4 AtollMain_Ver1(float4 SVPos)
{
    float4 Result = AtollMain_Ver0(SVPos);
    Result.x = 1.0 - Result.x;
    Result.y = 1.0 - Result.y;
    Result.z = 1.0 - Result.z;
    return Result;
}
#define MAIN_VERSION (1)
float4 AtollMain(float4 SVPos)
{
#if MAIN_VERSION == 0
    return AtollMain_Ver0(SVPos);
#elif MAIN_VERSION == 1
    return AtollMain_Ver1(SVPos);
#else
    return 0.0;
#endif // MAIN_VERSION
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
    return AtollMain(Input.Pos);
#endif // ENABLE_VERTEX_COLOR
}


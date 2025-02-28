/////////////////////////
// InstRectShader.hlsl //
/////////////////////////

#pragma pack_matrix(row_major)

#ifndef INST_RECT_COLOR
    #define INST_RECT_COLOR (0)
#endif // INST_RECT_COLOR
#ifndef INST_RECT_TEXTURE
    #define INST_RECT_TEXTURE (0)
#endif // INST_RECT_TEXTURE
#ifndef INST_RECT_ROTATION
    #define INST_RECT_ROTATION (0)
#endif // INST_RECT_ROTATION

cbuffer WorldBuffer : register(b0)
{
    matrix World;
}

cbuffer ViewProjBuffer : register(b1)
{
    matrix View;
    matrix Proj;
}

struct VS_INPUT
{
    float4 Pos : POSITION;
    float4 Rect : RECT;
#if INST_RECT_COLOR
    float4 RGBA : COLOR;
#endif //INST_RECT_COLOR
#if INST_RECT_TEXTURE
    float4 UVRect : TEXCOORD;
#endif // INST_RECT_TEXTURE
#if INST_RECT_ROTATION
    float AngleZ : ANGLE;
#endif // INST_RECT_ROTATION
};

struct VS_OUTPUT
{
    float4 Pos : SV_POSITION;
#if INST_RECT_COLOR
    float4 RGBA : COLOR;
#endif //INST_RECT_COLOR
#if INST_RECT_TEXTURE
    float2 TexUV : TEXCOORD;
#endif // INST_RECT_TEXTURE
};

#if INST_RECT_TEXTURE
    Texture2D MainTexture : register(t0);
    SamplerState MainSampler : register(s0);
#endif // INST_RECT_TEXTURE

float2 RectTransform(float2 InPos, float4 InRect)
{
    /* NOTE:
        InRect.x == PosX    InRect.y == PosY
        InRect.z == SizeX   InRect.w == SizeY
    */
    float AdjX = InRect.x + (InPos.x * InRect.z);
    float AdjY = InRect.y + (InPos.y * InRect.w);
    return float2(AdjX, AdjY);
}

float2 TransformSRT(float2 InPos, float4 InRect, float AngleZ)
{
    float2 Result = { InPos.x * InRect.z, InPos.y * InRect.w };
    Result = float2(
        Result.x * cos(AngleZ) + Result.y * -sin(AngleZ),
        Result.x * sin(AngleZ) + Result.y * cos(AngleZ)
    );
    Result.x += InRect.x;
    Result.y += InRect.y;
    return Result;
}

VS_OUTPUT VSMain(VS_INPUT Input)
{
    VS_OUTPUT Output;
#if INST_RECT_ROTATION
    float2 RectPoint = TransformSRT(Input.Pos.xy, Input.Rect, Input.AngleZ);
#else // INST_RECT_ROTATION
    float2 RectPoint = RectTransform(Input.Pos.xy, Input.Rect);
#endif // INST_RECT_ROTATION
    Output.Pos = mul(float4(RectPoint, Input.Pos.zw), World);
    Output.Pos = mul(Output.Pos, View);
    Output.Pos = mul(Output.Pos, Proj);
#if INST_RECT_COLOR
    Output.RGBA = Input.RGBA;
#endif //INST_RECT_COLOR
#if INST_RECT_TEXTURE
    float2 PosInvY = float2(Input.Pos.x + (0.5 * INST_RECT_ROTATION), 1 - Input.Pos.y + (0.5 * INST_RECT_ROTATION));
    Output.TexUV = RectTransform(PosInvY, Input.UVRect);
#endif // INST_RECT_TEXTURE
    return Output;
}

float4 PSMain(VS_OUTPUT Input) : SV_Target
{
#if INST_RECT_COLOR
    return Input.RGBA;
#endif //INST_RECT_COLOR
#if INST_RECT_TEXTURE
    return MainTexture.Sample(MainSampler, Input.TexUV);
#endif // INST_RECT_TEXTURE
    return float4(0, 0, 0, 1);
}


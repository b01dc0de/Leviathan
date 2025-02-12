/////////////////////////
// InstRectShader.hlsl //
/////////////////////////

#pragma pack_matrix(row_major)

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
    float4 RGBA : COLOR;
};

struct VS_OUTPUT
{
    float4 Pos : SV_POSITION;
    float4 RGBA : COLOR;
};

float4 RectTransform(float4 InPos, float4 InRect)
{
    float PosX = InRect.x;
    float PosY = InRect.y;
    float SizeX = InRect.z;
    float SizeY = InRect.w;

    float AdjX = PosX + (InPos.x * SizeX);
    float AdjY = PosY + (InPos.y * SizeY);

    return float4(AdjX, AdjY, InPos.z, InPos.w);
}

VS_OUTPUT VSMain(VS_INPUT Input)
{
    VS_OUTPUT Output;
    Output.Pos = mul(RectTransform(Input.Pos, Input.Rect), World);
    Output.Pos = mul(Output.Pos, View);
    Output.Pos = mul(Output.Pos, Proj);
    Output.RGBA = Input.RGBA;
    return Output;
}

float4 PSMain(VS_OUTPUT Input) : SV_Target
{
    return Input.RGBA;
}

/////////////////////////
// InstLineShader.hlsl //
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
    float4 Line : LINE;
    float4 RGBA : COLOR;
};

struct VS_OUTPUT
{
    float4 Pos : SV_POSITION;
    float4 RGBA : COLOR;
};

float2 LineTransform(float2 InPos, float4 InLine)
{
    // NOTE:
    // InLine.x == BeginX
    // InLine.y == BeginY
    // InLine.z == EndX
    // InLine.w == EndY
    float RangeX = InLine.z - InLine.x;
    float RangeY = InLine.w - InLine.y;
    float2 OutPos = float2
    (
        InLine.x + (RangeX * InPos.x),
        InLine.y + (RangeY * InPos.y)
    );
    return OutPos;
}

VS_OUTPUT VSMain(VS_INPUT Input)
{
    VS_OUTPUT Output;
    Output.Pos = mul(float4(LineTransform(Input.Pos.xy, Input.Line), Input.Pos.zw), World);
    Output.Pos = mul(Output.Pos, View);
    Output.Pos = mul(Output.Pos, Proj);
    Output.RGBA = Input.RGBA;
    return Output;
}

float4 PSMain(VS_OUTPUT Input) : SV_Target
{
    return Input.RGBA;
}


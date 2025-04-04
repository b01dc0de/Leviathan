//////////////////////////
// InstVoxelShader.hlsl //
//////////////////////////

#pragma pack_matrix(row_major)

cbuffer ViewProjBuffer : register(b0)
{
    matrix View;
    matrix Proj;
}

struct VS_INPUT
{
    float4 Pos : POSITION;
    float4 WorldPos : INSTPOS;
    float4 RGBA : COLOR;
    float Scale : SCALE;
};

struct VS_OUTPUT
{
    float4 Pos : SV_POSITION;
    float4 RGBA : COLOR;
};

VS_OUTPUT VSMain(VS_INPUT Input)
{
    VS_OUTPUT Output;
    Output.Pos = (Input.Pos * Input.Scale) + Input.WorldPos;
    Output.Pos = mul(Output.Pos, View);
    Output.Pos = mul(Output.Pos, Proj);
    Output.RGBA = Input.RGBA;
    return Output;
}

float4 PSMain(VS_OUTPUT Input) : SV_Target
{
    return Input.RGBA;
}


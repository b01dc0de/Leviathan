////////////////////
// MinShader.hlsl //
////////////////////

struct VS_INPUT
{
    float4 Pos : POSITION;
    float4 RGBA : COLOR;
};

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


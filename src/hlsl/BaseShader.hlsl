/////////////////////
// BaseShader.hlsl //
/////////////////////

#pragma pack_matrix(row_major)

#ifndef ENABLE_VERTEX_COLOR
    #define ENABLE_VERTEX_COLOR (0)
#endif // ENABLE_VERTEX_COLOR
#ifndef ENABLE_VERTEX_TEXTURE
    #define ENABLE_VERTEX_TEXTURE (0)
#endif // ENABLE_VERTEX_TEXTURE
#ifndef ENABLE_VERTEX_NORMAL
    #define ENABLE_VERTEX_NORMAL (0)
#endif // ENABLE_VERTEX_NORMAL
#ifndef ENABLE_WVP_TRANSFORM
    #define ENABLE_WVP_TRANSFORM (0)
#endif // ENABLE_WVP_TRANSFORM
#ifndef ENABLE_UNICOLOR
    #define ENABLE_UNICOLOR (0)
#endif // ENABLE_WVP_TRANSFORM

#if ENABLE_WVP_TRANSFORM
    cbuffer WorldBuffer : register(b0)
    {
        matrix World;
    }
    cbuffer ViewProjBuffer : register(b1)
    {
        matrix View;
        matrix Proj;
    }
#endif // ENABLE_WVP_TRANSFORM
#if ENABLE_UNICOLOR
    cbuffer UnicolorBuffer : register(b2)
    {
        float4 Unicolor;
    }
#endif // ENABLE_UNICOLOR

#if ENABLE_VERTEX_TEXTURE
    Texture2D MainTexture : register(t0);
    SamplerState MainSampler : register(s0);
#endif // ENABLE_VERTEX_TEXTURE

struct VS_INPUT
{
    float4 Pos : POSITION;
#if ENABLE_VERTEX_COLOR
    float4 RGBA : COLOR;
#endif // ENABLE_VERTEX_COLOR
#if ENABLE_VERTEX_TEXTURE
    float2 TexUV : TEXCOORD;
#endif // ENABLE_VERTEX_TEXTURE
#if ENABLE_VERTEX_NORMAL
    float3 Normal : NORMAL;
#endif // ENABLE_VERTEX_NORMAL

};

struct VS_OUTPUT
{
    float4 Pos : SV_POSITION;
#if ENABLE_VERTEX_COLOR
    float4 RGBA : COLOR;
#endif // ENABLE_VERTEX_COLOR
#if ENABLE_VERTEX_TEXTURE
    float2 TexUV : TEXCOORD;
#endif // ENABLE_VERTEX_TEXTURE
#if ENABLE_VERTEX_NORMAL
    float3 Normal : NORMAL;
#endif // ENABLE_VERTEX_NORMAL
#if ENABLE_UNICOLOR
    float4 Unicolor : COLOR;
#endif // ENABLE_UNICOLOR
};

VS_OUTPUT VSMain(VS_INPUT Input)
{
    VS_OUTPUT Output;
    Output.Pos = Input.Pos;
#if ENABLE_WVP_TRANSFORM
    Output.Pos = mul(Output.Pos, World);
    Output.Pos = mul(Output.Pos, View);
    Output.Pos = mul(Output.Pos, Proj);
#endif // ENABLE_WVP_TRANSFORM
#if ENABLE_VERTEX_COLOR
    Output.RGBA = Input.RGBA;
#endif // ENABLE_VERTEX_COLOR
#if ENABLE_VERTEX_TEXTURE
    Output.TexUV = Input.TexUV;
#endif // ENABLE_VERTEX_TEXTURE
#if ENABLE_VERTEX_NORMAL
    float4 Normal = float4(Input.Normal, 1.0);
    Normal = mul(Normal, World);
    Normal = mul(Normal, View);
    Normal = mul(Normal, Proj);
    Output.Normal = Input.Normal.xyz;
#endif // ENABLE_VERTEX_NORMAL
#if ENABLE_UNICOLOR
    Output.Unicolor = Unicolor;
#endif // ENABLE_UNICOLOR
    return Output;
}

float4 PSMain(VS_OUTPUT Input) : SV_Target
{
#if ENABLE_VERTEX_NORMAL
    return float4(Input.Normal, 1.0);
#else // ENABLE_VERTEX_NORMAL
#endif // ENABLE_VERTEX_NORMAL
#if ENABLE_VERTEX_COLOR
    return Input.RGBA;
#endif // ENABLE_VERTEX_COLOR
#if ENABLE_VERTEX_TEXTURE
    return MainTexture.Sample(MainSampler, Input.TexUV);
#endif // ENABLE_VERTEX_TEXTURE
#if ENABLE_UNICOLOR
    return Input.Unicolor;
#endif // ENABLE_UNICOLOR
}


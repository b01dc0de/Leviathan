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
#ifndef OUTPUT_NORMAL_COLOR
    #define OUTPUT_NORMAL_COLOR (0)
#endif // OUTPUT_NORMAL_COLOR

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
    Output.Normal = mul(float4(Input.Normal, 0.0), World).xyz;
#endif // ENABLE_VERTEX_NORMAL
#if ENABLE_UNICOLOR
    Output.Unicolor = Unicolor;
#endif // ENABLE_UNICOLOR
    return Output;
}

float4 SimpleShading(float4 InColor, float3 Normal)
{
    float ShadowIntensity = 0.75;
    float LightCoeff = dot(normalize(Normal), float3(0, 1, 0)) * 0.5 + 0.5;
    return lerp(InColor * (1 - ShadowIntensity), InColor, float4(LightCoeff.xxx, 1));
}

float4 PSMain(VS_OUTPUT Input) : SV_Target
{
#if ENABLE_VERTEX_COLOR
    float4 OutColor = Input.RGBA;
#endif // ENABLE_VERTEX_COLOR
#if ENABLE_VERTEX_TEXTURE
    float4 OutColor = MainTexture.Sample(MainSampler, Input.TexUV);
#endif // ENABLE_VERTEX_TEXTURE
#if ENABLE_UNICOLOR
    float4 OutColor = Input.Unicolor;
#endif // ENABLE_UNICOLOR
#if ENABLE_VERTEX_NORMAL && OUTPUT_NORMAL_COLOR
    OutColor = float4(Input.Normal, 1.0);
#endif // ENABLE_VERTEX_NORMAL && OUTPUT_NORMAL_COLOR

    // Process normal shading
#define ENABLE_SIMPLE_SHADING (1 && ENABLE_VERTEX_NORMAL)
#if ENABLE_SIMPLE_SHADING
    OutColor = SimpleShading(OutColor, Input.Normal);
#endif // ENABLE_SIMPLE_SHADING
    return OutColor;
}


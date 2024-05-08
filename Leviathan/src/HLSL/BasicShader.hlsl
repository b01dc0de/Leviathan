//////////////////////
// BasicShader.hlsl //
//////////////////////
#pragma pack_matrix( row_major )

// Defines:
#ifndef ENABLE_WVP_TRANSFORM
    #define ENABLE_WVP_TRANSFORM 1
#endif
#ifndef COMBINED_WVP_BUFFER
    #define COMBINED_WVP_BUFFER 1
#endif
#ifndef ENABLE_VERTEX_TEXTURE
    #define ENABLE_VERTEX_TEXTURE 0
#endif // ENABLE_VERTEX_TEXTURE
#ifndef ENABLE_VERTEX_COLOR
    #define ENABLE_VERTEX_COLOR 0
#endif // ENABLE_VERTEX_COLOR

#if !ENABLE_VERTEX_COLOR && !ENABLE_VERTEX_TEXTURE
    #error "Either VertexTexture or VertexColor must be enabled"
#endif

#if COMBINED_WVP_BUFFER
    cbuffer WorldViewProjBuffer : register(b0)
    {
        matrix World;
        matrix View;
        matrix Proj;
    }
#else
    cbuffer WorldBuffer : register(b0)
    {
        matrix World;
    }
    cbuffer ViewProjBuffer : register(b1)
    {
        matrix View;
        matrix Proj;
    }
#endif

#if ENABLE_VERTEX_TEXTURE
    Texture2D MainTexture : register(t0);
    SamplerState MainSampler : register(s0)
#endif

struct VS_INPUT
{
    float4 Pos : POSITION;
#if ENABLE_VERTEX_TEXTURE
    float2 TexUV : TEXCOORD;
#elif ENABLE_VERTEX_COLOR
    float4 RGBA : COLOR;
#endif
};

struct VS_OUTPUT
{
    float4 Pos : SV_POSITION;
#if ENABLE_VERTEX_TEXTURE
    float2 TexUV : TEXCOORD;
#elif ENABLE_VERTEX_COLOR
    float4 RGBA : COLOR;
#endif
};

VS_OUTPUT VSMain(VS_INPUT Input)
{
    VS_OUTPUT Output;
#if ENABLE_WVP_TRANSFORM
    Output.Pos = mul(Input.Pos, World);
    Output.Pos = mul(Output.Pos, View);
    Output.Pos = mul(Output.Pos, Proj);
#else
    Output.Pos = Input.Pos;
#endif // WVP_TRANSFORM
#if ENABLE_VERTEX_TEXTURE
    Output.Tex = Input.Tex;
#elif ENABLE_VERTEX_COLOR
    Output.RGBA = Input.RGBA;
#endif
    return Output;
}

float4 PSMain(VS_OUTPUT Input) : SV_Target
{
#if ENABLE_VERTEX_TEXTURE
    return MainTexture.Sample(MainSampler, Input.Tex);
#elif ENABLE_VERTEX_COLOR
    return Input.RGBA;
#else
    #error "Either VertexTexture or VertexColor must be enabled"
#endif
}


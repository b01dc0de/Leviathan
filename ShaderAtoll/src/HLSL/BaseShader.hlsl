/////////////////////
// BaseShader.hlsl //
/////////////////////

#include "AtollMain.hlsl"

PS_INPUT VSMain(VS_INPUT Input)
{
    PS_INPUT Output;
    Output.Pos = Input.Pos;
#if ENABLE_VERTEX_TEXTURE
    Output.TexUV = Input.TexUV;
#elif ENABLE_VERTEX_COLOR
    Output.RGBA = Input.RGBA;
#endif //ENABLE_VERTEX_COLOR
    return Output;
}

float4 PSMain(PS_INPUT Input) : SV_Target
{
#if ENABLE_VERTEX_TEXTURE
    return MainTexture.Sample(MainSampler, Input.TexUV);
#elif ENABLE_VERTEX_COLOR
    return Input.RGBA;
#else
    return AtollMain(Input.Pos);
#endif
}

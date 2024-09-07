///////////////////
// Example1.hlsl //
///////////////////

float4 Main_Example1(PS_INPUT Input)
{
    return (DebugTexture.Sample(MainSampler, Input.TexUV) + Example_Gradient(Input.Pos)) / 0.5f;
}


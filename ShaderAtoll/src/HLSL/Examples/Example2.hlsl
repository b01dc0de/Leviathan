///////////////////
// Example2.hlsl //
///////////////////

float4 Main_Example2(float4 ScreenPos)
{
    return 1.0 - Example_Gradient(ScreenPos);
}


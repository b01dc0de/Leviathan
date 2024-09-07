///////////////////
// Example2.hlsl //
///////////////////

float4 Main_Example2(PS_INPUT Input)
{
    return 1.0 - Example_Gradient(Input.Pos);
}


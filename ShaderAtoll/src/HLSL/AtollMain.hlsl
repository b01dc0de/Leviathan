/////////////////////
// AtollMain.hlsl //
/////////////////////

#include "ShaderCommon.hlsl"

#ifndef SELECT_ERROR_SHADER
    #define SELECT_ERROR_SHADER 0
#endif
#ifndef SELECT_LIVE_SHADER
    #define SELECT_LIVE_SHADER 0
#endif
#ifndef SELECT_EXAMPLE_SHADER
    #define SELECT_EXAMPLE_SHADER -1
#endif

#include "ErrorShader.hlsl"
#if SELECT_LIVE_SHADER
    #include "LiveShader_Wrapper.hlsl"
#else
    #include "Examples/AllExamples.hlsl"
#endif // SELECT_LIVE_SHADER

float4 AtollMain(float4 ScreenPos)
{
#if SELECT_ERROR_SHADER
    return Main_Error(ScreenPos);
#elif SELECT_LIVE_SHADER
    return Main_Live(ScreenPos);
#else // SELECT_EXAMPLE_SHADER
    #if SELECT_EXAMPLE_SHADER == 1
        return Main_Example1(ScreenPos);
    #elif SELECT_EXAMPLE_SHADER == 2
        return Main_Example2(ScreenPos);
    #elif SELECT_EXAMPLE_SHADER == 3
        return Main_Example3(ScreenPos);
    #elif SELECT_EXAMPLE_SHADER == 4
        return Main_Example4(ScreenPos);
    #elif SELECT_EXAMPLE_SHADER == 5
        return Main_Example5(ScreenPos);
    #elif SELECT_EXAMPLE_SHADER == 6
        return Main_Example6(ScreenPos);
    #elif SELECT_EXAMPLE_SHADER == 7
        return Main_Example7(ScreenPos);
    #elif SELECT_EXAMPLE_SHADER == 8
        return Main_Example8(ScreenPos);
    #elif SELECT_EXAMPLE_SHADER == 9
        return Main_Example9(ScreenPos);
    #elif SELECT_EXAMPLE_SHADER == 0
        return Main_Example0(ScreenPos);
    #else
        return Main_Error(ScreenPos);
    #endif // SELECT_EXAMPLE_SHADER
#endif // SELECT_SHADER
}


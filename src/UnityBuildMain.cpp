#define BUILD_UNITY (1)

#ifndef BUILD_UNITY
    #define BUILD_UNITY (0)
#endif // ifndef UNITY_BUILD

#if BUILD_UNITY
    #include "Camera.cpp"
    #include "Clock.cpp"
    #include "Draw2D.cpp"
    #include "DrawState.cpp"
    #include "Engine.cpp"
    #include "Fonts.cpp"
    #include "GameEntity.cpp"
    #include "Graphics.cpp"
    #include "Image.cpp"
    #include "Input.cpp"
    #include "InputVisualizer.cpp"
    #include "Math.cpp"
    #include "MeshState.h"
    #include "PerformanceGraph.cpp"
    #include "UserInterface.cpp"
    #include "Utils.cpp"
    #include "Game/GameManager.cpp"
    #include "Leviathan.cpp"

    #include "Game/BulletLimbo.cpp"
    #include "Game/Tetris.cpp"
    #include "Game/Snake.cpp"
#endif // BUILD_UNITY()


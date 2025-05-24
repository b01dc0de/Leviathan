#define BUILD_UNITY (1)

#ifndef BUILD_UNITY
    #define BUILD_UNITY (0)
#endif // ifndef UNITY_BUILD

#if BUILD_UNITY
    #include "AssetProcessor.cpp"
    #include "BatchDraw.cpp"
    #include "Camera.cpp"
    #include "Clock.cpp"
    #include "DrawState.cpp"
    #include "Engine.cpp"
    #include "Fonts.cpp"
    #include "GameEntity.cpp"
    #include "Graphics.cpp"
    #include "Image.cpp"
    #include "Input.cpp"
    #include "InputVisualizer.cpp"
    #include "Math.cpp"
    #include "MeshState.cpp"
    #include "PerformanceGraph.cpp"
    #include "UserInterface.cpp"
    #include "Utils.cpp"
    #include "Game/GameManager.cpp"
    // Main:
    #include "Leviathan.cpp"

    #include "Game/BulletLimbo.cpp"
    #include "Game/Labyrinth.cpp"
    #include "Game/Snake.cpp"
    #include "Game/SpeedCube.cpp"
    #include "Game/Tetris.cpp"
#endif // BUILD_UNITY()


#include "Graphics.h"
#include "Camera.h"
#include "DrawState.h"
#include "Image.h"
#include "InputVisualizer.h"
#include "UserInterface.h"

#include "Game/GameManager.h"

namespace Leviathan
{
    inline namespace GraphicsState
    {
        IDXGIFactory2* DXGI_Factory2 = nullptr;
        ID3D11Device* DX_Device = nullptr;
        ID3D11DeviceContext* DX_ImmediateContext = nullptr;
        IDXGISwapChain1* DXGI_SwapChain1 = nullptr;
        ID3D11Texture2D* DX_Backbuffer = nullptr;
        ID3D11RenderTargetView* DX_RenderTargetView = nullptr;
        ID3D11RasterizerState* DX_RasterizerState = nullptr;
        ID3D11Texture2D* DX_DepthStencil = nullptr;
        ID3D11DepthStencilView* DX_DepthStencilView = nullptr;
        ID3D11DepthStencilState* DX_DefaultDepthStencilState = nullptr;
        ID3D11DepthStencilState* DX_Draw2DDepthStencilState = nullptr;
        ID3D11BlendState* DX_DefaultBlendState = nullptr;
        ID3D11BlendState* DX_AlphaBlendState = nullptr;

        DrawStateT DrawStateColor;
        DrawStateT DrawStateTexture;
        DrawStateT DrawStateInstRectColor;
        DrawStateT DrawStateInstRectTexture;
        DrawStateT DrawStateInstLine;

        ID3D11Buffer* DX_WBuffer = nullptr;
        ID3D11Buffer* DX_VPBuffer = nullptr;
        const int WBufferSlot = 0;
        const int VPBufferSlot = 1;

        MeshStateT MeshStateTriangle;
        MeshStateT MeshStateCube;
        MeshStateT MeshStateQuad;
        MeshInstStateT MeshInstStateRect;
        MeshInstStateT MeshInstStateLine;

        LvTexture2D LvDebugTexture;
        LvTexture2D LvTestTexture;
        ID3D11SamplerState* DX_DefaultSamplerState = nullptr;

        D3D_FEATURE_LEVEL UsedFeatureLevel;

        // Direct2D:
        ID2D1Factory* D2_Factory = nullptr;
        IDXGISurface* DXGI_Surface = nullptr;
        ID2D1RenderTarget* D2_RenderTarget = nullptr;
    }

#define DX_UUID_HELPER(Type, Ptr) __uuidof(Type), (void**)&Ptr

    struct VxMin
    {
        v4f Pos;
    };

    struct VxColor
    {
        v4f Pos;
        v4f Col;
    };

    struct VxTexture
    {
        v4f Pos;
        v2f TexUV;
    };

    struct HandmadeTextSheet
    {
        LvTexture2D LvTex2D;
        int NumGlyphsX;
        int NumGlyphsY;
        v2f GlyphSize;
        RectF* GlyphRects;

        void Init(ID3D11Device* InDevice, const char* Filename, int InNumGlyphsX, int InNumGlyphsY);
        void Draw(BatchDraw2D& Draw2D, v2f Origin, float Scale, const char* Msg, int MsgLength);
        RectF GetGlyph(char GlyphChar);
        void Term();
    };

    void HandmadeTextSheet::Init(ID3D11Device* InDevice, const char* Filename, int InNumGlyphsX, int InNumGlyphsY)
    {
        *this = {};
        NumGlyphsX = InNumGlyphsX;
        NumGlyphsY = InNumGlyphsY;

        LvTex2D = LoadTextureBMP(Filename, InDevice);
        if (LvTex2D.Texture)
        {
            GlyphSize = {1.0f / NumGlyphsX, 1.0f / NumGlyphsY};
            GlyphRects = new RectF[NumGlyphsX * NumGlyphsY];
            int GlyphIdx = 0;
            for (int RowIdx = 0; RowIdx < NumGlyphsY; RowIdx++)
            {
                for (int ColIdx = 0; ColIdx < NumGlyphsX; ColIdx++, GlyphIdx++)
                {
                    GlyphRects[GlyphIdx] = RectF
                    {
                        GlyphSize.X * ColIdx, GlyphSize.Y * RowIdx,
                        GlyphSize.X, GlyphSize.Y
                    };
                }
            }
        }
    }

    void HandmadeTextSheet::Draw(BatchDraw2D& Draw2D, v2f Origin, float Scale, const char* Msg, int MsgLength)
    {
        v2f TextPos = Origin;
        const v2f TextSize{ GlyphSize.X * Scale, GlyphSize.Y * Scale };
        const float Spacing = 0.01f * Scale;
        for (int MsgCharIdx = 0; MsgCharIdx < MsgLength; MsgCharIdx++)
        {
            RectF TextRect{ TextPos.X, TextPos.Y, TextSize.X, TextSize.Y };
            Draw2D.AddTextRect(TextRect, GetGlyph(Msg[MsgCharIdx]));
            TextPos.X += TextSize.X + Spacing;
        }
    }

    RectF HandmadeTextSheet::GetGlyph(char GlyphChar)
    {
        constexpr RectF InvalidRect{ 1.0f, 1.0f, 0.0f, 0.0f };
        RectF Result = InvalidRect;
        if (!GlyphRects) { return Result; }
        switch (GlyphChar)
        {
            case 'A':
            case 'a': { Result = GlyphRects[0]; } break;
            case 'B':
            case 'b': { Result = GlyphRects[1]; } break;
            case 'C':
            case 'c': { Result = GlyphRects[2]; } break;
            case 'D':
            case 'd': { Result = GlyphRects[3]; } break;
            case 'E':
            case 'e': { Result = GlyphRects[4]; } break;
            case 'F':
            case 'f': { Result = GlyphRects[5]; } break;
            case 'G':
            case 'g': { Result = GlyphRects[6]; } break;
            case 'H':
            case 'h': { Result = GlyphRects[7]; } break;
            case 'I':
            case 'i': { Result = GlyphRects[8]; } break;
            case 'J':
            case 'j': { Result = GlyphRects[9]; } break;
            case 'K':
            case 'k': { Result = GlyphRects[10]; } break;
            case 'L':
            case 'l': { Result = GlyphRects[11]; } break;
            case 'M':
            case 'm': { Result = GlyphRects[12]; } break;
            case 'N':
            case 'n': { Result = GlyphRects[13]; } break;
            case 'O':
            case 'o': { Result = GlyphRects[14]; } break;
            case 'P':
            case 'p': { Result = GlyphRects[15]; } break;
            case 'Q':
            case 'q': { Result = GlyphRects[16]; } break;
            case 'R':
            case 'r': { Result = GlyphRects[17]; } break;
            case 'S':
            case 's': { Result = GlyphRects[18]; } break;
            case 'T':
            case 't': { Result = GlyphRects[19]; } break;
            case 'U':
            case 'u': { Result = GlyphRects[20]; } break;
            case 'V':
            case 'v': { Result = GlyphRects[21]; } break;
            case 'W':
            case 'w': { Result = GlyphRects[22]; } break;
            case 'X':
            case 'x': { Result = GlyphRects[23]; } break;
            case 'Y':
            case 'y': { Result = GlyphRects[24]; } break;
            case 'Z':
            case 'z': { Result = GlyphRects[25]; } break;
            // NOTE: Version 1
            case '0': { Result = GlyphRects[26]; } break;
            case '1': { Result = GlyphRects[27]; } break;
            case '2': { Result = GlyphRects[28]; } break;
            case '3': { Result = GlyphRects[29]; } break;
            case '4': { Result = GlyphRects[30]; } break;
            case '5': { Result = GlyphRects[31]; } break;
            case '6': { Result = GlyphRects[32]; } break;
            case '7': { Result = GlyphRects[33]; } break;
            case '8': { Result = GlyphRects[34]; } break;
            case '9': { Result = GlyphRects[35]; } break;
            // NOTE: Version 0
            //case '1': { Result = GlyphRects[26]; } break;
            //case '2': { Result = GlyphRects[27]; } break;
            //case '3': { Result = GlyphRects[28]; } break;
            //case '4': { Result = GlyphRects[29]; } break;
            //case '5': { Result = GlyphRects[30]; } break;
            //case '6': { Result = GlyphRects[31]; } break;
            //case '7': { Result = GlyphRects[32]; } break;
            //case '8': { Result = GlyphRects[33]; } break;
            //case '9': { Result = GlyphRects[34]; } break;
            // case '0': { Result = GlyphRects[35]; } break;
            case ' ': { Result = GlyphRects[36]; } break;
            default: { } break;
        }
        return Result;
    }

    void HandmadeTextSheet::Term()
    {
        SafeRelease(LvTex2D);
        delete[] GlyphRects;
        *this = {};
    }

    VxColor Vertices_Triangle[] =
    {
        { {+0.0f, +0.5f, +0.5f, 1.0f}, {0.0f, 1.0f, 1.0f, 1.0f} },
        { {-0.5f, -0.5f, +0.5f, 1.0f}, {1.0f, 0.0f, 1.0f, 1.0f} },
        { {+0.5f, -0.5f, +0.5f, 1.0f}, {1.0f, 1.0f, 0.0f, 1.0f} },
    };
    unsigned int Indices_Triangle[] =
    {
        0, 1, 2
    };

    constexpr float fUnit = 1.0f;
    VxColor Vertices_Cube[] =
    {
        {{-fUnit, +fUnit, +fUnit, 1.0f}, {1.0f, 0.0f, 0.0f, 1.0f}}, // 0
        {{+fUnit, +fUnit, +fUnit, 1.0f}, {0.0f, 1.0f, 0.0f, 1.0f}}, // 1
        {{-fUnit, -fUnit, +fUnit, 1.0f}, {0.0f, 0.0f, 1.0f, 1.0f}}, // 2
        {{+fUnit, -fUnit, +fUnit, 1.0f}, {1.0f, 1.0f, 1.0f, 1.0f}}, // 3

        {{-fUnit, +fUnit, -fUnit, 1.0f}, {0.0f, 1.0f, 1.0f, 1.0f}}, // 4
        {{+fUnit, +fUnit, -fUnit, 1.0f}, {1.0f, 0.0f, 1.0f, 1.0f}}, // 5
        {{-fUnit, -fUnit, -fUnit, 1.0f}, {1.0f, 1.0f, 0.0f, 1.0f}}, // 6
        {{+fUnit, -fUnit, -fUnit, 1.0f}, {0.0f, 0.0f, 0.0f, 1.0f}}, // 7
    };
    unsigned int Indices_Cube[] =
    {
        // Front
        0, 2, 1,
        1, 2, 3,
        // Back
        5, 7, 4,
        4, 7, 6,
        // Top
        4, 0, 5,
        5, 0, 1,
        // Bottom
        2, 6, 3,
        3, 6, 7,
        // Left
        4, 6, 0,
        0, 6, 2,
        // Right
        1, 3, 5,
        5, 3, 7,
    };

    VxTexture Vertices_Rect[] =
    {
        { {-0.5f, +0.5f, +0.5f, +1.0f}, { 0.0f, 0.0f } },
        { {+0.5f, +0.5f, +0.5f, +1.0f}, { 1.0f, 0.0f } },
        { {-0.5f, -0.5f, +0.5f, +1.0f}, { 0.0f, 1.0f } },
        { {+0.5f, -0.5f, +0.5f, +1.0f}, { 1.0f, 1.0f } },
    };
    VxMin Vertices_PlatonicRect[] =
    {
        { { 0.0f, 1.0f, +0.5f, +1.0f } },
        { { 1.0f, 1.0f, +0.5f, +1.0f } },
        { { 0.0f, 0.0f, +0.5f, +1.0f } },
        { { 1.0f, 0.0f, +0.5f, +1.0f } },
    };
    unsigned int Indices_Rect[] =
    {
        0, 2, 1,
        1, 2, 3
    };

    struct SpriteTransform
    {
        v2f Scale{1.0f, 1.0f};
        v2f Pos{0.0f, 0.0f};
        float RotAngle = 0.0f;
    };

    const float TestInstRectSize = 50.0f;
    const float RightRectX = (float)AppWidth - TestInstRectSize;
    const float BottomRectY = (float)AppHeight - TestInstRectSize;
    InstRectTextureData InstRectTextureDataArray[] =
    {
        { {0.0f, BottomRectY, TestInstRectSize, TestInstRectSize}, {0.0f, 0.0f, 0.5f, 0.5f} },
        { {RightRectX, BottomRectY, TestInstRectSize, TestInstRectSize}, {0.5f, 0.0f, 0.5f, 0.5f} },
        { {0.0f, 0.0f, TestInstRectSize, TestInstRectSize}, { 0.0f, 0.5f, 0.5f, 0.5f } },
        { {RightRectX, 0.0f, TestInstRectSize, TestInstRectSize}, {0.5f, 0.5f, 0.5f, 0.5f} }
    };

    float HalfWidth = (float)AppWidth / 2.0f;
    float HalfHeight = (float)AppHeight / 2.0f;
    InstLineData InstData_Lines[]
    {
        { {0.0f, 0.0f, AppWidth, AppHeight}, {1.0f, 0.0f, 0.0f, 1.0f} },
        { {0.0f, AppHeight, AppWidth, 0.0f}, {0.0f, 1.0f, 0.0f, 1.0f} },
        { {HalfWidth, 0.0f, HalfWidth, AppHeight}, {0.0f, 0.0f, 1.0f, 1.0f} },
        { {0.0f, HalfHeight, AppWidth, HalfHeight}, {1.0f, 1.0f, 1.0f, 1.0f} },
    };
    VxMin Vertices_PlatonicLine[] =
    {
        {0.0f, 0.0f, +0.5f, 1.0f},
        {1.0f, 1.0f, +0.5f, 1.0f}
    };

    m4f GetMatrix(const SpriteTransform& InSprTrans)
    {
        return m4f::Scale(InSprTrans.Scale.X, InSprTrans.Scale.Y, 1.0f) * m4f::RotAxisZ(InSprTrans.RotAngle) * m4f::Trans(InSprTrans.Pos.X, InSprTrans.Pos.Y, 0.0f);
    }

    Camera OrthoCamera;
    Camera GameCamera;
    BatchDraw2D Draw2D;
    HandmadeTextSheet HandmadeText;

    void UpdateShaderWorld(ID3D11DeviceContext* Context, m4f* WorldData)
    {
        ID3D11Buffer* WorldBuffer = DX_WBuffer;
        UpdateShaderResource(Context, WorldBuffer, WorldData, sizeof(m4f));
    }
    void UpdateShaderViewProj(ID3D11DeviceContext* Context, Camera* CameraData)
    {
        ID3D11Buffer* ViewProjBuffer = DX_VPBuffer;
        UpdateShaderResource(Context, ViewProjBuffer, CameraData, sizeof(Camera));
    }

    constexpr int DefaultSize_BatchDraw2D = 1024;
    void Graphics::Draw()
    {
        Draw2D.Clear();

        DX_ImmediateContext->RSSetState(DX_RasterizerState);
        constexpr UINT DefaultSampleMask = 0xFFFFFFFF;
        DX_ImmediateContext->OMSetBlendState(nullptr, nullptr, DefaultSampleMask);
        DX_ImmediateContext->OMSetDepthStencilState(nullptr, 0);
        DX_ImmediateContext->OMSetRenderTargets(1, &DX_RenderTargetView, DX_DepthStencilView);
        v4f ClearColor = { 30.0f / 255.0f, 30.0f / 255.0f, 45.0f / 255.0f, 1.0f };
        float fClearDepth = 1.0f;
        DX_ImmediateContext->ClearRenderTargetView(DX_RenderTargetView, &ClearColor.X);
        DX_ImmediateContext->ClearDepthStencilView(DX_DepthStencilView, D3D11_CLEAR_DEPTH, fClearDepth, 0);
        m4f DefaultSpriteWorld = m4f::Trans(-HalfWidth, -HalfHeight, 0.0f);

        ASSERT(sizeof(InstRectColorData) == sizeof(InstRectTextureData));
        ASSERT(sizeof(InstRectColorData) == sizeof(InstLineData));

        ID3D11Buffer* World_ViewProjBuffers[] =
        {
            DX_WBuffer,
            DX_VPBuffer
        };
        ID3D11SamplerState* DefaultSampler[] =
        {
            DX_DefaultSamplerState
        };
        ID3D11ShaderResourceView* TestTextureSRV[] =
        {
            LvTestTexture.SRV
        };
        ID3D11ShaderResourceView* HandmadeFontTextureSRV[] =
        {
            HandmadeText.LvTex2D.SRV
        };

        SetShaderSamplers(DX_ImmediateContext, ARRAY_SIZE(DefaultSampler), DefaultSampler);

        static bool bDrawInstLines = true;
        if (bDrawInstLines)
        {
            DX_ImmediateContext->OMSetDepthStencilState(DX_Draw2DDepthStencilState, 0);
            DX_ImmediateContext->OMSetBlendState(DX_AlphaBlendState, nullptr, 0xFFFFFFFF);
            UpdateShaderWorld(DX_ImmediateContext, &DefaultSpriteWorld);
            UpdateShaderViewProj(DX_ImmediateContext, &OrthoCamera);

            for (int Idx = 0; Idx < ARRAY_SIZE(InstData_Lines); Idx++)
            {
                Draw2D.AddLine(InstData_Lines[Idx].Line, InstData_Lines[Idx].Color);
            }

            SetShaderConstantBuffers(DX_ImmediateContext, ARRAY_SIZE(World_ViewProjBuffers), World_ViewProjBuffers);

            DrawMeshInstanced
            (
                DX_ImmediateContext,
                DrawStateInstLine,
                MeshInstStateLine,
                Draw2D.LineBatchCmds.Num,
                Draw2D.LineBatchCmds.Data
            );
        }

        static bool bDrawTriangle = true;
        if (bDrawTriangle)
        { // Draw triangle
            DX_ImmediateContext->OMSetBlendState(nullptr, nullptr, DefaultSampleMask);
            m4f TriangleWorld = m4f::Scale(128.0f, 128.0f, 1.0f) * m4f::Trans(256.0f, -256.0f, 0.0f);
            UpdateShaderWorld(DX_ImmediateContext, &TriangleWorld);
            UpdateShaderViewProj(DX_ImmediateContext, &OrthoCamera);

            SetShaderConstantBuffers(DX_ImmediateContext, ARRAY_SIZE(World_ViewProjBuffers), World_ViewProjBuffers);

            DrawMesh(DX_ImmediateContext, DrawStateColor, MeshStateTriangle);
        }

        static bool bDrawTexQuad = true;
        if (bDrawTexQuad)
        { // Draw tex quad
            DX_ImmediateContext->OMSetBlendState(DX_AlphaBlendState, nullptr, 0xFFFFFFFF);
            m4f TexQuadWorld = m4f::Scale(100.0f, 100.0f, 1.0f) * m4f::Trans(+256.0f, +256.0f, 0.0f);
            UpdateShaderWorld(DX_ImmediateContext, &TexQuadWorld);
            UpdateShaderViewProj(DX_ImmediateContext, &OrthoCamera);

            SetShaderConstantBuffers(DX_ImmediateContext, ARRAY_SIZE(World_ViewProjBuffers), World_ViewProjBuffers);
            SetShaderResourceViews(DX_ImmediateContext, ARRAY_SIZE(TestTextureSRV), TestTextureSRV);

            DrawMesh(DX_ImmediateContext, DrawStateTexture, MeshStateQuad);
        }
        DX_ImmediateContext->OMSetDepthStencilState(DX_Draw2DDepthStencilState, 0);

        const float HalfWidth = (float)AppWidth / 2.0f;
        const float HalfHeight = (float)AppHeight / 2.0f;
        static bool bDrawInstRects = true;
        if (bDrawInstRects)
        { // Draw Instanced Rects
            DX_ImmediateContext->OMSetBlendState(DX_AlphaBlendState, nullptr, 0xFFFFFFFF);
            UpdateShaderWorld(DX_ImmediateContext, &DefaultSpriteWorld);
            UpdateShaderViewProj(DX_ImmediateContext, &OrthoCamera);

            SetShaderConstantBuffers(DX_ImmediateContext, ARRAY_SIZE(World_ViewProjBuffers), World_ViewProjBuffers);
            SetShaderResourceViews(DX_ImmediateContext, ARRAY_SIZE(TestTextureSRV), TestTextureSRV);

            DrawMeshInstanced
            (
                DX_ImmediateContext,
                DrawStateInstRectTexture,
                MeshInstStateRect,
                ARRAY_SIZE(InstRectTextureDataArray),
                InstRectTextureDataArray
            );
        }

        static bool bDrawGame = true;
        // Get instanced draw commands from game:
        if (bDrawGame) { GameManager::Draw(Draw2D); }
        if (Draw2D.ColorBatchCmds.Num > 0)
        {
            DX_ImmediateContext->OMSetDepthStencilState(DX_Draw2DDepthStencilState, 0);
            DX_ImmediateContext->OMSetBlendState(nullptr, nullptr, 0xFFFFFFFF);

            UpdateShaderWorld(DX_ImmediateContext, &DefaultSpriteWorld);
            UpdateShaderViewProj(DX_ImmediateContext, &OrthoCamera);

            SetShaderConstantBuffers(DX_ImmediateContext, ARRAY_SIZE(World_ViewProjBuffers), World_ViewProjBuffers);

            DrawMeshInstanced
            (
                DX_ImmediateContext,
                DrawStateInstRectColor,
                MeshInstStateRect,
                Draw2D.ColorBatchCmds.Num,
                Draw2D.ColorBatchCmds.Data
            );
        }

        static bool bDrawCube = true;
        if (bDrawCube)
        { // Draw cube
            DX_ImmediateContext->OMSetBlendState(nullptr, nullptr, 0xFFFFFFFF);

            static float RotationX = 0.0f;
            static float RotationY = 0.0f;
            static constexpr float RotSpeed = (1.0f / 60.0f) / 10.0f;
            RotationX += RotSpeed;
            RotationY += RotSpeed * 0.5f;
            m4f CubeWorld = m4f::RotAxisX(RotationX) * m4f::RotAxisY(RotationY);
            UpdateShaderWorld(DX_ImmediateContext, &CubeWorld);
            UpdateShaderViewProj(DX_ImmediateContext, &GameCamera);

            SetShaderConstantBuffers(DX_ImmediateContext, ARRAY_SIZE(World_ViewProjBuffers), World_ViewProjBuffers);

            DrawMesh(DX_ImmediateContext, DrawStateColor, MeshStateCube);
        }

        Draw2D.Clear();

        static bool bDrawUI = true;
        if (bDrawUI) { UserInterface::Draw(D2_RenderTarget, Draw2D); }
        if (Draw2D.ColorBatchCmds.Num > 0)
        {
            DX_ImmediateContext->OMSetBlendState(nullptr, nullptr, 0xFFFFFFFF);
            DX_ImmediateContext->OMSetDepthStencilState(DX_Draw2DDepthStencilState, 0);
            UpdateShaderWorld(DX_ImmediateContext, &DefaultSpriteWorld);
            UpdateShaderViewProj(DX_ImmediateContext, &OrthoCamera);

            SetShaderConstantBuffers(DX_ImmediateContext, ARRAY_SIZE(World_ViewProjBuffers), World_ViewProjBuffers);
            SetShaderResourceViews(DX_ImmediateContext, ARRAY_SIZE(TestTextureSRV), TestTextureSRV);

            DrawMeshInstanced
            (
                DX_ImmediateContext,
                DrawStateInstRectColor,
                MeshInstStateRect,
                Draw2D.ColorBatchCmds.Num,
                Draw2D.ColorBatchCmds.Data
            );
        }

        static bool bDrawHandmadeText = true;
        if (bDrawHandmadeText)
        {
            DX_ImmediateContext->OMSetBlendState(DX_AlphaBlendState, nullptr, 0xFFFFFFFF);

            v2f TextOrigin{ AppWidth / 2.0f, AppHeight / 2.0f };
            const float TextScale = 300.0f;
            const char TextMsg[] = "HELLO WORLD";
            HandmadeText.Draw(Draw2D, TextOrigin, TextScale, TextMsg, ARRAY_SIZE(TextMsg) - 1);

            const char TextRow0[] = "ABCDEFGHIJKL";
            const char TextRow1[] = "MNOPQRSTUVWX";
            const char TextRow2[] = "YZ0123456789";
            TextOrigin.Y -= 60.0f;
            HandmadeText.Draw(Draw2D, TextOrigin, TextScale, TextRow0, ARRAY_SIZE(TextRow0) - 1);
            TextOrigin.Y -= 60.0f;
            HandmadeText.Draw(Draw2D, TextOrigin, TextScale, TextRow1, ARRAY_SIZE(TextRow1) - 1);
            TextOrigin.Y -= 60.0f;
            HandmadeText.Draw(Draw2D, TextOrigin, TextScale, TextRow2, ARRAY_SIZE(TextRow2) - 1);

            UpdateShaderWorld(DX_ImmediateContext, &DefaultSpriteWorld);
            UpdateShaderViewProj(DX_ImmediateContext, &OrthoCamera);

            SetShaderConstantBuffers(DX_ImmediateContext, ARRAY_SIZE(World_ViewProjBuffers), World_ViewProjBuffers);
            SetShaderResourceViews(DX_ImmediateContext, ARRAY_SIZE(HandmadeFontTextureSRV), HandmadeFontTextureSRV);

            DrawMeshInstanced
            (
                DX_ImmediateContext,
                DrawStateInstRectTexture,
                MeshInstStateRect,
                Draw2D.TextBatchCmds.Num,
                Draw2D.TextBatchCmds.Data
            );
        }

        DX_ImmediateContext->OMSetBlendState(nullptr, nullptr, 0xFFFFFFFF);

        UINT SyncInterval = 0, PresentFlags = 0;
        DXGI_PRESENT_PARAMETERS PresentParams = {};
        DXGI_SwapChain1->Present1(SyncInterval, PresentFlags, &PresentParams);
    }

    void Graphics::Init()
    {
        D3D_FEATURE_LEVEL SupportedFeatureLevels[] =
        {
            D3D_FEATURE_LEVEL_11_1,
            D3D_FEATURE_LEVEL_11_0,
        };
        UINT NumSupportedFeatureLevels = ARRAY_SIZE(SupportedFeatureLevels);

        CreateDXGIFactory1(DX_UUID_HELPER(IDXGIFactory2, DXGI_Factory2));

        UINT CreateDeviceFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
    #ifdef _DEBUG
        CreateDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
    #endif // _DEBUG
        DX_CHECK(D3D11CreateDevice
        (
            nullptr,
            D3D_DRIVER_TYPE_HARDWARE,
            nullptr,
            CreateDeviceFlags,
            SupportedFeatureLevels,
            NumSupportedFeatureLevels,
            D3D11_SDK_VERSION,
            &DX_Device,
            &UsedFeatureLevel,
            &DX_ImmediateContext
        ));

        DXGI_SAMPLE_DESC SampleDesc = {};
        SampleDesc.Count = 1;
        SampleDesc.Quality = 0;

        DXGI_SWAP_CHAIN_DESC1 SwapChainDesc1 = {};
        SwapChainDesc1.Width = AppWidth;
        SwapChainDesc1.Height = AppHeight;
        SwapChainDesc1.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        SwapChainDesc1.Stereo = FALSE;
        SwapChainDesc1.SampleDesc = SampleDesc;
        SwapChainDesc1.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        SwapChainDesc1.BufferCount = 2;
        SwapChainDesc1.Scaling = DXGI_SCALING_STRETCH;
        SwapChainDesc1.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
        SwapChainDesc1.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
        SwapChainDesc1.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING;

        DX_CHECK(DXGI_Factory2->CreateSwapChainForHwnd(
            DX_Device,
            AppWindow,
            &SwapChainDesc1,
            nullptr,
            nullptr,
            &DXGI_SwapChain1
        ));

        DX_CHECK(DXGI_SwapChain1->GetBuffer(0, DX_UUID_HELPER(ID3D11Texture2D, DX_Backbuffer)));

        DX_CHECK(DX_Device->CreateRenderTargetView(DX_Backbuffer, nullptr, &DX_RenderTargetView));

        D3D11_RASTERIZER_DESC RasterDesc = {};
        RasterDesc.FillMode = D3D11_FILL_SOLID;
        RasterDesc.CullMode = D3D11_CULL_BACK;
        RasterDesc.FrontCounterClockwise = TRUE;
        RasterDesc.DepthClipEnable = TRUE;
        RasterDesc.ScissorEnable = FALSE;
        RasterDesc.MultisampleEnable = TRUE;
        RasterDesc.AntialiasedLineEnable = TRUE;
        DX_CHECK(DX_Device->CreateRasterizerState(&RasterDesc, &DX_RasterizerState));

        D3D11_TEXTURE2D_DESC DepthDesc = {};
        DepthDesc.Width = AppWidth;
        DepthDesc.Height = AppHeight;
        DepthDesc.MipLevels = 1;
        DepthDesc.ArraySize = 1;
        DepthDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
        DepthDesc.SampleDesc = SampleDesc;
        DepthDesc.Usage = D3D11_USAGE_DEFAULT;
        DepthDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
        DepthDesc.CPUAccessFlags = 0;
        DepthDesc.MiscFlags = 0;
        DX_CHECK(DX_Device->CreateTexture2D(&DepthDesc, nullptr, &DX_DepthStencil));

        D3D11_DEPTH_STENCIL_VIEW_DESC DepthStencilViewDesc = {};
        DepthStencilViewDesc.Format = DepthDesc.Format;
        DepthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
        DepthStencilViewDesc.Texture2D.MipSlice = 0;
        DX_CHECK(DX_Device->CreateDepthStencilView(DX_DepthStencil, &DepthStencilViewDesc, &DX_DepthStencilView));

        D3D11_DEPTH_STENCIL_DESC Instanced2DDepthStencilStateDesc = {};
        Instanced2DDepthStencilStateDesc.DepthEnable = TRUE;
        Instanced2DDepthStencilStateDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
        Instanced2DDepthStencilStateDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
        Instanced2DDepthStencilStateDesc.StencilEnable = FALSE;
        DX_Device->CreateDepthStencilState(&Instanced2DDepthStencilStateDesc, &DX_Draw2DDepthStencilState);

        { // AlphaBlendState:
            D3D11_RENDER_TARGET_BLEND_DESC AlphaRTBlendDesc = {};
            AlphaRTBlendDesc.BlendEnable = TRUE;
            AlphaRTBlendDesc.SrcBlend = D3D11_BLEND_SRC_COLOR;
            AlphaRTBlendDesc.DestBlend = D3D11_BLEND_DEST_COLOR;
            AlphaRTBlendDesc.BlendOp = D3D11_BLEND_OP_ADD;
            AlphaRTBlendDesc.SrcBlendAlpha = D3D11_BLEND_SRC_ALPHA;
            AlphaRTBlendDesc.DestBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;
            AlphaRTBlendDesc.BlendOpAlpha = D3D11_BLEND_OP_ADD;
            AlphaRTBlendDesc.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
            D3D11_BLEND_DESC AlphaBlendDesc = {};
            AlphaBlendDesc.AlphaToCoverageEnable = FALSE;
            AlphaBlendDesc.IndependentBlendEnable = FALSE;
            AlphaBlendDesc.RenderTarget[0] = AlphaRTBlendDesc;
            
            DX_CHECK(DX_Device->CreateBlendState(&AlphaBlendDesc, &DX_AlphaBlendState));
        }

        D3D11_VIEWPORT ViewportDesc = {};
        ViewportDesc.TopLeftX = 0.0f;
        ViewportDesc.TopLeftY = 0.0f;
        ViewportDesc.Width = (FLOAT)AppWidth;
        ViewportDesc.Height = (FLOAT)AppHeight;
        ViewportDesc.MinDepth = 0.0f;
        ViewportDesc.MaxDepth = 1.0f;
        DX_ImmediateContext->RSSetViewports(1, &ViewportDesc);

        const wchar_t* BaseShaderFilename = L"src/hlsl/BaseShader.hlsl";
        {
            const D3D_SHADER_MACRO DefinesVxColor[] =
            {
                "ENABLE_VERTEX_COLOR", "1",
                "ENABLE_VERTEX_TEXTURE", "0",
                "ENABLE_WVP_TRANSFORM", "1",
                "ENABLE_UNICOLOR", "0",
                nullptr, nullptr
            };
            D3D11_INPUT_ELEMENT_DESC InputLayoutDesc[] =
            {
                { "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
                { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
            };

            DrawStateColor = CreateDrawState(DX_Device, BaseShaderFilename, DefinesVxColor, InputLayoutDesc, ARRAY_SIZE(InputLayoutDesc));
        }

        {
            const D3D_SHADER_MACRO DefinesVxTexture[] =
            {
                "ENABLE_VERTEX_COLOR", "0",
                "ENABLE_VERTEX_TEXTURE", "1",
                "ENABLE_WVP_TRANSFORM", "1",
                "ENABLE_UNICOLOR", "0",
                nullptr, nullptr
            };
            D3D11_INPUT_ELEMENT_DESC InputLayoutDesc[] =
            {
                { "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
                { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
            };

            DrawStateTexture = CreateDrawState(DX_Device, BaseShaderFilename, DefinesVxTexture, InputLayoutDesc, ARRAY_SIZE(InputLayoutDesc));
        }

        const wchar_t* InstRectShaderFilename = L"src/hlsl/InstRectShader.hlsl";
        { // InstRectColor
            const D3D_SHADER_MACRO DefinesInst[] =
            {
                "INST_RECT_COLOR", "1",
                "INST_RECT_TEXTURE", "0",
                nullptr, nullptr
            };
            D3D11_INPUT_ELEMENT_DESC InputLayoutDesc[] =
            {
                { "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
                { "RECT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
                { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 }
            };

            DrawStateInstRectColor = CreateDrawState(DX_Device, InstRectShaderFilename, DefinesInst, InputLayoutDesc, ARRAY_SIZE(InputLayoutDesc));
        }

        { // InstRectTexture
            const D3D_SHADER_MACRO DefinesInst[] =
            {
                "INST_RECT_COLOR", "0",
                "INST_RECT_TEXTURE", "1",
                nullptr, nullptr
            };
            D3D11_INPUT_ELEMENT_DESC InputLayoutDesc[] =
            {
                { "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
                { "RECT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
                { "TEXCOORD", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 }
            };

            DrawStateInstRectTexture = CreateDrawState(DX_Device, InstRectShaderFilename, DefinesInst, InputLayoutDesc, ARRAY_SIZE(InputLayoutDesc));
        }

        { // InstLine
            const wchar_t* InstLineShaderFilename = L"src/hlsl/InstLineShader.hlsl";
            const D3D_SHADER_MACRO* DefinesInst = nullptr;
            D3D11_INPUT_ELEMENT_DESC InputLayoutDesc[] =
            {
                { "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
                { "LINE", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
                { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 }
            };

            DrawStateInstLine = CreateDrawState(DX_Device, InstLineShaderFilename, DefinesInst, InputLayoutDesc, ARRAY_SIZE(InputLayoutDesc));
        }

        Draw2D.ColorBatchCmds.Reserve(DefaultSize_BatchDraw2D);
        Draw2D.TextureBatchCmds.Reserve(DefaultSize_BatchDraw2D);

        D3D11_BUFFER_DESC WorldBufferDesc = {};
        WorldBufferDesc.ByteWidth = sizeof(m4f);
        WorldBufferDesc.Usage = D3D11_USAGE_DEFAULT;
        WorldBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        WorldBufferDesc.CPUAccessFlags = 0;
        DX_CHECK(DX_Device->CreateBuffer(&WorldBufferDesc, nullptr, &DX_WBuffer));
        D3D11_BUFFER_DESC ViewProjBufferDesc = {};
        ViewProjBufferDesc.ByteWidth = sizeof(m4f) * 2;
        ViewProjBufferDesc.Usage = D3D11_USAGE_DEFAULT;
        ViewProjBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        ViewProjBufferDesc.CPUAccessFlags = 0;
        DX_CHECK(DX_Device->CreateBuffer(&ViewProjBufferDesc, nullptr, &DX_VPBuffer));

        MeshStateTriangle = CreateMeshState
        (
            DX_Device,
            sizeof(VxColor),
            ARRAY_SIZE(Vertices_Triangle),
            Vertices_Triangle,
            ARRAY_SIZE(Indices_Triangle),
            Indices_Triangle
        );
        MeshStateCube = CreateMeshState
        (
            DX_Device,
            sizeof(VxColor),
            ARRAY_SIZE(Vertices_Cube),
            Vertices_Cube,
            ARRAY_SIZE(Indices_Cube),
            Indices_Cube
        );

        MeshInstStateRect = CreateMeshInstState
        (
            DX_Device,
            sizeof(VxMin),
            sizeof(InstRectColorData),
            DefaultSize_BatchDraw2D,
            ARRAY_SIZE(Vertices_PlatonicRect),
            Vertices_PlatonicRect,
            ARRAY_SIZE(Indices_Rect),
            Indices_Rect
        );

        MeshInstStateLine = CreateMeshInstState
        (
            DX_Device,
            sizeof(VxMin),
            sizeof(InstLineData),
            DefaultSize_BatchDraw2D,
            ARRAY_SIZE(Vertices_PlatonicLine),
            Vertices_PlatonicLine,
            0,
            nullptr,
            D3D_PRIMITIVE_TOPOLOGY_LINELIST
        );

        { // Default sampler state:
            D3D11_TEXTURE_ADDRESS_MODE SamplerAddressMode = D3D11_TEXTURE_ADDRESS_WRAP;
            D3D11_SAMPLER_DESC DefaultSamplerDesc = {};
            DefaultSamplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
            DefaultSamplerDesc.AddressU = SamplerAddressMode;
            DefaultSamplerDesc.AddressV = SamplerAddressMode;
            DefaultSamplerDesc.AddressW = SamplerAddressMode;
            DefaultSamplerDesc.MipLODBias = 0.0f;
            DefaultSamplerDesc.MaxAnisotropy = 0;
            DefaultSamplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
            DefaultSamplerDesc.MinLOD = 0;
            DefaultSamplerDesc.MaxLOD = 0;
            DX_CHECK(DX_Device->CreateSamplerState(&DefaultSamplerDesc, &DX_DefaultSamplerState));
        }

        ImageT DebugImage = {};
        GetDebugImage(DebugImage);
        LvDebugTexture = LoadTextureFromImage(DebugImage, DX_Device);
        LvTestTexture = LoadTextureBMP("Assets/TestTexture.bmp", DX_Device);

        //HandmadeText.Init(DX_Device, "Assets/HandmadeTextFont_0.bmp", 12, 6);
        HandmadeText.Init(DX_Device, "Assets/HandmadeTextFont_1.bmp", 12, 6);

        MeshStateQuad = CreateMeshState
        (
            DX_Device,
            sizeof(VxTexture),
            ARRAY_SIZE(Vertices_Rect),
            Vertices_Rect,
            ARRAY_SIZE(Indices_Rect),
            Indices_Rect
        );

        { // Direct2D
            DX_CHECK(D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &D2_Factory));
            DX_CHECK(DXGI_SwapChain1->GetBuffer(0, DX_UUID_HELPER(IDXGISurface, DXGI_Surface)));
            D2D1_RENDER_TARGET_PROPERTIES D2_RT_Properties = D2D1::RenderTargetProperties(
                D2D1_RENDER_TARGET_TYPE_DEFAULT,
                D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED),
                0, 0
            );
            DX_CHECK(D2_Factory->CreateDxgiSurfaceRenderTarget(
                DXGI_Surface,
                &D2_RT_Properties,
                &D2_RenderTarget 
            ));
        }

        v3f CameraPos{5.0f, 5.0f, -5.0f};
        v3f CameraLookAt{ 0.0f, 0.0f, 0.0f };
        GameCamera.Persp(CameraPos, CameraLookAt);

        OrthoCamera.Ortho((float)AppWidth, (float)AppHeight, -2.0f);

        UserInterface::Init(D2_RenderTarget);
    }

    void Graphics::Term()
    {
        UserInterface::Term();

        { // Direct2D:
            SafeRelease(D2_Factory);
            SafeRelease(DXGI_Surface);
            SafeRelease(D2_RenderTarget);
        }

        SafeRelease(MeshStateTriangle);
        SafeRelease(MeshStateCube);
        SafeRelease(MeshStateQuad);
        SafeRelease(MeshInstStateRect);
        SafeRelease(MeshInstStateLine);

        SafeRelease(LvDebugTexture);
        SafeRelease(LvTestTexture);

        SafeRelease(DX_WBuffer);
        SafeRelease(DX_VPBuffer);

        SafeRelease(DX_DefaultSamplerState);

        SafeRelease(DrawStateColor);
        SafeRelease(DrawStateTexture);
        SafeRelease(DrawStateInstRectColor);
        SafeRelease(DrawStateInstRectTexture);
        SafeRelease(DrawStateInstLine);

        SafeRelease(DXGI_SwapChain1);
        SafeRelease(DX_Backbuffer);
        SafeRelease(DX_RenderTargetView);
        SafeRelease(DX_RasterizerState);
        SafeRelease(DX_DepthStencil);
        SafeRelease(DX_DepthStencilView);
        SafeRelease(DX_Draw2DDepthStencilState);
        SafeRelease(DX_DefaultBlendState);
        SafeRelease(DX_AlphaBlendState);

        SafeRelease(DXGI_Factory2);
        SafeRelease(DX_ImmediateContext);

        if (DX_Device)
        {
            ID3D11Debug* DX_Debug = nullptr;
            DX_Device->QueryInterface(DX_UUID_HELPER(ID3D11Debug, DX_Debug));
            if (DX_Debug) { DX_Debug->ReportLiveDeviceObjects(D3D11_RLDO_IGNORE_INTERNAL); }
            SafeRelease(DX_Debug);
        }

        SafeRelease(DX_Device);
    }
}
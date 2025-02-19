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
        ID3D11DepthStencilState* DX_2DInstancedDepthStencilState = nullptr;
        ID3D11BlendState* DX_BlendState = nullptr;
        ID3D11BlendState* DX_AlphaBlendState = nullptr;

        DrawStateT DrawStateColor;
        DrawStateT DrawStateTexture;
        DrawStateT DrawStateUnicolor;
        DrawStateT DrawStateInstRectColor;
        DrawStateT DrawStateInstRectTexture;

        ID3D11Buffer* DX_WBuffer = nullptr;
        ID3D11Buffer* DX_VPBuffer = nullptr;
        const int WBufferSlot = 0;
        const int VPBufferSlot = 1;
        ID3D11Buffer* DX_UnicolorBuffer = nullptr;
        const int UnicolorBufferSlot = 2;

        MeshStateT MeshStateTriangle;
        MeshStateT MeshStateCube;
        MeshStateT MeshStateQuad;
        MeshStateT MeshStateMinQuad;
        MeshStateT MeshStateSpriteQuad;

        ID3D11Buffer* DX_VxBufferLines = nullptr;

        ID3D11Buffer* DX_InstRectBuffer = nullptr;
        ID3D11Buffer* DX_BatchedRectColorCmdsBuffer = nullptr;
        ID3D11Buffer* DX_BatchedRectTextureCmdsBuffer = nullptr;

        LvTexture2D LvDebugTexture;
        LvTexture2D LvTestTexture;
        ID3D11SamplerState* DX_DefaultSamplerState = nullptr;

        D3D_FEATURE_LEVEL UsedFeatureLevel;

        // Direct2D:
        ID2D1Factory* D2_Factory = nullptr;
        IDXGISurface* DXGI_Surface = nullptr;
        ID2D1RenderTarget* D2_RenderTarget = nullptr;
        ID2D1LinearGradientBrush* D2_LinearGradientBrush = nullptr;
        ID2D1GradientStopCollection* D2_GradientStops = nullptr;
        ID2D1SolidColorBrush* D2_LightGrayBrush = nullptr;
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

    struct VPData
    {
        m4f View;
        m4f Proj;
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
        constexpr RectF InvalidRect{ 0.0f, 0.0f, 0.0f, 0.0f };
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

    VxTexture Vertices_Quad[] =
    {
        { {-0.5f, +0.5f, +0.5f, +1.0f}, { 0.0f, 0.0f } },
        { {+0.5f, +0.5f, +0.5f, +1.0f}, { 1.0f, 0.0f } },
        { {-0.5f, -0.5f, +0.5f, +1.0f}, { 0.0f, 1.0f } },
        { {+0.5f, -0.5f, +0.5f, +1.0f}, { 1.0f, 1.0f } },
    };
    VxMin Vertices_MinQuad[] =
    {
        { {-0.5f, +0.5f, +0.5f, +1.0f} },
        { {+0.5f, +0.5f, +0.5f, +1.0f} },
        { {-0.5f, -0.5f, +0.5f, +1.0f} },
        { {+0.5f, -0.5f, +0.5f, +1.0f} },
    };
    unsigned int Indices_Quad[] =
    {
        0, 2, 1,
        1, 2, 3
    };

    VxMin Vertices_SpriteQuad[] =
    {
        { { 0.0f, 1.0f, +0.5f, +1.0f } },
        { { 1.0f, 1.0f, +0.5f, +1.0f } },
        { { 0.0f, 0.0f, +0.5f, +1.0f } },
        { { 1.0f, 0.0f, +0.5f, +1.0f } },
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
    InstRectColorData InstRectDataArray[] =
    {
        { {0.0f, BottomRectY, TestInstRectSize, TestInstRectSize}, {0.0f, 1.0f, 1.0f, 1.0f} },
        { {RightRectX, BottomRectY, TestInstRectSize, TestInstRectSize}, {1.0f, 0.0f, 1.0f, 1.0f} },
        { {0.0f, 0.0f, TestInstRectSize, TestInstRectSize}, { 1.0f, 1.0f, 0.0f, 1.0f } },
        { {RightRectX, 0.0f, TestInstRectSize, TestInstRectSize}, {0.0f, 0.0f, 0.0f, 1.0f} }
    };
    InstRectTextureData InstRectTextureDataArray[] =
    {
        { {0.0f, BottomRectY, TestInstRectSize, TestInstRectSize}, {0.0f, 0.0f, 0.5f, 0.5f} },
        { {RightRectX, BottomRectY, TestInstRectSize, TestInstRectSize}, {0.5f, 0.0f, 0.5f, 0.5f} },
        { {0.0f, 0.0f, TestInstRectSize, TestInstRectSize}, { 0.0f, 0.5f, 0.5f, 0.5f } },
        { {RightRectX, 0.0f, TestInstRectSize, TestInstRectSize}, {0.5f, 0.5f, 0.5f, 0.5f} }
    };

    float HalfWidth = (float)AppWidth / 2.0f;
    float HalfHeight = (float)AppHeight / 2.0f;
    VxColor Vertices_Lines[] =
    {
        { {0.0f, 0.0f, +0.5f, 1.0f}, {1.0f, 0.0f, 0.0f, 1.0f} },
        { {AppWidth, AppHeight, +0.5f, 1.0f}, {1.0f, 0.0f, 0.0f, 1.0f} },

        { {0.0f, AppHeight, +0.5f, 1.0f}, {0.0f, 1.0f, 0.0f, 1.0f} },
        { {AppWidth, 0.0f, +0.5f, 1.0f}, {0.0f, 1.0f, 0.0f, 1.0f} },

        { {HalfWidth, 0.0f, +0.5f, 1.0f}, {0.0f, 0.0f, 1.0f, 1.0f} },
        { {HalfWidth, AppHeight, +0.5f, 1.0f}, {0.0f, 0.0f, 1.0f, 1.0f} },

        { {0.0f, HalfHeight, +0.5f, 1.0f}, {1.0f, 1.0f, 1.0f, 1.0f} },
        { {AppWidth, HalfHeight, +0.5f, 1.0f}, {1.0f, 1.0f, 1.0f, 1.0f} },
    };

    m4f GetMatrix(const SpriteTransform& InSprTrans)
    {
        return m4f::Scale(InSprTrans.Scale.X, InSprTrans.Scale.Y, 1.0f) * m4f::RotAxisZ(InSprTrans.RotAngle) * m4f::Trans(InSprTrans.Pos.X, InSprTrans.Pos.Y, 0.0f);
    }

    Camera OrthoCamera;
    Camera GameCamera;
    BatchDraw2D Draw2D;
    HandmadeTextSheet HandmadeText;

    constexpr int DefaultSize_BatchDraw2D = 1024;
    void Graphics::Draw()
    {
        Draw2D.Clear();

        DX_ImmediateContext->RSSetState(DX_RasterizerState);
        UINT DefaultSampleMask = 0xFFFFFFFF;
        DX_ImmediateContext->OMSetBlendState(nullptr, nullptr, DefaultSampleMask);
        DX_ImmediateContext->OMSetDepthStencilState(nullptr, 0);
        DX_ImmediateContext->OMSetRenderTargets(1, &DX_RenderTargetView, DX_DepthStencilView);
        v4f ClearColor = { 30.0f / 255.0f, 30.0f / 255.0f, 45.0f / 255.0f, 1.0f };
        float fClearDepth = 1.0f;
        DX_ImmediateContext->ClearRenderTargetView(DX_RenderTargetView, &ClearColor.X);
        DX_ImmediateContext->ClearDepthStencilView(DX_DepthStencilView, D3D11_CLEAR_DEPTH, fClearDepth, 0);
        const m4f DefaultSpriteWorld = m4f::Trans(-HalfWidth, -HalfHeight, 0.0f);

        static bool bD2Background = false;
        if (bD2Background)
        { // Direct2D background
            DXGI_SWAP_CHAIN_DESC SwapChainDesc = {};
            DX_CHECK(DXGI_SwapChain1->GetDesc(&SwapChainDesc));

            D2D1_SIZE_F TargetSize = D2_RenderTarget->GetSize();

            D2_RenderTarget->BeginDraw();

            static bool bDrawGradientBG = true;
            if (bDrawGradientBG)
            {
                D2_LinearGradientBrush->SetTransform(D2D1::Matrix3x2F::Scale(TargetSize));
                D2D1_RECT_F Rect = D2D1::RectF(0.0f, 0.0f, TargetSize.width, TargetSize.height);
                D2_RenderTarget->FillRectangle(&Rect, D2_LinearGradientBrush);
            }

            static bool bDrawGridBG = true;
            if (bDrawGridBG)
            {
                const int GridX = 16;
                const int GridY = 9;
                const float CellSizeX = TargetSize.width / (float)GridX;
                const float CellSizeY = TargetSize.height / (float)GridY;
                const FLOAT StrokeWidth = 1.0f;
                ID2D1StrokeStyle* StrokeStyle = nullptr;
                for (int ColIdx = 1; ColIdx < GridX; ColIdx++)
                {
                    D2D1_POINT_2F StartPoint = D2D1::Point2F(CellSizeX * ColIdx, 0.0f);
                    D2D1_POINT_2F EndPoint = D2D1::Point2F(CellSizeX * ColIdx, TargetSize.height);
                    D2_RenderTarget->DrawLine(StartPoint, EndPoint, D2_LightGrayBrush, StrokeWidth, StrokeStyle);
                }
                for (int RowIdx = 1; RowIdx < GridY; RowIdx++)
                {
                    D2D1_POINT_2F StartPoint = D2D1::Point2F(0.0f, CellSizeY * RowIdx);
                    D2D1_POINT_2F EndPoint = D2D1::Point2F(TargetSize.width, CellSizeY * RowIdx);
                    D2_RenderTarget->DrawLine(StartPoint, EndPoint, D2_LightGrayBrush, StrokeWidth, StrokeStyle);
                }
            }

            DX_CHECK(D2_RenderTarget->EndDraw());
        }

        static bool bDrawLines = true;
        if (bDrawLines)
        {
            DX_ImmediateContext->OMSetBlendState(nullptr, nullptr, DefaultSampleMask);
            DX_ImmediateContext->UpdateSubresource(DX_WBuffer, 0, nullptr, &DefaultSpriteWorld, sizeof(m4f), 0);
            DX_ImmediateContext->UpdateSubresource(DX_VPBuffer, 0, nullptr, &OrthoCamera.View, sizeof(Camera), 0);
            
            UINT Offset = 0;
            const UINT Stride = sizeof(VxColor);
            DX_ImmediateContext->IASetInputLayout(DrawStateColor.InputLayout);
            DX_ImmediateContext->IASetVertexBuffers(0, 1, &DX_VxBufferLines, &Stride, &Offset);
            DX_ImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

            DX_ImmediateContext->VSSetShader(DrawStateColor.VertexShader, nullptr, 0);
            DX_ImmediateContext->VSSetConstantBuffers(WBufferSlot, 1, &DX_WBuffer);
            DX_ImmediateContext->VSSetConstantBuffers(VPBufferSlot, 1, &DX_VPBuffer);
            DX_ImmediateContext->PSSetShader(DrawStateColor.PixelShader, nullptr, 0);

            DX_ImmediateContext->Draw(ARRAY_SIZE(Vertices_Lines), 0u);
        }


        static bool bDrawTriangle = true;
        if (bDrawTriangle)
        { // Draw triangle
            DX_ImmediateContext->OMSetBlendState(nullptr, nullptr, DefaultSampleMask);
            m4f TriangleWorld = m4f::Scale(128.0f, 128.0f, 1.0f) * m4f::Trans(256.0f, -256.0f, 0.0f);
            DX_ImmediateContext->UpdateSubresource(DX_WBuffer, 0, nullptr, &TriangleWorld, sizeof(m4f), 0);
            DX_ImmediateContext->UpdateSubresource(DX_VPBuffer, 0, nullptr, &OrthoCamera.View, sizeof(Camera), 0);

            UINT Offset = 0;
            const UINT Stride = sizeof(VxColor);
            DX_ImmediateContext->IASetInputLayout(DrawStateColor.InputLayout);
            DX_ImmediateContext->IASetVertexBuffers(0, 1, &MeshStateTriangle.VxBuffer, &Stride, &Offset);
            DX_ImmediateContext->IASetIndexBuffer(MeshStateTriangle.IxBuffer, DXGI_FORMAT_R32_UINT, 0);
            DX_ImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

            DX_ImmediateContext->VSSetShader(DrawStateColor.VertexShader, nullptr, 0);
            DX_ImmediateContext->VSSetConstantBuffers(WBufferSlot, 1, &DX_WBuffer);
            DX_ImmediateContext->VSSetConstantBuffers(VPBufferSlot, 1, &DX_VPBuffer);
            DX_ImmediateContext->PSSetShader(DrawStateColor.PixelShader, nullptr, 0);

            DX_ImmediateContext->DrawIndexed(MeshStateTriangle.NumInds, 0u, 0u);
        }

        static bool bDrawTexQuad = true;
        if (bDrawTexQuad)
        { // Draw tex quad
            DX_ImmediateContext->OMSetBlendState(DX_AlphaBlendState, nullptr, 0xFFFFFFFF);
            m4f TexQuadWorld = m4f::Scale(100.0f, 100.0f, 1.0f) * m4f::Trans(+256.0f, +256.0f, 0.0f);
            DX_ImmediateContext->UpdateSubresource(DX_WBuffer, 0, nullptr, &TexQuadWorld, sizeof(m4f), 0);
            DX_ImmediateContext->UpdateSubresource(DX_VPBuffer, 0, nullptr, &OrthoCamera.View, sizeof(Camera), 0);

            UINT Offset = 0;
            const UINT Stride = sizeof(VxTexture);
            DX_ImmediateContext->IASetInputLayout(DrawStateTexture.InputLayout);
            DX_ImmediateContext->IASetVertexBuffers(0, 1, &MeshStateQuad.VxBuffer, &Stride, &Offset);
            DX_ImmediateContext->IASetIndexBuffer(MeshStateQuad.IxBuffer, DXGI_FORMAT_R32_UINT, 0);
            DX_ImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

            DX_ImmediateContext->VSSetShader(DrawStateTexture.VertexShader, nullptr, 0);
            DX_ImmediateContext->VSSetConstantBuffers(WBufferSlot, 1, &DX_WBuffer);
            DX_ImmediateContext->VSSetConstantBuffers(VPBufferSlot, 1, &DX_VPBuffer);
            DX_ImmediateContext->PSSetShader(DrawStateTexture.PixelShader, nullptr, 0);
            //DX_ImmediateContext->PSSetShaderResources(0, 1, &DX_DebugTextureSRV);
            DX_ImmediateContext->PSSetShaderResources(0, 1, &LvTestTexture.SRV);
            DX_ImmediateContext->PSSetSamplers(0, 1, &DX_DefaultSamplerState);

            DX_ImmediateContext->DrawIndexed(MeshStateQuad.NumInds, 0u, 0u);
        }
        DX_ImmediateContext->OMSetBlendState(nullptr, nullptr, DefaultSampleMask);

        static bool bDrawUnicolorQuad = true;
        if (bDrawUnicolorQuad)
        {
            DX_ImmediateContext->OMSetBlendState(DX_AlphaBlendState, nullptr, 0xFFFFFFFF);
            m4f UnicolorQuadWorld = m4f::Scale(100.0f, 100.0f, 1.0f) * m4f::Trans(-256.0f, -256.0f, 0.0f);
            DX_ImmediateContext->UpdateSubresource(DX_WBuffer, 0, nullptr, &UnicolorQuadWorld, sizeof(m4f), 0);
            DX_ImmediateContext->UpdateSubresource(DX_VPBuffer, 0, nullptr, &OrthoCamera.View, sizeof(Camera), 0);
            v4f UnicolorValue{ 1.0f, 0.5f, 0.75f, 1.0f };
            DX_ImmediateContext->UpdateSubresource(DX_UnicolorBuffer, 0, nullptr, &UnicolorValue, sizeof(v4f), 0);

            UINT Offset = 0;
            const UINT Stride = sizeof(VxMin);
            DX_ImmediateContext->IASetInputLayout(DrawStateUnicolor.InputLayout);
            DX_ImmediateContext->IASetVertexBuffers(0, 1, &MeshStateMinQuad.VxBuffer, &Stride, &Offset);
            DX_ImmediateContext->IASetIndexBuffer(MeshStateMinQuad.IxBuffer, DXGI_FORMAT_R32_UINT, 0);
            DX_ImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

            DX_ImmediateContext->VSSetShader(DrawStateUnicolor.VertexShader, nullptr, 0);
            DX_ImmediateContext->VSSetConstantBuffers(WBufferSlot, 1, &DX_WBuffer);
            DX_ImmediateContext->VSSetConstantBuffers(VPBufferSlot, 1, &DX_VPBuffer);
            DX_ImmediateContext->VSSetConstantBuffers(UnicolorBufferSlot, 1, &DX_UnicolorBuffer);
            DX_ImmediateContext->PSSetShader(DrawStateUnicolor.PixelShader, nullptr, 0);

            DX_ImmediateContext->DrawIndexed(MeshStateMinQuad.NumInds, 0u, 0u);
        }

        DX_ImmediateContext->OMSetDepthStencilState(DX_2DInstancedDepthStencilState, 0);

        const float HalfWidth = (float)AppWidth / 2.0f;
        const float HalfHeight = (float)AppHeight / 2.0f;
        static bool bDrawInstRects = true;
        if (bDrawInstRects)
        { // Draw Instanced Rects
            DX_ImmediateContext->OMSetBlendState(DX_AlphaBlendState, nullptr, 0xFFFFFFFF);
            DX_ImmediateContext->UpdateSubresource(DX_WBuffer, 0, nullptr, &DefaultSpriteWorld, sizeof(m4f), 0);
            DX_ImmediateContext->UpdateSubresource(DX_VPBuffer, 0, nullptr, &OrthoCamera.View, sizeof(Camera), 0);

            ID3D11Buffer* VxInstBuffers[] = { MeshStateSpriteQuad.VxBuffer, DX_InstRectBuffer };
            const UINT Strides[] = { sizeof(VxMin), sizeof(InstRectColorData) };
            const UINT Offsets[] = { 0, 0 };
            ASSERT((ARRAY_SIZE(VxInstBuffers) == ARRAY_SIZE(Strides)) && (ARRAY_SIZE(VxInstBuffers) == ARRAY_SIZE(Offsets)));
            //DX_ImmediateContext->IASetInputLayout(DrawStateInstRectColor.InputLayout);
            DX_ImmediateContext->IASetInputLayout(DrawStateInstRectTexture.InputLayout);
            DX_ImmediateContext->IASetVertexBuffers(0, ARRAY_SIZE(VxInstBuffers), VxInstBuffers, Strides, Offsets);
            DX_ImmediateContext->IASetIndexBuffer(MeshStateSpriteQuad.IxBuffer, DXGI_FORMAT_R32_UINT, 0);
            DX_ImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

            //DX_ImmediateContext->VSSetShader(DrawStateInstRectColor.VertexShader, nullptr, 0);
            DX_ImmediateContext->VSSetShader(DrawStateInstRectTexture.VertexShader, nullptr, 0);
            DX_ImmediateContext->VSSetConstantBuffers(VPBufferSlot, 1, &DX_VPBuffer);
            //DX_ImmediateContext->PSSetShader(DrawStateInstRectColor.PixelShader, nullptr, 0);
            DX_ImmediateContext->PSSetShader(DrawStateInstRectTexture.PixelShader, nullptr, 0);
            DX_ImmediateContext->PSSetShaderResources(0, 1, &LvTestTexture.SRV);
            DX_ImmediateContext->PSSetSamplers(0, 1, &DX_DefaultSamplerState);

            DX_ImmediateContext->DrawIndexedInstanced(MeshStateSpriteQuad.NumInds, ARRAY_SIZE(InstRectDataArray), 0u, 0, 0u);
        }

        static bool bDrawGame = true;
        // Get instanced draw commands from game:
        if (bDrawGame) { GameManager::Draw(Draw2D); }
        if (Draw2D.ColorBatchCmds.Num > 0)
        {
            DX_ImmediateContext->UpdateSubresource(DX_WBuffer, 0, nullptr, &DefaultSpriteWorld, sizeof(m4f), 0);
            DX_ImmediateContext->UpdateSubresource(DX_VPBuffer, 0, nullptr, &OrthoCamera.View, sizeof(Camera), 0);

            { // Send BatchedCmds state to GPU
                D3D11_MAPPED_SUBRESOURCE MappedBatchCmds = {};
                DX_ImmediateContext->Map(DX_BatchedRectColorCmdsBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedBatchCmds);
                memcpy(MappedBatchCmds.pData, Draw2D.ColorBatchCmds.Data, sizeof(InstRectColorData) * Draw2D.ColorBatchCmds.Num);
                DX_ImmediateContext->Unmap(DX_BatchedRectColorCmdsBuffer, 0);
            }

            ID3D11Buffer* VxInstBuffers[] = { MeshStateSpriteQuad.VxBuffer, DX_BatchedRectColorCmdsBuffer };
            const UINT Strides[] = { sizeof(VxMin), sizeof(InstRectColorData) };
            const UINT Offsets[] = { 0, 0 };
            ASSERT((ARRAY_SIZE(VxInstBuffers) == ARRAY_SIZE(Strides)) && (ARRAY_SIZE(VxInstBuffers) == ARRAY_SIZE(Offsets)));
            DX_ImmediateContext->IASetInputLayout(DrawStateInstRectColor.InputLayout);
            DX_ImmediateContext->IASetVertexBuffers(0, ARRAY_SIZE(VxInstBuffers), VxInstBuffers, Strides, Offsets);
            DX_ImmediateContext->IASetIndexBuffer(MeshStateSpriteQuad.IxBuffer, DXGI_FORMAT_R32_UINT, 0);
            DX_ImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

            DX_ImmediateContext->VSSetShader(DrawStateInstRectColor.VertexShader, nullptr, 0);
            DX_ImmediateContext->VSSetConstantBuffers(WBufferSlot, 1, &DX_WBuffer);
            DX_ImmediateContext->VSSetConstantBuffers(VPBufferSlot, 1, &DX_VPBuffer);
            DX_ImmediateContext->PSSetShader(DrawStateInstRectColor.PixelShader, nullptr, 0);

            DX_ImmediateContext->DrawIndexedInstanced(MeshStateSpriteQuad.NumInds, Draw2D.ColorBatchCmds.Num, 0u, 0, 0u);
        }

        DX_ImmediateContext->OMSetDepthStencilState(nullptr, 0);

        static bool bDrawCube = true;
        if (bDrawCube)
        { // Draw cube
            static float RotationX = 0.0f;
            static float RotationY = 0.0f;
            static constexpr float RotSpeed = (1.0f / 60.0f) / 10.0f;
            RotationX += RotSpeed;
            RotationY += RotSpeed * 0.5f;
            m4f CubeWorld = m4f::RotAxisX(RotationX) * m4f::RotAxisY(RotationY);
            DX_ImmediateContext->UpdateSubresource(DX_WBuffer, 0, nullptr, &CubeWorld, sizeof(m4f), 0);
            DX_ImmediateContext->UpdateSubresource(DX_VPBuffer, 0, nullptr, &GameCamera.View, sizeof(Camera), 0);

            UINT Offset = 0;
            const UINT Stride = sizeof(VxColor);
            DX_ImmediateContext->IASetInputLayout(DrawStateColor.InputLayout);
            DX_ImmediateContext->IASetVertexBuffers(0, 1, &MeshStateCube.VxBuffer, &Stride, &Offset);
            DX_ImmediateContext->IASetIndexBuffer(MeshStateCube.IxBuffer, DXGI_FORMAT_R32_UINT, 0);
            DX_ImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

            DX_ImmediateContext->VSSetShader(DrawStateColor.VertexShader, nullptr, 0);
            DX_ImmediateContext->VSSetConstantBuffers(WBufferSlot, 1, &DX_WBuffer);
            DX_ImmediateContext->VSSetConstantBuffers(VPBufferSlot, 1, &DX_VPBuffer);
            DX_ImmediateContext->PSSetShader(DrawStateColor.PixelShader, nullptr, 0);

            DX_ImmediateContext->DrawIndexed(MeshStateCube.NumInds, 0u, 0u);
        }

        static bool bDrawUI = true;
        if (bDrawUI)
        {
            Draw2D.Clear();

            UserInterface::Draw(D2_RenderTarget, Draw2D);

            DX_ImmediateContext->OMSetBlendState(nullptr, nullptr, 0xFFFFFFFF);
            DX_ImmediateContext->OMSetDepthStencilState(DX_2DInstancedDepthStencilState, 0);
            DX_ImmediateContext->UpdateSubresource(DX_WBuffer, 0, nullptr, &DefaultSpriteWorld, sizeof(m4f), 0);
            DX_ImmediateContext->UpdateSubresource(DX_VPBuffer, 0, nullptr, &OrthoCamera.View, sizeof(Camera), 0);

            { // Send BatchedCmds state to GPU
                D3D11_MAPPED_SUBRESOURCE MappedBatchCmds = {};
                DX_ImmediateContext->Map(DX_BatchedRectColorCmdsBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedBatchCmds);
                memcpy(MappedBatchCmds.pData, Draw2D.ColorBatchCmds.Data, sizeof(InstRectColorData) * Draw2D.ColorBatchCmds.Num);
                DX_ImmediateContext->Unmap(DX_BatchedRectColorCmdsBuffer, 0);
            }

            ID3D11Buffer* VxInstBuffers[] = { MeshStateSpriteQuad.VxBuffer, DX_BatchedRectColorCmdsBuffer };
            const UINT Strides[] = { sizeof(VxMin), sizeof(InstRectColorData) };
            const UINT Offsets[] = { 0, 0 };
            ASSERT((ARRAY_SIZE(VxInstBuffers) == ARRAY_SIZE(Strides)) && (ARRAY_SIZE(VxInstBuffers) == ARRAY_SIZE(Offsets)));
            DX_ImmediateContext->IASetInputLayout(DrawStateInstRectColor.InputLayout);
            DX_ImmediateContext->IASetVertexBuffers(0, ARRAY_SIZE(VxInstBuffers), VxInstBuffers, Strides, Offsets);
            DX_ImmediateContext->IASetIndexBuffer(MeshStateSpriteQuad.IxBuffer, DXGI_FORMAT_R32_UINT, 0);
            DX_ImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

            DX_ImmediateContext->VSSetShader(DrawStateInstRectColor.VertexShader, nullptr, 0);
            DX_ImmediateContext->VSSetConstantBuffers(VPBufferSlot, 1, &DX_VPBuffer);
            DX_ImmediateContext->PSSetShader(DrawStateInstRectColor.PixelShader, nullptr, 0);

            DX_ImmediateContext->DrawIndexedInstanced(MeshStateSpriteQuad.NumInds, Draw2D.ColorBatchCmds.Num, 0u, 0, 0u);
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

            DX_ImmediateContext->UpdateSubresource(DX_WBuffer, 0, nullptr, &DefaultSpriteWorld, sizeof(m4f), 0);
            DX_ImmediateContext->UpdateSubresource(DX_VPBuffer, 0, nullptr, &OrthoCamera.View, sizeof(Camera), 0);

            { // Send BatchedCmds state to GPU
                D3D11_MAPPED_SUBRESOURCE MappedBatchCmds = {};
                DX_ImmediateContext->Map(DX_BatchedRectColorCmdsBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedBatchCmds);
                memcpy(MappedBatchCmds.pData, Draw2D.TextBatchCmds.Data, sizeof(InstRectTextureData) * Draw2D.TextBatchCmds.Num);
                DX_ImmediateContext->Unmap(DX_BatchedRectColorCmdsBuffer, 0);
            }

            ID3D11Buffer* VxInstBuffers[] = { MeshStateSpriteQuad.VxBuffer, DX_BatchedRectColorCmdsBuffer };
            const UINT Strides[] = { sizeof(VxMin), sizeof(InstRectTextureData) };
            const UINT Offsets[] = { 0, 0 };
            ASSERT((ARRAY_SIZE(VxInstBuffers) == ARRAY_SIZE(Strides)) && (ARRAY_SIZE(VxInstBuffers) == ARRAY_SIZE(Offsets)));
            DX_ImmediateContext->IASetInputLayout(DrawStateInstRectTexture.InputLayout);
            DX_ImmediateContext->IASetVertexBuffers(0, ARRAY_SIZE(VxInstBuffers), VxInstBuffers, Strides, Offsets);
            DX_ImmediateContext->IASetIndexBuffer(MeshStateSpriteQuad.IxBuffer, DXGI_FORMAT_R32_UINT, 0);
            DX_ImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

            DX_ImmediateContext->VSSetShader(DrawStateInstRectTexture.VertexShader, nullptr, 0);
            DX_ImmediateContext->VSSetConstantBuffers(WBufferSlot, 1, &DX_WBuffer);
            DX_ImmediateContext->VSSetConstantBuffers(VPBufferSlot, 1, &DX_VPBuffer);
            DX_ImmediateContext->PSSetShaderResources(0, 1, &HandmadeText.LvTex2D.SRV);
            DX_ImmediateContext->PSSetSamplers(0, 1, &DX_DefaultSamplerState);
            DX_ImmediateContext->PSSetShader(DrawStateInstRectTexture.PixelShader, nullptr, 0);

            DX_ImmediateContext->DrawIndexedInstanced(MeshStateSpriteQuad.NumInds, Draw2D.TextBatchCmds.Num, 0u, 0, 0u);
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

        UINT FrameRefreshRate = 60;
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
        DX_Device->CreateDepthStencilState(&Instanced2DDepthStencilStateDesc, &DX_2DInstancedDepthStencilState);

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

        {
            const D3D_SHADER_MACRO DefinesVxMin[] =
            {
                "ENABLE_VERTEX_COLOR", "0",
                "ENABLE_VERTEX_TEXTURE", "0",
                "ENABLE_WVP_TRANSFORM", "1",
                "ENABLE_UNICOLOR", "1",
                nullptr, nullptr
            };
            D3D11_INPUT_ELEMENT_DESC InputLayoutDesc[] =
            {
                { "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
            };

            DrawStateUnicolor = CreateDrawState(DX_Device, BaseShaderFilename, DefinesVxMin, InputLayoutDesc, ARRAY_SIZE(InputLayoutDesc));
        }

        {
            const D3D_SHADER_MACRO DefinesVxMin[] =
            {
                "ENABLE_VERTEX_COLOR", "0",
                "ENABLE_VERTEX_TEXTURE", "0",
                "ENABLE_WVP_TRANSFORM", "1",
                "ENABLE_UNICOLOR", "1",
                nullptr, nullptr
            };
            D3D11_INPUT_ELEMENT_DESC InputLayoutDesc[] =
            {
                { "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
            };

            DrawStateUnicolor = CreateDrawState(DX_Device, BaseShaderFilename, DefinesVxMin, InputLayoutDesc, ARRAY_SIZE(InputLayoutDesc));
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

        D3D11_BUFFER_DESC InstRectBufferDesc = {};
        InstRectBufferDesc.ByteWidth = sizeof(InstRectDataArray);
        InstRectBufferDesc.Usage = D3D11_USAGE_DEFAULT;
        InstRectBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        InstRectBufferDesc.CPUAccessFlags = 0;
        InstRectBufferDesc.StructureByteStride = 0;
        //D3D11_SUBRESOURCE_DATA InstRectBufferInitData = { InstRectDataArray, 0, 0 };
        D3D11_SUBRESOURCE_DATA InstRectBufferInitData = { InstRectTextureDataArray, 0, 0 };
        DX_CHECK(DX_Device->CreateBuffer(&InstRectBufferDesc, &InstRectBufferInitData, &DX_InstRectBuffer));

        Draw2D.ColorBatchCmds.Reserve(DefaultSize_BatchDraw2D);
        D3D11_BUFFER_DESC BatchedColorCmdsBuffer = {};
        BatchedColorCmdsBuffer.ByteWidth = sizeof(InstRectColorData) * Draw2D.ColorBatchCmds.Capacity;
        BatchedColorCmdsBuffer.Usage = D3D11_USAGE_DYNAMIC;
        BatchedColorCmdsBuffer.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        BatchedColorCmdsBuffer.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        BatchedColorCmdsBuffer.MiscFlags = 0;
        BatchedColorCmdsBuffer.StructureByteStride = 0;
        DX_CHECK(DX_Device->CreateBuffer(&BatchedColorCmdsBuffer, nullptr, &DX_BatchedRectColorCmdsBuffer));

        Draw2D.TextureBatchCmds.Reserve(DefaultSize_BatchDraw2D);
        D3D11_BUFFER_DESC BatchedTextureCmdsBuffer = {};
        BatchedColorCmdsBuffer.ByteWidth = sizeof(InstRectTextureData) * Draw2D.TextureBatchCmds.Capacity;
        BatchedColorCmdsBuffer.Usage = D3D11_USAGE_DYNAMIC;
        BatchedColorCmdsBuffer.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        BatchedColorCmdsBuffer.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        BatchedColorCmdsBuffer.MiscFlags = 0;
        BatchedColorCmdsBuffer.StructureByteStride = 0;
        DX_CHECK(DX_Device->CreateBuffer(&BatchedColorCmdsBuffer, nullptr, &DX_BatchedRectTextureCmdsBuffer));

        D3D11_BUFFER_DESC WorldBufferDesc = {};
        WorldBufferDesc.ByteWidth = sizeof(m4f);
        WorldBufferDesc.Usage = D3D11_USAGE_DEFAULT;
        WorldBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        WorldBufferDesc.CPUAccessFlags = 0;
        D3D11_BUFFER_DESC ViewProjBufferDesc = {};
        ViewProjBufferDesc.ByteWidth = sizeof(VPData);
        ViewProjBufferDesc.Usage = D3D11_USAGE_DEFAULT;
        ViewProjBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        ViewProjBufferDesc.CPUAccessFlags = 0;
        DX_CHECK(DX_Device->CreateBuffer(&WorldBufferDesc, nullptr, &DX_WBuffer));
        DX_CHECK(DX_Device->CreateBuffer(&ViewProjBufferDesc, nullptr, &DX_VPBuffer));

        D3D11_BUFFER_DESC UnicolorBufferDesc = {};
        UnicolorBufferDesc.ByteWidth = sizeof(v4f);
        UnicolorBufferDesc.Usage = D3D11_USAGE_DEFAULT;
        UnicolorBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        UnicolorBufferDesc.CPUAccessFlags = 0;
        DX_CHECK(DX_Device->CreateBuffer(&UnicolorBufferDesc, nullptr, &DX_UnicolorBuffer));

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

        MeshStateMinQuad = CreateMeshState
        (
            DX_Device,
            sizeof(VxMin),
            ARRAY_SIZE(Vertices_MinQuad),
            Vertices_MinQuad,
            ARRAY_SIZE(Indices_Quad),
            Indices_Quad
        );

        MeshStateSpriteQuad = CreateMeshState
        (
            DX_Device,
            sizeof(VxMin),
            ARRAY_SIZE(Vertices_SpriteQuad),
            Vertices_SpriteQuad,
            ARRAY_SIZE(Indices_Quad),
            Indices_Quad
        );

        { // Lines Vertex Buffer
            D3D11_BUFFER_DESC VertexBufferDesc = { sizeof(Vertices_Lines), D3D11_USAGE_DEFAULT, D3D11_BIND_VERTEX_BUFFER, 0, 0};
            D3D11_SUBRESOURCE_DATA VertexBufferInitData = { Vertices_Lines, 0, 0 };
            DX_CHECK(DX_Device->CreateBuffer(&VertexBufferDesc, &VertexBufferInitData, &DX_VxBufferLines));
        }

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
            ARRAY_SIZE(Vertices_Quad),
            Vertices_Quad,
            ARRAY_SIZE(Indices_Quad),
            Indices_Quad
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
            D2D1_GRADIENT_STOP GradientBrushStops[] =
            {
                {0.0f, D2D1::ColorF(D2D1::ColorF::MidnightBlue, 1)},
                {1.0f, D2D1::ColorF(D2D1::ColorF::DarkViolet, 1)},
            };
            DX_CHECK(D2_RenderTarget->CreateGradientStopCollection(GradientBrushStops, ARRAY_SIZE(GradientBrushStops), D2D1_GAMMA_2_2, D2D1_EXTEND_MODE_CLAMP, &D2_GradientStops));
            DX_CHECK(D2_RenderTarget->CreateLinearGradientBrush(
                D2D1::LinearGradientBrushProperties(D2D1::Point2F(0.0f, 0.0f), D2D1::Point2F(1.0, 1.0f)),
                D2_GradientStops, &D2_LinearGradientBrush));

            D2D1_COLOR_F LightGrayColor = D2D1::ColorF(0xE1E6EF);
            D2_RenderTarget->CreateSolidColorBrush(LightGrayColor, &D2_LightGrayBrush);
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
            SafeRelease(D2_LinearGradientBrush);
            SafeRelease(D2_GradientStops);
            SafeRelease(D2_LightGrayBrush);
        }

        SafeRelease(MeshStateTriangle);
        SafeRelease(MeshStateCube);
        SafeRelease(MeshStateQuad);
        SafeRelease(MeshStateMinQuad);
        SafeRelease(MeshStateSpriteQuad);

        SafeRelease(LvDebugTexture);
        SafeRelease(LvTestTexture);

        SafeRelease(DX_WBuffer);
        SafeRelease(DX_VPBuffer);
        SafeRelease(DX_UnicolorBuffer);
        SafeRelease(DX_InstRectBuffer);
        SafeRelease(DX_BatchedRectColorCmdsBuffer);
        SafeRelease(DX_BatchedRectTextureCmdsBuffer);

        SafeRelease(DrawStateColor);
        SafeRelease(DrawStateTexture);
        SafeRelease(DrawStateUnicolor);
        SafeRelease(DrawStateInstRectColor);
        SafeRelease(DrawStateInstRectTexture);

        SafeRelease(DXGI_SwapChain1);
        SafeRelease(DX_Backbuffer);
        SafeRelease(DX_RenderTargetView);
        SafeRelease(DX_RasterizerState);
        SafeRelease(DX_DepthStencil);
        SafeRelease(DX_DepthStencilView);
        SafeRelease(DX_2DInstancedDepthStencilState);
        SafeRelease(DX_BlendState);

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
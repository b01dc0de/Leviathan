#include "Graphics.h"
#include "Camera.h"
#include "DrawState.h"
#include "Image.h"
#include "UserInterface.h"

#include "Game/Tetris.h"

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

        DrawStateT DrawStateColor;
        DrawStateT DrawStateTexture;
        DrawStateT DrawStateUnicolor;
        DrawStateT DrawStateInstRect;

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
        ID3D11Buffer* DX_BatchedQuadCmdsBuffer = nullptr;

        ID3D11Texture2D* DX_DebugTexture = nullptr;
        ID3D11ShaderResourceView* DX_DebugTextureSRV = nullptr;
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

    Array<InstQuadColorData> DrawCmds;

    const float TestInstRectSize = 50.0f;
    const float RightRectX = (float)AppWidth - TestInstRectSize;
    const float BottomRectY = (float)AppHeight - TestInstRectSize;
    InstQuadColorData InstRectDataArray[] =
    {
        { {0.0f, 0.0f, TestInstRectSize, TestInstRectSize}, {0.0f, 1.0f, 1.0f, 1.0f} },
        { {RightRectX, 0.0f, TestInstRectSize, TestInstRectSize}, {1.0f, 0.0f, 1.0f, 1.0f} },
        { {0.0f, BottomRectY, TestInstRectSize, TestInstRectSize}, { 1.0f, 1.0f, 0.0f, 1.0f } },
        { {RightRectX, BottomRectY, TestInstRectSize, TestInstRectSize}, {0.0f, 0.0f, 0.0f, 1.0f} }
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

    Array<InstQuadColorData> BatchedQuadCmds(256);
    void Graphics::UpdateAndDraw()
    {
        // Get instanced draw commands from game (Tetris):
        BatchedQuadCmds.Empty();
        Game::Tetris::UpdateAndDraw(BatchedQuadCmds);

        DX_ImmediateContext->RSSetState(DX_RasterizerState);
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

        static bool bDrawLines = false;
        if (bDrawLines)
        {
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


        static bool bDrawTriangle = false;
        if (bDrawTriangle)
        { // Draw triangle
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

        static bool bDrawTexQuad = false;
        if (bDrawTexQuad)
        { // Draw tex quad
            m4f TexQuadWorld = m4f::Scale(100.0f, 100.0f, 1.0f) * m4f::Trans(-256.0f, +256.0f, 0.0f);
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
            DX_ImmediateContext->PSSetShaderResources(0, 1, &DX_DebugTextureSRV);
            DX_ImmediateContext->PSSetSamplers(0, 1, &DX_DefaultSamplerState);

            DX_ImmediateContext->DrawIndexed(MeshStateQuad.NumInds, 0u, 0u);
        }

        static bool bDrawUnicolorQuad = false;
        if (bDrawUnicolorQuad)
        {
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

        const float HalfWidth = (float)AppWidth / 2.0f;
        const float HalfHeight = (float)AppHeight / 2.0f;
        static bool bDrawInstRects = true;
        if (bDrawInstRects)
        { // Draw Instanced Rects
            DX_ImmediateContext->UpdateSubresource(DX_WBuffer, 0, nullptr, &DefaultSpriteWorld, sizeof(m4f), 0);
            DX_ImmediateContext->UpdateSubresource(DX_VPBuffer, 0, nullptr, &OrthoCamera.View, sizeof(Camera), 0);

            ID3D11Buffer* VxInstBuffers[] = { MeshStateSpriteQuad.VxBuffer, DX_InstRectBuffer };
            const UINT Strides[] = { sizeof(VxMin), sizeof(InstQuadColorData) };
            const UINT Offsets[] = { 0, 0 };
            ASSERT((ARRAY_SIZE(VxInstBuffers) == ARRAY_SIZE(Strides)) && (ARRAY_SIZE(VxInstBuffers) == ARRAY_SIZE(Offsets)));
            DX_ImmediateContext->IASetInputLayout(DrawStateInstRect.InputLayout);
            DX_ImmediateContext->IASetVertexBuffers(0, ARRAY_SIZE(VxInstBuffers), VxInstBuffers, Strides, Offsets);
            DX_ImmediateContext->IASetIndexBuffer(MeshStateSpriteQuad.IxBuffer, DXGI_FORMAT_R32_UINT, 0);
            DX_ImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

            DX_ImmediateContext->VSSetShader(DrawStateInstRect.VertexShader, nullptr, 0);
            DX_ImmediateContext->VSSetConstantBuffers(VPBufferSlot, 1, &DX_VPBuffer);
            DX_ImmediateContext->PSSetShader(DrawStateInstRect.PixelShader, nullptr, 0);

            DX_ImmediateContext->DrawIndexedInstanced(MeshStateSpriteQuad.NumInds, ARRAY_SIZE(InstRectDataArray), 0u, 0, 0u);
        }

        static bool bDrawTetris = true;
        if (bDrawTetris && BatchedQuadCmds.Num > 0)
        {
            DX_ImmediateContext->UpdateSubresource(DX_WBuffer, 0, nullptr, &DefaultSpriteWorld, sizeof(m4f), 0);
            DX_ImmediateContext->UpdateSubresource(DX_VPBuffer, 0, nullptr, &OrthoCamera.View, sizeof(Camera), 0);

            { // Send BatchedCmds state to GPU
                D3D11_MAPPED_SUBRESOURCE MappedBatchCmds = {};
                DX_ImmediateContext->Map(DX_BatchedQuadCmdsBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedBatchCmds);
                memcpy(MappedBatchCmds.pData, BatchedQuadCmds.Data, sizeof(InstQuadColorData) * BatchedQuadCmds.Num);
                DX_ImmediateContext->Unmap(DX_BatchedQuadCmdsBuffer, 0);
            }

            ID3D11Buffer* VxInstBuffers[] = { MeshStateSpriteQuad.VxBuffer, DX_BatchedQuadCmdsBuffer };
            const UINT Strides[] = { sizeof(VxMin), sizeof(InstQuadColorData) };
            const UINT Offsets[] = { 0, 0 };
            ASSERT((ARRAY_SIZE(VxInstBuffers) == ARRAY_SIZE(Strides)) && (ARRAY_SIZE(VxInstBuffers) == ARRAY_SIZE(Offsets)));
            DX_ImmediateContext->IASetInputLayout(DrawStateInstRect.InputLayout);
            DX_ImmediateContext->IASetVertexBuffers(0, ARRAY_SIZE(VxInstBuffers), VxInstBuffers, Strides, Offsets);
            DX_ImmediateContext->IASetIndexBuffer(MeshStateSpriteQuad.IxBuffer, DXGI_FORMAT_R32_UINT, 0);
            DX_ImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

            DX_ImmediateContext->VSSetShader(DrawStateInstRect.VertexShader, nullptr, 0);
            DX_ImmediateContext->VSSetConstantBuffers(VPBufferSlot, 1, &DX_VPBuffer);
            DX_ImmediateContext->PSSetShader(DrawStateInstRect.PixelShader, nullptr, 0);

            DX_ImmediateContext->DrawIndexedInstanced(MeshStateSpriteQuad.NumInds, BatchedQuadCmds.Num, 0u, 0, 0u);
        }

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
            UserInterface::Draw(D2_RenderTarget);
        }

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

        D3D11_RENDER_TARGET_BLEND_DESC RenderTargetBlendDesc = {};
        RenderTargetBlendDesc.BlendEnable = TRUE;
        RenderTargetBlendDesc.SrcBlend = D3D11_BLEND_SRC_ALPHA;
        RenderTargetBlendDesc.DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
        RenderTargetBlendDesc.BlendOp = D3D11_BLEND_OP_ADD;
        RenderTargetBlendDesc.SrcBlendAlpha = D3D11_BLEND_ONE;
        RenderTargetBlendDesc.DestBlendAlpha = D3D11_BLEND_ZERO;
        RenderTargetBlendDesc.BlendOpAlpha = D3D11_BLEND_OP_ADD;
        RenderTargetBlendDesc.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALPHA;
        D3D11_BLEND_DESC BlendDesc = {};
        BlendDesc.RenderTarget[0] = RenderTargetBlendDesc;
        ID3D11BlendState* DX_BlendState = nullptr;
        DX_CHECK(DX_Device->CreateBlendState(&BlendDesc, &DX_BlendState));

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

        { // InstRect
            const wchar_t* InstRectShaderFilename = L"src/hlsl/InstRectShader.hlsl";
            const D3D_SHADER_MACRO DefinesInst[] =
            {
                nullptr, nullptr
            };
            D3D11_INPUT_ELEMENT_DESC InputLayoutDesc[] =
            {
                { "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
                { "RECT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
                { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 }
            };

            DrawStateInstRect = CreateDrawState(DX_Device, InstRectShaderFilename, DefinesInst, InputLayoutDesc, ARRAY_SIZE(InputLayoutDesc));
        }

        D3D11_BUFFER_DESC InstRectBufferDesc = {};
        InstRectBufferDesc.ByteWidth = sizeof(InstRectDataArray);
        InstRectBufferDesc.Usage = D3D11_USAGE_DEFAULT;
        InstRectBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        InstRectBufferDesc.CPUAccessFlags = 0;
        InstRectBufferDesc.StructureByteStride = 0;
        D3D11_SUBRESOURCE_DATA InstRectBufferInitData = { InstRectDataArray, 0, 0 };
        DX_CHECK(DX_Device->CreateBuffer(&InstRectBufferDesc, &InstRectBufferInitData, &DX_InstRectBuffer));

        D3D11_BUFFER_DESC BatchedQuadCmdsBufferDesc = {};
        BatchedQuadCmdsBufferDesc.ByteWidth = sizeof(InstQuadColorData) * BatchedQuadCmds.Capacity;
        BatchedQuadCmdsBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
        BatchedQuadCmdsBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        BatchedQuadCmdsBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        BatchedQuadCmdsBufferDesc.MiscFlags = 0;
        BatchedQuadCmdsBufferDesc.StructureByteStride = 0;
        DX_CHECK(DX_Device->CreateBuffer(&BatchedQuadCmdsBufferDesc, nullptr, &DX_BatchedQuadCmdsBuffer));

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

        {
            ImageT DebugImage = {};
            GetDebugImage(DebugImage);

            D3D11_SUBRESOURCE_DATA DebugTextureDataDesc[] = { {} };
            DebugTextureDataDesc[0].pSysMem = DebugImage.PxBuffer;
            DebugTextureDataDesc[0].SysMemPitch = sizeof(RGBA32) * DebugImage.Width;
            DebugTextureDataDesc[0].SysMemSlicePitch = DebugImage.PxBytes;
            D3D11_TEXTURE2D_DESC DebugTextureDesc = {};
            DebugTextureDesc.Width = DebugImage.Width;
            DebugTextureDesc.Height = DebugImage.Height;
            DebugTextureDesc.MipLevels = 1;
            DebugTextureDesc.ArraySize = 1;
            DebugTextureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
            DebugTextureDesc.SampleDesc.Count = 1;
            DebugTextureDesc.SampleDesc.Quality = 0;
            DebugTextureDesc.Usage = D3D11_USAGE_DEFAULT;
            DebugTextureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
            DebugTextureDesc.CPUAccessFlags = 0;
            DebugTextureDesc.MiscFlags = 0;
            DX_CHECK(DX_Device->CreateTexture2D(&DebugTextureDesc, &DebugTextureDataDesc[0], &DX_DebugTexture));
            DX_CHECK(DX_Device->CreateShaderResourceView(DX_DebugTexture, nullptr, &DX_DebugTextureSRV));

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

        Game::Tetris::Init();
    }

    void Graphics::Term()
    {
        Game::Tetris::Term();

        UserInterface::Term();

        { // Direct2D:
            DX_SAFE_RELEASE(D2_Factory);
            DX_SAFE_RELEASE(DXGI_Surface);
            DX_SAFE_RELEASE(D2_RenderTarget);
            DX_SAFE_RELEASE(D2_LinearGradientBrush);
            DX_SAFE_RELEASE(D2_GradientStops);
            DX_SAFE_RELEASE(D2_LightGrayBrush);
        }

        SafeRelease(MeshStateTriangle);
        SafeRelease(MeshStateCube);
        SafeRelease(MeshStateQuad);
        SafeRelease(MeshStateMinQuad);
        SafeRelease(MeshStateSpriteQuad);

        DX_SAFE_RELEASE(DX_WBuffer);
        DX_SAFE_RELEASE(DX_VPBuffer);
        DX_SAFE_RELEASE(DX_UnicolorBuffer);
        DX_SAFE_RELEASE(DX_InstRectBuffer);

        SafeRelease(DrawStateColor);
        SafeRelease(DrawStateTexture);
        SafeRelease(DrawStateUnicolor);
        SafeRelease(DrawStateInstRect);

        DX_SAFE_RELEASE(DXGI_SwapChain1);
        DX_SAFE_RELEASE(DX_Backbuffer);
        DX_SAFE_RELEASE(DX_RenderTargetView);
        DX_SAFE_RELEASE(DX_RasterizerState);
        DX_SAFE_RELEASE(DX_DepthStencil);
        DX_SAFE_RELEASE(DX_DepthStencilView);

        DX_SAFE_RELEASE(DXGI_Factory2);
        DX_SAFE_RELEASE(DX_ImmediateContext);

        if (DX_Device)
        {
            ID3D11Debug* DX_Debug = nullptr;
            DX_Device->QueryInterface(DX_UUID_HELPER(ID3D11Debug, DX_Debug));
            if (DX_Debug) { DX_Debug->ReportLiveDeviceObjects(D3D11_RLDO_IGNORE_INTERNAL); }
            DX_SAFE_RELEASE(DX_Debug);
        }

        DX_SAFE_RELEASE(DX_Device);
    }
}
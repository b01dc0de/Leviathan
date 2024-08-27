#ifndef ATOLLGRAPHICS_H
#define ATOLLGRAPHICS_H

#include "AtollCommon.h"
#include "AtollDrawPipelineState.h"

#define DXCHECK(Result) if (FAILED(Result)) { return -1; }
#define DXCHECKMSG(Result, Msg) if (FAILED(Result)) { OutputDebugStringA((Msg)); return -1; }

namespace ShaderAtoll
{
	struct VF4
	{
		float X;
		float Y;
		float Z;
		float W;
	};

	struct VertexColor
	{
		VF4 Position;
		VF4 Color;
	};

	struct TriIx
	{
		using IxType = unsigned int;
		IxType Ix0;
		IxType Ix1;
		IxType Ix2;
	};
	constexpr unsigned int TriIxCount = sizeof(TriIx) / sizeof(TriIx::IxType);

	constexpr float msPerFrame_60 = 1000.0f / 60.0f;
	struct ShaderGlobals
	{
		float AppTime_s;
		float DeltaTime_ms;
		int FrameWidth;
		int FrameHeight;
		int MouseX;
		int MouseY;
		int Pad0;
		int Pad1;
	};

	struct AtollGraphics
	{
		static DrawPipelineState MainDrawPipelineState;

		static IDXGISwapChain1* DX_SwapChain1;
		static ID3D11Device* DX_Device;
		static D3D_FEATURE_LEVEL UsedFeatureLevel;
		static ID3D11DeviceContext* DX_ImmediateContext;

		static ID3D11Texture2D* DX_BackBuffer;
		static ID3D11Texture2D* DX_RenderTargetTexture;
		static ID3D11RenderTargetView* DX_RenderTargetView;

		static IDXGIFactory2* DX_Factory2;

		static ID3D11RasterizerState* DX_RasterizerState;
		static ID3D11Texture2D* DX_DepthStencil;
		static ID3D11DepthStencilView* DX_DepthStencilView;
		static ID3D11BlendState* DX_BlendState;

		static ID3D11Buffer* DX_VertexBuffer;
		static ID3D11Buffer* DX_IndexBuffer;
		static ID3D11Buffer* DX_GlobalsBuffer;

		static ID3D11VertexShader* DX_VertexShader;
		static ID3D11PixelShader* DX_PixelShader;
		static ID3D11InputLayout* DX_InputLayout;

		static int InitGraphics();
		static void UpdateGraphicsState();
		static void Draw();
	};
}

#endif // ATOLLGRAPHICS_H
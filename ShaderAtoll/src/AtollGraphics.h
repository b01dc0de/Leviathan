#ifndef ATOLLGRAPHICS_H
#define ATOLLGRAPHICS_H

#include "AtollCommon.h"

#define DXCHECK(Result) if (FAILED(Result)) { return -1; }
#define DXCHECKMSG(Result, Msg) if (FAILED(Result)) { OutputDebugStringA((Msg)); return -1; }

namespace ShaderAtoll
{
	struct AtollGraphics
	{
		static IDXGISwapChain* DX_SwapChain;
		static ID3D11Device* DX_Device;
		static D3D_FEATURE_LEVEL UsedFeatureLevel;
		static ID3D11DeviceContext* DX_ImmediateContext;

		static ID3D11Texture2D* DX_BackBuffer;
		static ID3D11RenderTargetView* DX_RenderTargetView;

		static IDXGIFactory1* DX_Factory;
		static std::vector<IDXGIAdapter*> DX_AdapterList;
		static std::vector<IDXGIOutput*> DX_Outputs;
		static std::vector<DXGI_MODE_DESC*> OutputModeDescList;

		static ID3D11RasterizerState* DX_RasterizerState;
		static ID3D11Texture2D* DX_DepthStencil;
		static ID3D11DepthStencilView* DX_DepthStencilView;
		static ID3D11BlendState* DX_BlendState;

		static ID3D11Buffer* DX_VertexBuffer;
		static ID3D11Buffer* DX_IndexBuffer;

		static ID3D11VertexShader* DX_VertexShader;
		static ID3D11PixelShader* DX_PixelShader;
		static ID3D11InputLayout* DX_InputLayout;

		static int CompileShaderHelper(LPCWSTR SourceFileName, LPCSTR EntryPointFunction, LPCSTR Profile, ID3DBlob** ShaderBlob);
		static int InitGraphics();
		static void UpdateGraphicsState();
		static void Draw();
	};

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
		unsigned int Ix0;
		unsigned int Ix1;
		unsigned int Ix2;
	};
}

#endif // ATOLLGRAPHICS_H
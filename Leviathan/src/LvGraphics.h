#ifndef LVGRAPHICS_H
#define LVGRAPHICS_H

#include "LvCommon.h"

#include <d3d11.h>
#include <d3dcompiler.h>
#include <dxgi.h>

#if PLATFORM_WINDOWS()
	#define DXCHECK(DXResult) Assert(!FAILED((DXResult)))
#else
	#define DXCHECK(DXResult) void(0)
#endif

namespace Leviathan
{
	template <typename T>
	struct DXHandle
	{
		T* Ptr = nullptr;

		void DxSafeRelease() { if (Ptr) { Ptr->Release(); Ptr = nullptr; } }
		DXHandle(T* InPtr) : Ptr(InPtr) {}
		~DXHandle() { DxSafeRelease(); }
		operator T*() { return Ptr; }
		T* operator ->() { return Ptr; }
		T* operator *() { return Ptr; }
		T** operator &() { return &Ptr; }
	};

	using R32 = float;

	struct VertexR32
	{
		R32 X = 0.0f;
		R32 Y = 0.0f;
		R32 Z = 0.0f;
		R32 W = 0.0f;
	};

	struct MatrixR32
	{
		VertexR32 Row0;
		VertexR32 Row1;
		VertexR32 Row2;
		VertexR32 Row3;

		void Identity()
		{
			Row0 = { 1.0f, 0.0f, 0.0f, 0.0f };
			Row1 = { 0.0f, 1.0f, 0.0f, 0.0f };
			Row2 = { 0.0f, 0.0f, 1.0f, 0.0f };
			Row3 = { 0.0f, 0.0f, 0.0f, 1.0f };
		}
	};

	struct WorldViewProjData
	{
		MatrixR32 World;
		MatrixR32 View;
		MatrixR32 Proj;
	};

	struct VertexColor
	{
		VertexR32 Position;
		VertexR32 Color;
	};

	struct LvGraphics
	{
		D3D_FEATURE_LEVEL UsedFeatureLevel = D3D_FEATURE_LEVEL_11_0;

		DXHandle<IDXGISwapChain> DX_SwapChain = nullptr;
		DXHandle<ID3D11Device> DX_Device = nullptr;
		DXHandle<ID3D11DeviceContext> DX_ImmediateContext = nullptr;

		DXHandle<ID3D11Texture2D> DX_BackBuffer = nullptr;
		DXHandle<ID3D11RenderTargetView> DX_RenderTargetView = nullptr;

		DXHandle<IDXGIFactory1> DX_Factory = nullptr;
		std::vector<DXHandle<IDXGIAdapter>> DX_AdapterList;
		std::vector<DXHandle<IDXGIOutput>> DX_Outputs;
		std::vector<DXGI_MODE_DESC*> OutputModeDescList;

		DXHandle<ID3D11RasterizerState> DX_RasterizerState = nullptr;
		DXHandle<ID3D11Texture2D> DX_DepthStencil = nullptr;
		DXHandle<ID3D11DepthStencilView> DX_DepthStencilView = nullptr;
		DXHandle<ID3D11BlendState> DX_BlendState = nullptr;

		DXHandle<ID3D11VertexShader> DX_VertexShader = nullptr;
		DXHandle<ID3D11PixelShader> DX_PixelShader = nullptr;
		DXHandle<ID3D11InputLayout> DX_InputLayout = nullptr;

		DXHandle<ID3D11Buffer> DX_VertexBuffer = nullptr;
		DXHandle<ID3D11Buffer> DX_IndexBuffer = nullptr;

		DXHandle<ID3D11Buffer> DX_WorldBuffer = nullptr;
		DXHandle<ID3D11Buffer> DX_ViewProjBuffer = nullptr;

		static LvGraphics* PvInst;

		static LvGraphics* Inst()
		{
			if (!PvInst)
			{
				PvInst = new LvGraphics;
			}
			return PvInst;
		}
		LvGraphics() = default;
		~LvGraphics();

		int CompileShader(LPCWSTR SourceFileName, LPCSTR EntryPointFunction, LPCSTR Profile, ID3DBlob** ShaderBlob);

		void PvInit();
		void PvUpdateState();
		void PvDraw();

		static void Init() { Inst()->PvInit(); }
		static void Term() { Assert(PvInst); delete PvInst; PvInst = nullptr; }
		static void Draw() { Inst()->PvDraw(); }
	};
}

#endif // LVGRAPHICS_H
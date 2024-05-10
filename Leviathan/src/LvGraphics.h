#ifndef LVGRAPHICS_H
#define LVGRAPHICS_H

#include "LvCommon.h"

#include <d3d11.h>
#include <d3d11_1.h>
//#include <d3d11_2.h>
//#include <d3d11_3.h>
//#include <d3d11_4.h>
#include <d3dcommon.h>
#include <d3dcompiler.h>
#include <dxgi.h>
#include <dxgi1_2.h>
#include <dxgidebug.h>

#define DXCHECK(DXResult) Assert(!FAILED((DXResult)))

namespace Leviathan
{
	template <typename T>
	struct DXHandle
	{
		T* Ptr = nullptr;

		void SafeRelease() { if (Ptr) { Ptr->Release(); Ptr = nullptr; } }
		DXHandle(T* InPtr) : Ptr(InPtr) {}
		~DXHandle()
		{ 
			// CKA_TODO: We don't want to SafeRelase here so we can control our COM references
			// - Instead for now just assert that the handle has been cleared
			Assert(nullptr == Ptr);
			//SafeRelease();
		}

		operator T*() { return Ptr; }
		T* operator ->() { return Ptr; }
		T* operator *() { return Ptr; }
		T** operator &() { return &Ptr; }
	};

	struct BasicMeshColor;
	struct BasicMeshUV;

	struct LvGraphics
	{
		D3D_FEATURE_LEVEL UsedFeatureLevel = D3D_FEATURE_LEVEL_11_1;
		DXGI_FORMAT RenderTargetFormat = DXGI_FORMAT_R8G8B8A8_UNORM;

		BasicMeshColor* pCubeMesh = nullptr;
		BasicMeshColor* pTriangleMesh = nullptr;
		BasicMeshUV* pRectUVMesh = nullptr;

		DXHandle<ID3D11Device> DX_Device = nullptr;
		DXHandle<ID3D11DeviceContext> DX_ImmediateContext = nullptr;
		DXHandle<IDXGISwapChain1> DX_SwapChain1 = nullptr;
		DXHandle<ID3D11Texture2D> DX_BackBuffer = nullptr;

		DXHandle<ID3D11Texture2D> DX_RenderTargetTexture = nullptr;
		DXHandle<ID3D11RenderTargetView> DX_RenderTargetView = nullptr;
		DXHandle<ID3D11DepthStencilState> DX_DepthStencilState = nullptr;
		DXHandle<ID3D11Texture2D> DX_DepthStencilTexture = nullptr;
		DXHandle<ID3D11DepthStencilView> DX_DepthStencilView = nullptr;
		DXHandle<ID3D11RasterizerState> DX_RasterizerState = nullptr;
		DXHandle<ID3D11BlendState> DX_BlendState = nullptr;

		DXHandle<ID3D11VertexShader> DX_VSColor = nullptr;
		DXHandle<ID3D11PixelShader> DX_PSColor = nullptr;
		DXHandle<ID3D11InputLayout> DX_InputLayoutColor = nullptr;

		DXHandle<ID3D11Buffer> DX_CubeVertexBuffer = nullptr;
		DXHandle<ID3D11Buffer> DX_CubeIndexBuffer = nullptr;

		DXHandle<ID3D11VertexShader> DX_VSTexture = nullptr;
		DXHandle<ID3D11PixelShader> DX_PSTexture = nullptr;
		DXHandle<ID3D11InputLayout> DX_InputLayoutTexture = nullptr;

		DXHandle<ID3D11Buffer> DX_RectVertexBuffer = nullptr;
		DXHandle<ID3D11Buffer> DX_RectIndexBuffer = nullptr;

		DXHandle<ID3D11Buffer> DX_WorldBuffer = nullptr;
		DXHandle<ID3D11Buffer> DX_ViewProjBuffer = nullptr;

		DXHandle<ID3D11Texture2D> DX_DefaultTexture = nullptr;
		DXHandle<ID3D11ShaderResourceView> DX_DefaultTexture_SRV = nullptr;
		DXHandle<ID3D11SamplerState> DX_DefaultSamplerState = nullptr;

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

		//int CompileShader(LPCWSTR SourceFileName, LPCSTR EntryPointFunction, LPCSTR Profile, ID3DBlob** ShaderBlob);
		int CompileShader(LPCWSTR SourceFileName, LPCSTR EntryPointFunction, LPCSTR Profile, ID3DBlob** ShaderBlob, D3D_SHADER_MACRO* pOptDefines = nullptr);

		void PvInit();
		void PvUpdateState();
		void PvUpdateAndDraw();
		void PvSetDXDBGNames();
		void PvInitFallbackTexture();

		static void Init();
		static void Term();
		static void UpdateAndDraw();
	};
}

#endif // LVGRAPHICS_H
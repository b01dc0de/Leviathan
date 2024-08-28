#ifndef ATOLLGRAPHICS_H
#define ATOLLGRAPHICS_H

#include "AtollCommon.h"
#include "LvShared.h"
#include "AtollDrawPipelineState.h"

#define DXCHECK(Result) if (FAILED(Result)) { DebugBreak(); }
#define DXCHECKMSG(Result, Msg) if (FAILED(Result)) { OutputDebugStringA((Msg)); DebugBreak(); }

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

	struct DXHandleMgr
	{
		Lv::DynArray<IUnknown**> HandleList;

		static DXHandleMgr* _Inst;
		static DXHandleMgr* Inst();

		static void Add(IUnknown** InHandle);
		static void SafeReleaseAll();
		static void Term();
	};

	template <typename DX_T>
	struct DXHandle
	{
		DX_T* Handle = nullptr;

		DXHandle(DX_T* InHandle = nullptr)
			: Handle(InHandle)
		{
			DXHandleMgr::Add((IUnknown**)&Handle);
		}
		DXHandle(const DXHandle&) = default;
		DXHandle& operator=(const DXHandle&) = default;
		~DXHandle() = default;
		DXHandle(DXHandle&&) = delete;
		DXHandle& operator=(DXHandle&&) = delete;
		DXHandle& operator=(DX_T* InHandle)
		{
			Handle = InHandle;
			return *this;
		}
		DX_T** operator&() { return &Handle; }
		DX_T* operator->() { return Handle; }
		operator bool() { return nullptr != Handle; }
		operator DX_T*() { return Handle; }
	};

	struct AtollGraphics
	{
		static DXHandle<IDXGISwapChain1> DX_SwapChain1;
		static DXHandle<ID3D11Device> DX_Device;
		static DXHandle<ID3D11DeviceContext> DX_ImmediateContext;
		static DXHandle<ID3D11Texture2D> DX_BackBuffer;
		static DXHandle<ID3D11Texture2D> DX_RenderTargetTexture;
		static DXHandle<ID3D11RenderTargetView> DX_RenderTargetView;
		static DXHandle<IDXGIFactory2> DX_Factory2;
		static DXHandle<ID3D11RasterizerState> DX_RasterizerState;
		static DXHandle<ID3D11Texture2D> DX_DepthStencil;
		static DXHandle<ID3D11DepthStencilView> DX_DepthStencilView;
		static DXHandle<ID3D11BlendState> DX_BlendState;
		static DXHandle<ID3D11Buffer> DX_VertexBuffer;
		static DXHandle<ID3D11Buffer> DX_IndexBuffer;
		static DXHandle<ID3D11Buffer> DX_GlobalsBuffer;

		static D3D_FEATURE_LEVEL UsedFeatureLevel;
		static DrawPipelineState MainDrawPipelineState;

		static int InitGraphics();
		static void UpdateGraphicsState();
		static void RecompileShaders();
		static void Draw();
		static int TermGraphics();
	};
}

#endif // ATOLLGRAPHICS_H
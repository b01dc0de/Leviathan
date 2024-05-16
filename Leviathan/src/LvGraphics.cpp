#include "LvCommon.h"

#include "LvGraphics.h"
#include "LvEngine.h"
#include "LvMesh.h"
#include "LvCamera.h"

#include "LvPredefGraphics.inl"

namespace Leviathan
{
	struct LvGraphicsInstance
	{
		LvPredefGraphics PredefGFX;

		LvDrawState<VertexColor> VxColorCube_DrawState;
		LvDrawState<VertexUV> VxUVRect_DrawState;

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

		BasicMeshColor* pMeshCube = nullptr;
		BasicMeshColor* pMeshTriangle = nullptr;
		BasicMeshUV* pMeshRect = nullptr;

		u64 RenderFrameID = 0;

		static LvGraphicsInstance* PvInst;
		static LvGraphicsInstance* Inst()
		{
			if (!PvInst)
			{
				PvInst = new LvGraphicsInstance;
			}
			return PvInst;
		}
		LvGraphicsInstance() = default;
		~LvGraphicsInstance();

		void PvInit();
		void PvUpdateAndDraw();
		void PvBeginDraw();
		void PvEndDraw();
	};

	LvGraphicsInstance* LvGraphicsInstance::PvInst = nullptr;

	LvGraphicsInstance::~LvGraphicsInstance()
	{
		/*
			- CKA_NOTE: Order of DX initialization, last updated 5/7/24
			- Only care because it's generally considered good practice to
			- release these handles in reverse order of initialization
				Device related:
				- SwapChain, Device, ImmediateContext (D3D11CreateDeviceAndSwapChain)
				- BackBuffer (SwapChain::GetBuffer)
				- RenderTargetTexture (Device::CreateTexture2D)
				- RenderTargetView (Device::CreateRenderTargetView)
				- DepthStencil (Device::CreateTexture2D)

				- RasterizerState (Device::CreateRasterizerState)
				- BlendState (Device::CreateBlendState)

				Draw Pipeline related:
				Shader/IA:
					- VertexShader (Device::CreateVertexShaer)
					- PixelShader (Device::CreatePixelShader)
					- InputLayout (Device::CreateInputLayout)
				CBuffers+PrimitiveData:
					- VertexBuffer (Device::CreateBuffer)
					- IndexBuffer (Device::CreateBuffer)
					- WorldViewProjBuffer (Device::CreateBuffer)
		*/
		//Outf("ReportLiveObjects -- Before SafeReleasing DXHandles:\n");
		//ReportLiveObjects(DX_Device);

		PredefGFX.TermState();

		delete pMeshCube;
		delete pMeshTriangle;
		delete pMeshRect;

		// Release resources in (generally) reverse init order

		DX_WorldBuffer.SafeRelease();
		DX_ViewProjBuffer.SafeRelease();

		DX_RectVertexBuffer.SafeRelease();
		DX_RectIndexBuffer.SafeRelease();
		DX_CubeVertexBuffer.SafeRelease();
		DX_CubeIndexBuffer.SafeRelease();

		DX_InputLayoutTexture.SafeRelease();
		DX_VSTexture.SafeRelease();
		DX_PSTexture.SafeRelease();
		DX_InputLayoutColor.SafeRelease();
		DX_VSColor.SafeRelease();
		DX_PSColor.SafeRelease();

		DX_RasterizerState.SafeRelease();
		DX_BlendState.SafeRelease();
		DX_DepthStencilState.SafeRelease();
		DX_DepthStencilView.SafeRelease();
		DX_DepthStencilTexture.SafeRelease();
		DX_RenderTargetView.SafeRelease();
		DX_RenderTargetTexture.SafeRelease();

		DX_BackBuffer.SafeRelease();
		DX_SwapChain1.SafeRelease();

		DX_ImmediateContext->ClearState();
		DX_ImmediateContext->Flush();
		DX_ImmediateContext.SafeRelease();

	#if LV_CONFIG_DEBUG()
		LvGraphicsUtils::ReportLiveObjects(DX_Device);
	#endif // LV_CONFIG_DEBUG

		// CKA_NOTE: Disabled the exception detailed below in VS. Throws exceptions on following line:
		DX_Device.SafeRelease();
		//		Microsoft C++ exception thrown: MONZA::DdiThreadingContext<MONZA::AdapterTraits_Gen12LP>::msg_end at memory... 
		//		Microsoft C++ exception thrown: MONZA::IgcThreadingContext<MONZA::AdapterTraits_Gen12LP>::msg_end at memory...
		//		In module: igd10um64xe.dll
		//		^ Above DLL looks to be Intel Graphics related
		// Don't care enough to solve this now but should investigate in future
		DX_Device = nullptr;
	}

#if LV_CONFIG_DEBUG()
	#define DXDBG_SETDBGNAMEHELPER(DX_Handle) \
		static const char* DBGNAME##DX_Handle = #DX_Handle; \
		DX_Handle->SetPrivateData(WKPDID_D3DDebugObjectName, sizeof(#DX_Handle) - 1, DBGNAME##DX_Handle)
#else
	#define DXDBG_SETDBGNAMEHELPER(DX_Handle) LV_NOOP()
#endif // LV_CONFIG_DEBUG

	void LvGraphicsInstance::PvInit()
	{
		UINT CreateDeviceFlags = 0;
	#if LV_CONFIG_DEBUG()
		CreateDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
	#endif
		D3D_FEATURE_LEVEL DXFeatureLevel;

		// CKA_NOTE: After updating graphics drivers on my laptop, the following
		//	CreateDevice call started to throw an exception:
		//		Microsoft C++ exception: Poco::NotFoundException
		// The exception also fires when calling Release on the ID3D11Device handle at program termination
		// Disabled the exception breakpoint for now :shrug:
		DXCHECK(D3D11CreateDevice(
			nullptr,
			D3D_DRIVER_TYPE_HARDWARE,
			nullptr,
			CreateDeviceFlags,
			LvDefault_SupportedFeatureLevels,
			LvDefault_NumSupportedFeatureLevels,
			D3D11_SDK_VERSION,
			&DX_Device,
			&DXFeatureLevel,
			&DX_ImmediateContext
		));
		Assert(DXFeatureLevel == D3D_FEATURE_LEVEL_11_1);

		CreateDefaultSwapChainRenderTarget
		(
			DX_Device,
			Lv_GetWindowHandle(),
			&DX_SwapChain1,
			&DX_BackBuffer,
			&DX_RenderTargetTexture,
			&DX_RenderTargetView
		);
		CreateDefaultDepthStencilState
		(
			DX_Device,
			&DX_DepthStencilState,
			&DX_DepthStencilTexture,
			&DX_DepthStencilView
		);
		CreateDefaultRasterizerBlendState(DX_Device, &DX_RasterizerState, &DX_BlendState);

		D3D11_VIEWPORT Viewport_Desc = {};
		Viewport_Desc.Width = (FLOAT)ResX;
		Viewport_Desc.Height = (FLOAT)ResY;
		Viewport_Desc.MinDepth = 0.0f;
		Viewport_Desc.MaxDepth = 1.0f;
		Viewport_Desc.TopLeftX = 0;
		Viewport_Desc.TopLeftY = 0;
		DX_ImmediateContext->OMSetRenderTargets(1, &DX_RenderTargetView, DX_DepthStencilView);
		DX_ImmediateContext->RSSetState(DX_RasterizerState);
		DX_ImmediateContext->RSSetViewports(1, &Viewport_Desc);

		pMeshCube = InitCube();
		pMeshTriangle = InitTriangle();
		pMeshRect = InitTextureRect();

		LPCWSTR WBasicShaderFilename = L"src/HLSL/BasicShader.hlsl";
		LvGraphicsUtils::CompileShaderPipeline_Defaults<VertexColor>
		(
			*DX_Device,
			WBasicShaderFilename,
			&ColorShader_Defines[0],
			&DX_VSColor,
			&DX_PSColor,
			&DX_InputLayoutColor
		);

		LvGraphicsUtils::CompileShaderPipeline_Defaults<VertexUV>
		(
			*DX_Device,
			WBasicShaderFilename,
			&TextureShader_Defines[0],
			&DX_VSTexture,
			&DX_PSTexture,
			&DX_InputLayoutTexture
		);
		LvGraphicsUtils::CreateVxIxBuffers
		(
			*DX_Device,
			pMeshCube,
			&DX_CubeVertexBuffer,
			&DX_CubeIndexBuffer
		);

		LvGraphicsUtils::CreateVxIxBuffers
		(
			*DX_Device,
			pMeshRect,
			&DX_RectVertexBuffer,
			&DX_RectIndexBuffer
		);

		LvGraphicsUtils::CreateWVPBuffers
		(
			*DX_Device,
			&DX_WorldBuffer,
			&DX_ViewProjBuffer
		);
		
		PredefGFX.InitState(DX_Device);

		VxColorCube_DrawState.Init(
			DX_InputLayoutColor,
			DX_VSColor,
			DX_PSColor,
			DX_CubeVertexBuffer,
			DX_CubeIndexBuffer,
			DX_WorldBuffer,
			DX_ViewProjBuffer
		);
		VxUVRect_DrawState.Init(
			DX_InputLayoutTexture,
			DX_VSTexture,
			DX_PSTexture,
			DX_RectVertexBuffer,
			DX_RectIndexBuffer,
			DX_WorldBuffer,
			DX_ViewProjBuffer,
			PredefGFX.FallbackTexture_SRV,
			PredefGFX.FallbackSamplerState
		);

		RenderFrameID = 0;

	#if LV_CONFIG_DEBUG()
		{
			// Only here to avoid syntax errors from SETDBGNAMEHELPER
			ID3D11Texture2D* PredefGFX_FallbackTexture = PredefGFX.FallbackTexture;
			ID3D11ShaderResourceView* PredefGFX_FallbackTexture_SRV = PredefGFX.FallbackTexture_SRV;
			ID3D11SamplerState* PredefGFX_FallbackSamplerState = PredefGFX.FallbackSamplerState;
			DXDBG_SETDBGNAMEHELPER(DX_Device);
			DXDBG_SETDBGNAMEHELPER(DX_ImmediateContext);
			DXDBG_SETDBGNAMEHELPER(DX_SwapChain1);
			DXDBG_SETDBGNAMEHELPER(DX_BackBuffer);
			DXDBG_SETDBGNAMEHELPER(DX_RenderTargetTexture);
			DXDBG_SETDBGNAMEHELPER(DX_RenderTargetView);
			DXDBG_SETDBGNAMEHELPER(DX_DepthStencilState);
			DXDBG_SETDBGNAMEHELPER(DX_DepthStencilTexture);
			DXDBG_SETDBGNAMEHELPER(DX_DepthStencilView);
			DXDBG_SETDBGNAMEHELPER(DX_RasterizerState);
			DXDBG_SETDBGNAMEHELPER(DX_BlendState);
			DXDBG_SETDBGNAMEHELPER(DX_VSColor);
			DXDBG_SETDBGNAMEHELPER(DX_PSColor);
			DXDBG_SETDBGNAMEHELPER(DX_InputLayoutColor);
			DXDBG_SETDBGNAMEHELPER(DX_CubeVertexBuffer);
			DXDBG_SETDBGNAMEHELPER(DX_CubeIndexBuffer);
			DXDBG_SETDBGNAMEHELPER(DX_WorldBuffer);
			DXDBG_SETDBGNAMEHELPER(DX_ViewProjBuffer);
			DXDBG_SETDBGNAMEHELPER(PredefGFX_FallbackTexture);
			DXDBG_SETDBGNAMEHELPER(PredefGFX_FallbackTexture_SRV);
			DXDBG_SETDBGNAMEHELPER(PredefGFX_FallbackSamplerState);
		}
	#endif // LV_CONFIG_DEBUG
	}

	const fVector WorldPos(10.0f, 10.0f, -10.0f);
	const fVector LookAt(0.0f, 0.0f, 0.0f);
	const fVector AbsUp(0.0f, 1.0f, 0.0f, 0.0f);
	LvCameraPerspective Cam3D(WorldPos, LookAt, AbsUp);
	LvCameraOrthographic Cam2D;

	constexpr float FrameStep = 0.0166666666666667f;

	// NOTE: Do not plan to use this yet but likely will be good for future sanity checks
	static bool PvInsideDraw = false;

	void LvGraphicsInstance::PvUpdateAndDraw()
	{
		float FakeTime = FrameStep * RenderFrameID++;
		float CurrRad = FakeTime * LvMath::fPI * 2.0f;

		// Update Cam3D's world position
		{
			constexpr float CamY = 10.0f;
			constexpr float SecondsForFullRotation = 25.0f;
			constexpr float CameraPathRadius = 10.0f;
			float CamT = CurrRad / SecondsForFullRotation;
			// When FakeTime == SecondsForFullRotation, CamT == 2 * PI
			fVector CurrCamPos = {LvMath::Cosf(CamT) * CameraPathRadius, CamY, LvMath::Sinf(CamT) * CameraPathRadius * 2.0f};
			Cam3D.Orient(CurrCamPos, LookAt, AbsUp);
		}

		PvBeginDraw();

		{
			fMatrix World(SCALE, 7.5f);
			VxColorCube_DrawState.Draw(DX_ImmediateContext, pMeshCube->NumPrims, &World, &Cam3D.CamTrans.View);
		}

		{
			constexpr float ImgPathRadius = 64.0f;
			constexpr float ImgSize = 512.0f;
			constexpr float Scale = 0.25f;
			const fVector CenterPos{ -512.0f, 256.0f, 0.0f };

			float Cam2DT = CurrRad / 5.0f;
			fVector ImgPos{CenterPos.X + (LvMath::Cosf(Cam2DT) * ImgPathRadius), CenterPos.Y + (LvMath::Sinf(Cam2DT) * ImgPathRadius), 0.0f};
			float CurrScale = Scale + ((Scale/2.0f) * LvMath::Cosf(CurrRad / 6.0f));
			float ImgSzScaled = ImgSize * CurrScale;
			float CurrAngle = -CurrRad / 3.0f;
			fMatrix World = LvMath::Mult(fMatrix{SCALE, ImgSzScaled, ImgSzScaled, 1.0f}, fMatrix{ROT_Z, CurrAngle}, fMatrix{ TRANS, ImgPos });

			VxUVRect_DrawState.Draw(DX_ImmediateContext, pMeshRect->NumPrims, &World, &Cam2D.CamTrans.View);
		}

		PvEndDraw();
	}

	void LvGraphicsInstance::PvBeginDraw()
	{
		Assert(!PvInsideDraw);
		// CKA_NOTE:
		//	- This call is now necessary every time after calling SwapChain::Present
		//	- Necessary when using DXGI flip model
		DX_ImmediateContext->OMSetRenderTargets(1, &DX_RenderTargetView, DX_DepthStencilView);
		DX_ImmediateContext->OMSetDepthStencilState(DX_DepthStencilState, 0);

		DX_ImmediateContext->ClearRenderTargetView(DX_RenderTargetView, LvDefault_ClearColor);
		DX_ImmediateContext->ClearDepthStencilView(DX_DepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

		PvInsideDraw = true;
	}

	void LvGraphicsInstance::PvEndDraw()
	{
		Assert(PvInsideDraw);

		DX_ImmediateContext->ResolveSubresource(DX_BackBuffer, 0, DX_RenderTargetTexture, 0, LvDefault_RenderTargetFormat);
		const DXGI_PRESENT_PARAMETERS PresentParams = {};
		DX_SwapChain1->Present1(0, DXGI_PRESENT_ALLOW_TEARING, &PresentParams);

		PvInsideDraw = false;
	}

	namespace LvGraphics
	{
		void Init()
		{
			LV_DBGTRACESCOPE();
			LvGraphicsInstance::Inst()->PvInit();
		}

		void Term()
		{
			LV_DBGTRACESCOPE();
			Assert(LvGraphicsInstance::PvInst);
			delete LvGraphicsInstance::PvInst;
			LvGraphicsInstance::PvInst = nullptr;
		}

		void UpdateAndDraw()
		{
			LvGraphicsInstance::Inst()->PvUpdateAndDraw();
		}
	}

}


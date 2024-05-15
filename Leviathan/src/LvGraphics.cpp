#include "LvCommon.h"

#include "LvGraphics.h"
#include "LvGraphicsTypes.h"
#include "LvGraphicsUtils.h"
#include "LvEngine.h"
#include "LvMesh.h"
#include "LvCamera.h"

#include "LvPredefGraphics.inl"

namespace Leviathan
{
	struct LvGraphicsInstance
	{
		LvPredefGraphics PredefGFX;

		LvArray<IUnknown*> DX_ObjRefs;

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
	};

	LvGraphicsInstance* LvGraphicsInstance::PvInst = nullptr;

	template <typename VxType>
	struct LvDrawState
	{
		ID3D11InputLayout* InputDataFormat = nullptr;
		ID3D11VertexShader* VertexShader = nullptr;
		ID3D11PixelShader* PixelShader = nullptr;
		ID3D11Buffer* VertexBuffer = nullptr;
		ID3D11Buffer* IndexBuffer = nullptr;
		ID3D11Buffer* WorldBuffer = nullptr;
		ID3D11Buffer* ViewProjBuffer = nullptr;

		void Draw(ID3D11DeviceContext* InDeviceContext, int NumPrims, Matrix* pWorld = nullptr, Matrix* pViewProj = nullptr)
		{
			Assert(InDeviceContext);
			UINT Stride = sizeof(VxType);
			UINT Offset = 0;
			InDeviceContext->IASetPrimitiveTopology(LvDefault_Topology);
			InDeviceContext->IASetInputLayout(InputDataFormat);
			InDeviceContext->IASetVertexBuffers(0, 1, &VertexBuffer, &Stride, &Offset);
			InDeviceContext->IASetIndexBuffer(IndexBuffer, LvDefault_IndexFormat, 0);

			InDeviceContext->VSSetShader(VertexShader, nullptr, 0);
			InDeviceContext->PSSetShader(PixelShader, nullptr, 0);

			InDeviceContext->VSSetConstantBuffers(LvDefault_WorldCBufferIdx, 1, &WorldBuffer);
			InDeviceContext->VSSetConstantBuffers(LvDefault_ViewProjCBufferIdx, 1, &ViewProjBuffer);

			if (pWorld) { InDeviceContext->UpdateSubresource(WorldBuffer, 0, nullptr, pWorld, sizeof(Matrix), 0); }
			if (pViewProj) { InDeviceContext->UpdateSubresource(ViewProjBuffer, 0, nullptr, pViewProj, sizeof(Matrix) * 2, 0); }

			InDeviceContext->DrawIndexed(NumPrims * 3, 0u, 0u);
		}

		LvDrawState
		(
			ID3D11InputLayout* InIAFmt,
			ID3D11VertexShader* InVS,
			ID3D11PixelShader* InPS,
			ID3D11Buffer* InVxBuffer,
			ID3D11Buffer* InIxBuffer,
			ID3D11Buffer* InWCbuffer,
			ID3D11Buffer* InVPCbuffer
		)
			: InputDataFormat(InIAFmt)
			, VertexShader(InVS)
			, PixelShader(InPS)
			, VertexBuffer(InVxBuffer)
			, IndexBuffer(InIxBuffer)
			, WorldBuffer(InWCbuffer)
			, ViewProjBuffer(InVPCbuffer)
		{
		}
	};

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
		/*
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
		*/

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
#endif // LV_CONFIG_DEBUG

	void LvGraphicsInstance::PvInit()
	{
		auto AddDXRef = [&](IUnknown* DXPtr) -> void
		{
			DX_ObjRefs.AddItem(DXPtr);
		};

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

		DXGI_SWAP_CHAIN_DESC1 SwapChainDesc1 = {};
		SwapChainDesc1.Width = ResX;
		SwapChainDesc1.Height = ResY;
		SwapChainDesc1.Format = LvDefault_RenderTargetFormat;
		SwapChainDesc1.Stereo = FALSE;
		SwapChainDesc1.SampleDesc.Count = 1;
		SwapChainDesc1.SampleDesc.Quality = 0;
		SwapChainDesc1.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		SwapChainDesc1.BufferCount = 2;
		SwapChainDesc1.Scaling = DXGI_SCALING_STRETCH;
		SwapChainDesc1.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
		SwapChainDesc1.AlphaMode = DXGI_ALPHA_MODE_IGNORE;
		SwapChainDesc1.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING;
		
		DXHandle<IDXGIFactory2> DXGI_Factory2 = nullptr;
		DXCHECK(CreateDXGIFactory1(IID_PPV_ARGS(&DXGI_Factory2)));
		DXCHECK(DXGI_Factory2->CreateSwapChainForHwnd(
			DX_Device,
			Lv_GetWindowHandle(),
			&SwapChainDesc1,
			nullptr,
			nullptr,
			&DX_SwapChain1
		));
		DXGI_Factory2.SafeRelease();

		DXCHECK(DX_SwapChain1->GetBuffer(0, IID_PPV_ARGS(&DX_BackBuffer)));

		DXGI_SAMPLE_DESC Shared_RT_SampleDesc = {};
		Shared_RT_SampleDesc.Count = 4;
		Shared_RT_SampleDesc.Quality = (UINT)D3D11_STANDARD_MULTISAMPLE_PATTERN;

		D3D11_TEXTURE2D_DESC RTT_Desc;
		RTT_Desc.Width = ResX;
		RTT_Desc.Height = ResY;
		RTT_Desc.MipLevels = 1;
		RTT_Desc.ArraySize = 1;
		RTT_Desc.Format = LvDefault_RenderTargetFormat;
		RTT_Desc.SampleDesc = Shared_RT_SampleDesc;
		RTT_Desc.Usage = D3D11_USAGE_DEFAULT;
		RTT_Desc.BindFlags = D3D11_BIND_RENDER_TARGET;
		RTT_Desc.CPUAccessFlags = 0;
		RTT_Desc.MiscFlags = 0;
		DXCHECK(DX_Device->CreateTexture2D(&RTT_Desc, nullptr, &DX_RenderTargetTexture));
		D3D11_RENDER_TARGET_VIEW_DESC RTV_Desc = {};
		RTV_Desc.Format = RTT_Desc.Format;
		RTV_Desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DMS;
		RTV_Desc.Texture2D.MipSlice = 0;
		DXCHECK(DX_Device->CreateRenderTargetView(DX_RenderTargetTexture, nullptr, &DX_RenderTargetView));

		// CKA_NOTE: Use the default values for now
		// See: https://learn.microsoft.com/en-us/windows/win32/api/d3d11/ns-d3d11-d3d11_depth_stencil_desc
		D3D11_DEPTH_STENCIL_DESC DepthStencilStateDesc = {};
		DepthStencilStateDesc.DepthEnable = TRUE;
		DepthStencilStateDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		DepthStencilStateDesc.DepthFunc = D3D11_COMPARISON_LESS;
		DepthStencilStateDesc.StencilEnable = FALSE;
		DepthStencilStateDesc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
		DepthStencilStateDesc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;
		D3D11_DEPTH_STENCILOP_DESC DefaultStencilOpDesc = {};
		DefaultStencilOpDesc.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		DefaultStencilOpDesc.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
		DefaultStencilOpDesc.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		DefaultStencilOpDesc.StencilFunc = D3D11_COMPARISON_ALWAYS;
		DepthStencilStateDesc.FrontFace = DefaultStencilOpDesc;
		DepthStencilStateDesc.BackFace = DefaultStencilOpDesc;
		DXCHECK(DX_Device->CreateDepthStencilState(&DepthStencilStateDesc, &DX_DepthStencilState));

		D3D11_TEXTURE2D_DESC DepthStencilTextureDesc = {};
		DepthStencilTextureDesc.Width = ResX;
		DepthStencilTextureDesc.Height = ResY;
		DepthStencilTextureDesc.MipLevels = 1;
		DepthStencilTextureDesc.ArraySize = 1;
		DepthStencilTextureDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		DepthStencilTextureDesc.SampleDesc = Shared_RT_SampleDesc;
		DepthStencilTextureDesc.Usage = D3D11_USAGE_DEFAULT;
		DepthStencilTextureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		DepthStencilTextureDesc.CPUAccessFlags = 0;
		DepthStencilTextureDesc.MiscFlags = 0;
		DXCHECK(DX_Device->CreateTexture2D(&DepthStencilTextureDesc, nullptr, &DX_DepthStencilTexture));

		D3D11_DEPTH_STENCIL_VIEW_DESC DepthStencilViewDesc = {};
		DepthStencilViewDesc.Format = DepthStencilTextureDesc.Format;
		DepthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
		DepthStencilViewDesc.Texture2D.MipSlice = 0;
		DXCHECK(DX_Device->CreateDepthStencilView(DX_DepthStencilTexture, &DepthStencilViewDesc, &DX_DepthStencilView));

		D3D11_RASTERIZER_DESC RasterDesc = {};
		RasterDesc.FillMode = D3D11_FILL_SOLID;
		RasterDesc.CullMode = D3D11_CULL_BACK;
		RasterDesc.FrontCounterClockwise = true;
		RasterDesc.DepthClipEnable = true;
		RasterDesc.ScissorEnable = false;
		RasterDesc.MultisampleEnable = true;
		RasterDesc.AntialiasedLineEnable = true;
		DXCHECK(DX_Device->CreateRasterizerState(&RasterDesc, &DX_RasterizerState));

		D3D11_RENDER_TARGET_BLEND_DESC RTVBlendDesc = {};
		RTVBlendDesc.BlendEnable = true;
		RTVBlendDesc.SrcBlend = D3D11_BLEND_SRC_ALPHA;
		RTVBlendDesc.DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		RTVBlendDesc.BlendOp = D3D11_BLEND_OP_ADD;
		RTVBlendDesc.SrcBlendAlpha = D3D11_BLEND_ONE;
		RTVBlendDesc.DestBlendAlpha = D3D11_BLEND_ZERO;
		RTVBlendDesc.BlendOpAlpha = D3D11_BLEND_OP_ADD;
		RTVBlendDesc.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALPHA;
		D3D11_BLEND_DESC BlendDesc = {};
		BlendDesc.AlphaToCoverageEnable = false;
		BlendDesc.IndependentBlendEnable = false;
		BlendDesc.RenderTarget[0] = RTVBlendDesc;
		DXCHECK(DX_Device->CreateBlendState(&BlendDesc, &DX_BlendState));

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

		AddDXRef(DX_Device);
		AddDXRef(DX_SwapChain1);
		AddDXRef(DX_BackBuffer);
		AddDXRef(DX_RenderTargetTexture);
		AddDXRef(DX_RenderTargetView);
		AddDXRef(DX_DepthStencilState);
		AddDXRef(DX_DepthStencilTexture);
		AddDXRef(DX_DepthStencilView);
		AddDXRef(DX_RasterizerState);
		AddDXRef(DX_BlendState);

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
		LvGraphicsUtils::CreateWVPBuffers
		(
			*DX_Device,
			&DX_WorldBuffer,
			&DX_ViewProjBuffer
		);

		LvGraphicsUtils::CreateVxIxBuffers
		(
			*DX_Device,
			pMeshRect,
			&DX_RectVertexBuffer,
			&DX_RectIndexBuffer
		);

		PredefGFX.InitState(DX_Device);
		
		/*
		pVxColorDrawState = new LvDrawState<VertexColor>(
			DX_InputLayoutColor,
			DX_VSColor,
			DX_PSColor,
			DX_CubeVertexBuffer,
			DX_CubeIndexBuffer,
			DX_WorldBuffer,
			DX_ViewProjBuffer
		);
		//VxColorDrawState.Draw(ID3D11DeviceContext* InDeviceContext, __int32 NumPrims, Matrix* pWorld, Matrix* pViewProj)
		pVxTextureDrawState = new LvDrawState<VertexUV>(
			DX_InputLayoutTexture,
			DX_VSTexture,
			DX_PSTexture,
			DX_RectVertexBuffer,
			DX_RectIndexBuffer,
			DX_WorldBuffer,
			DX_ViewProjBuffer
		);
		*/

	#if LV_CONFIG_DEBUG()
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
		//DXDBG_SETDBGNAMEHELPER(DX_DefaultTexture);
		//DXDBG_SETDBGNAMEHELPER(DX_DefaultTexture_SRV);
		//DXDBG_SETDBGNAMEHELPER(DX_DefaultSamplerState);
	#endif // LV_DEBUG
	}

	const fVector WorldPos(10.0f, 10.0f, -10.0f);
	const fVector LookAt(0.0f, 0.0f, 0.0f);
	const fVector AbsUp(0.0f, 1.0f, 0.0f, 0.0f);
	LvCameraPerspective Cam3D(WorldPos, LookAt, AbsUp);
	LvCameraOrthographic Cam2D;

	constexpr float FrameStep = 0.0166666666666667f;
	constexpr float fPI = 3.14159265359f;

	void LvGraphicsInstance::PvUpdateAndDraw()
	{
		static int RenderIdx = 0;
		float FakeTime = FrameStep * RenderIdx++;
		float CurrRad = FakeTime * fPI * 2.0f;

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

		// CKA_NOTE:
		//	- This call is now necessary every time after calling SwapChain::Present
		//	- Necessary when using DXGI flip model
		DX_ImmediateContext->OMSetRenderTargets(1, &DX_RenderTargetView, DX_DepthStencilView);
		DX_ImmediateContext->OMSetDepthStencilState(DX_DepthStencilState, 0);

		DX_ImmediateContext->ClearRenderTargetView(DX_RenderTargetView, LvDefault_ClearColor);
		DX_ImmediateContext->ClearDepthStencilView(DX_DepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

		{
			fMatrix World(SCALE, 7.5f);

			UINT Stride = sizeof(VertexColor);
			UINT Offset = 0;
			DX_ImmediateContext->IASetInputLayout(DX_InputLayoutColor);
			DX_ImmediateContext->IASetVertexBuffers(0, 1, &DX_CubeVertexBuffer, &Stride, &Offset);
			DX_ImmediateContext->IASetIndexBuffer(DX_CubeIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
			DX_ImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

			DX_ImmediateContext->VSSetShader(DX_VSColor, nullptr, 0);
			DX_ImmediateContext->PSSetShader(DX_PSColor, nullptr, 0);

			DX_ImmediateContext->VSSetConstantBuffers(0, 1, &DX_WorldBuffer);
			DX_ImmediateContext->VSSetConstantBuffers(1, 1, &DX_ViewProjBuffer);

			DX_ImmediateContext->UpdateSubresource(DX_WorldBuffer, 0, nullptr, &World, sizeof(World), 0);
			DX_ImmediateContext->UpdateSubresource(DX_ViewProjBuffer, 0, nullptr, &Cam3D.CamTrans.View, sizeof(Cam3D.CamTrans), 0);

			DX_ImmediateContext->DrawIndexed(pMeshCube->NumPrims * 3, 0u, 0u);
		}

		{
			float ImgSize = 512.0f;
			float Scale = 0.25f;
			fVector CenterPos{ -512.0f, 256.0f, 0.0f };
			constexpr float ImgPathRadius = 64.0f;

			float Cam2DT = CurrRad / 5.0f;

			fVector ImgPos{CenterPos.X + (LvMath::Cosf(Cam2DT) * ImgPathRadius), CenterPos.Y + (LvMath::Sinf(Cam2DT) * ImgPathRadius), 0.0f};
			float CurrScale = Scale + ((Scale/2.0f) * LvMath::Cosf(CurrRad / 6.0f));
			float CurrAngle = -CurrRad / 3.0f;
			fMatrix TransMat(TRANS, ImgPos);
			fMatrix RotMat(ROT_Z, CurrAngle);
			fMatrix ScaleMat(SCALE, CurrScale * ImgSize, CurrScale * ImgSize, 1.0f);
			fMatrix World = LvMath::Mult(ScaleMat, RotMat, TransMat);

			UINT Stride = sizeof(VertexUV);
			UINT Offset = 0;
			DX_ImmediateContext->IASetInputLayout(DX_InputLayoutTexture);
			DX_ImmediateContext->IASetVertexBuffers(0, 1, &DX_RectVertexBuffer, &Stride, &Offset);
			DX_ImmediateContext->IASetIndexBuffer(DX_RectIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
			DX_ImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

			DX_ImmediateContext->VSSetShader(DX_VSTexture, nullptr, 0);
			DX_ImmediateContext->PSSetShader(DX_PSTexture, nullptr, 0);

			DX_ImmediateContext->VSSetConstantBuffers(0, 1, &DX_WorldBuffer);
			DX_ImmediateContext->VSSetConstantBuffers(1, 1, &DX_ViewProjBuffer);

			DX_ImmediateContext->PSSetShaderResources(0, 1, &PredefGFX.FallbackTexture_SRV);
			DX_ImmediateContext->PSSetSamplers(0, 1, &PredefGFX.FallbackSamplerState);

			DX_ImmediateContext->UpdateSubresource(DX_WorldBuffer, 0, nullptr, &World, sizeof(World), 0);
			DX_ImmediateContext->UpdateSubresource(DX_ViewProjBuffer, 0, nullptr, &Cam2D.CamTrans.View, sizeof(Cam2D.CamTrans), 0);

			DX_ImmediateContext->DrawIndexed(pMeshRect->NumPrims * 3, 0u, 0u);
		}

		DX_ImmediateContext->ResolveSubresource(DX_BackBuffer, 0, DX_RenderTargetTexture, 0, LvDefault_RenderTargetFormat);
		const DXGI_PRESENT_PARAMETERS PresentParams = {};
		DX_SwapChain1->Present1(0, DXGI_PRESENT_ALLOW_TEARING, &PresentParams);
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


#include "LvCommon.h"

#include "LvGraphics.h"
#include "LvEngine.h"
#include "LvMesh.h"
#include "LvCamera.h"
#include "LvBitmapFile.h"

namespace Leviathan
{
	inline namespace LvPredefGraphicsConstants
	{
		static constexpr D3D11_PRIMITIVE_TOPOLOGY LvDefault_Topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		static constexpr DXGI_FORMAT LvDefault_IndexFormat = DXGI_FORMAT_R32_UINT;
		static constexpr int LvDefault_WorldCBufferIdx = 0;
		static constexpr int LvDefault_ViewProjCBufferIdx = 1;
		static constexpr float LvDefault_ClearColor[4] = { 0.1f, 0.2f, 0.3f, 1.0f };
		static constexpr D3D_FEATURE_LEVEL LvDefault_FeatureLevel = D3D_FEATURE_LEVEL_11_1;
		static constexpr DXGI_FORMAT LvDefault_RenderTargetFormat = DXGI_FORMAT_R8G8B8A8_UNORM;

		static constexpr D3D_FEATURE_LEVEL LvDefault_SupportedFeatureLevels[] =
		{
			D3D_FEATURE_LEVEL_11_1, D3D_FEATURE_LEVEL_11_0,
		};
		static unsigned int LvDefault_NumSupportedFeatureLevels = LV_ARRAYSIZE(LvDefault_SupportedFeatureLevels);

		static constexpr DXGI_FORMAT LvDefault_TextureFormat = DXGI_FORMAT_R32G32B32A32_FLOAT;

		static constexpr D3D11_RASTERIZER_DESC WireframeRasterDesc =
		{
			D3D11_FILL_WIREFRAME, //D3D11_FILL_MODE FillMode,
			D3D11_CULL_NONE, //D3D11_CULL_MODE CullMode,
			TRUE, //BOOL FrontCounterClockwise,
			0, //INT DepthBias,
			0.0f, //FLOAT DepthBiasClamp,
			0.0f,//FLOAT SlopeScaledDepthBias;
			TRUE,//BOOL DepthClipEnable,
			FALSE,//BOOL ScissorEnable,
			TRUE,//BOOL MultisampleEnable,
			TRUE//BOOL AntialiasedLineEnable
		};

		static constexpr D3D_SHADER_MACRO ColorShader_Defines[] =
		{
			"ENABLE_WVP_TRANSFORM", "1",
			"COMBINED_WVP_BUFFER", "0",
			"ENABLE_VERTEX_TEXTURE", "0",
			"ENABLE_VERTEX_COLOR", "1",
			NULL, NULL
		};
		static constexpr D3D_SHADER_MACRO TextureShader_Defines[] =
		{
			"ENABLE_WVP_TRANSFORM", "1",
			"COMBINED_WVP_BUFFER", "0",
			"ENABLE_VERTEX_TEXTURE", "1",
			"ENABLE_VERTEX_COLOR", "0",
			NULL, NULL
		};

		static const VxInputDescType VxColor_InputLayoutDesc[] =
		{
			{"POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};
		static const VxInputDescType VxUV_InputLayoutDesc[] =
		{
			{"POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};
	}

	const LvVxInputDef LvVxInputDef::VxTypeDefTable[(ullong)LvVxType::LVVX_NUM] =
	{
		{VxColor_InputLayoutDesc, LV_ARRAYSIZE(VxColor_InputLayoutDesc)},
		{VxUV_InputLayoutDesc, LV_ARRAYSIZE(VxUV_InputLayoutDesc)}
	};

	struct LvPredefGraphics
	{
		ID3D11Texture2D* FallbackTexture = nullptr;
		ID3D11ShaderResourceView* FallbackTexture_SRV = nullptr;
		ID3D11SamplerState* FallbackSamplerState = nullptr;

		void PvInitFallbackTextureState(ID3D11Device* DXDevice);

		void InitState(ID3D11Device* DXDevice)
		{
			Assert(DXDevice);
			PvInitFallbackTextureState(DXDevice);
		}

		void TermState()
		{
			DXSAFERELEASE(FallbackTexture);
			DXSAFERELEASE(FallbackTexture_SRV);
			DXSAFERELEASE(FallbackSamplerState);
		}
	};

	void LvPredefGraphics::PvInitFallbackTextureState(ID3D11Device* DXDevice)
	{
		Lv::fRGBAColor* FallbackBitmapData = nullptr;
		int Size = Lv::FallbackTexture_Size;
		Lv::GetFallbackBitmapTexture(&FallbackBitmapData);

		D3D11_SUBRESOURCE_DATA TexDataDesc[] = { {} };
		TexDataDesc[0].pSysMem = FallbackBitmapData;
		TexDataDesc[0].SysMemPitch = sizeof(Lv::fRGBAColor) * Size;
		TexDataDesc[0].SysMemSlicePitch = sizeof(Lv::fRGBAColor) * Size * Size;

		D3D11_TEXTURE2D_DESC DefaultTextureDesc = {};
		DefaultTextureDesc.Width = Size;
		DefaultTextureDesc.Height = Size;
		DefaultTextureDesc.MipLevels = 1;
		DefaultTextureDesc.ArraySize = 1;
		DefaultTextureDesc.Format = LvDefault_TextureFormat;
		DefaultTextureDesc.SampleDesc.Count = 1;
		DefaultTextureDesc.SampleDesc.Quality = 0;
		DefaultTextureDesc.Usage = D3D11_USAGE_DEFAULT;
		DefaultTextureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		DefaultTextureDesc.CPUAccessFlags = 0;
		DefaultTextureDesc.MiscFlags = 0;
		DXCHECK(DXDevice->CreateTexture2D(&DefaultTextureDesc, &TexDataDesc[0], &FallbackTexture));

		delete[] FallbackBitmapData;

		DXCHECK(DXDevice->CreateShaderResourceView(FallbackTexture, nullptr, &FallbackTexture_SRV));

		D3D11_SAMPLER_DESC DefaultTextureSamplerDesc = {};
		DefaultTextureSamplerDesc.Filter = D3D11_FILTER_MIN_MAG_POINT_MIP_LINEAR;
		DefaultTextureSamplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		DefaultTextureSamplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		DefaultTextureSamplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		DefaultTextureSamplerDesc.MipLODBias = 0.0f;
		DefaultTextureSamplerDesc.MaxAnisotropy = 0; 
		DefaultTextureSamplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
		DefaultTextureSamplerDesc.MinLOD = 0;
		DefaultTextureSamplerDesc.MaxLOD = 0;
		DXCHECK(DXDevice->CreateSamplerState(&DefaultTextureSamplerDesc, &FallbackSamplerState));
	}

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
		ID3D11ShaderResourceView* OptFallbackTextureSRV = nullptr;
		ID3D11SamplerState* OptFallbackSampler = nullptr;


		void Draw(ID3D11DeviceContext* InDeviceContext, int NumPrims, const Matrix* pWorld = nullptr, const Matrix* pViewProj = nullptr)
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
			if (OptFallbackTextureSRV) { InDeviceContext->PSSetShaderResources(0, 1, &OptFallbackTextureSRV); }
			if (OptFallbackSampler) { InDeviceContext->PSSetSamplers(0, 1, &OptFallbackSampler); }

			InDeviceContext->VSSetConstantBuffers(LvDefault_WorldCBufferIdx, 1, &WorldBuffer);
			InDeviceContext->VSSetConstantBuffers(LvDefault_ViewProjCBufferIdx, 1, &ViewProjBuffer);

			if (pWorld) { InDeviceContext->UpdateSubresource(WorldBuffer, 0, nullptr, pWorld, sizeof(Matrix), 0); }
			if (pViewProj) { InDeviceContext->UpdateSubresource(ViewProjBuffer, 0, nullptr, pViewProj, sizeof(Matrix) * 2, 0); }

			InDeviceContext->DrawIndexed(NumPrims * 3, 0u, 0u);
		}

		void Init
		(
			ID3D11InputLayout* InIAFmt,
			ID3D11VertexShader* InVS,
			ID3D11PixelShader* InPS,
			ID3D11Buffer* InVxBuffer,
			ID3D11Buffer* InIxBuffer,
			ID3D11Buffer* InWCbuffer,
			ID3D11Buffer* InVPCbuffer,
			ID3D11ShaderResourceView* InOptFallbackTextureSRV = nullptr,
			ID3D11SamplerState* InOptFallbackSampler = nullptr
		)
		{
			InputDataFormat = InIAFmt;
			VertexShader = InVS;
			PixelShader = InPS;
			VertexBuffer = InVxBuffer;
			IndexBuffer = InIxBuffer;
			WorldBuffer = InWCbuffer;
			ViewProjBuffer = InVPCbuffer;
			OptFallbackTextureSRV = InOptFallbackTextureSRV;
			OptFallbackSampler = InOptFallbackSampler;
		}
	};

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

		DXHandle<ID3D11RasterizerState> DX_DefaultRasterizerState = nullptr;
		DXHandle<ID3D11BlendState> DX_DefaultBlendState = nullptr;

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

		// D2D:
		DXHandle<IDXGISurface> DXGI_BBSurface = nullptr;
		DXHandle<ID2D1RenderTarget> DX_D2D_RT = nullptr;
		DXHandle<ID2D1Factory> DX_D2DFactory = nullptr;
		DXHandle<ID2D1GradientStopCollection> DX_BGGradientStopCollection = nullptr;
		DXHandle<ID2D1LinearGradientBrush> DX_D2DBackgroundGradientBrush = nullptr;
		DXHandle<ID2D1SolidColorBrush> DX_GreenBrush = nullptr;

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

		{
			DXGI_BBSurface.SafeRelease();
			DX_D2DFactory.SafeRelease();
			DX_D2D_RT.SafeRelease();
			DX_BGGradientStopCollection.SafeRelease();
			DX_D2DBackgroundGradientBrush.SafeRelease();
			DX_GreenBrush.SafeRelease();
		}

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

		DX_DefaultRasterizerState.SafeRelease();
		DX_DefaultBlendState.SafeRelease();
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

	static constexpr wchar_t const* LvText_StringToRender = L"Hello world!";
	static constexpr UINT32 LvText_TextLength = LV_ARRAYSIZE(LvText_StringToRender);
	void LvGraphicsInstance::PvInit()
	{
		UINT CreateDeviceFlags = 0;
	#if LV_CONFIG_DEBUG()
		CreateDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
	#endif
		// CKA_NOTE: This might be needed for D3D + D2D interoperability
		CreateDeviceFlags |= D3D11_CREATE_DEVICE_BGRA_SUPPORT;

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
		CreateDefaultRasterizerBlendState(DX_Device, &DX_DefaultRasterizerState, &DX_DefaultBlendState);

		D3D11_VIEWPORT Viewport_Desc = {};
		Viewport_Desc.Width = (FLOAT)ResX;
		Viewport_Desc.Height = (FLOAT)ResY;
		Viewport_Desc.MinDepth = 0.0f;
		Viewport_Desc.MaxDepth = 1.0f;
		Viewport_Desc.TopLeftX = 0;
		Viewport_Desc.TopLeftY = 0;
		DX_ImmediateContext->OMSetRenderTargets(1, &DX_RenderTargetView, DX_DepthStencilView);
		DX_ImmediateContext->RSSetState(DX_DefaultRasterizerState);
		DX_ImmediateContext->RSSetViewports(1, &Viewport_Desc);

		pMeshCube = InitCube();
		pMeshTriangle = InitTriangle();
		pMeshRect = InitTextureRect();

		LPCWSTR WBasicShaderFilename = L"src/HLSL/BasicShader.hlsl";
		LvGraphicsUtils::CompileShaderPipeline_Defaults
		(
			*DX_Device,
			WBasicShaderFilename,
			LvVxType::LVVX_COLOR,
			&ColorShader_Defines[0],
			&DX_VSColor,
			&DX_PSColor,
			&DX_InputLayoutColor
		);

		LvGraphicsUtils::CompileShaderPipeline_Defaults
		(
			*DX_Device,
			WBasicShaderFilename,
			LvVxType::LVVX_UV,
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



		//DXCHECK(DX_BackBuffer->QueryInterface<IDXGISurface>(&DXGI_BBSurface));
		DXCHECK(DX_SwapChain1->GetBuffer(0, IID_PPV_ARGS(&DXGI_BBSurface)));

		//D2D1_SIZE_U RCSize = D2D1::SizeU(ClientRect.right - ClientRect.left, ClientRect.bottom - ClientRect.top);
		DXCHECK(D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &DX_D2DFactory));
		DXCHECK(DX_D2DFactory->CreateDxgiSurfaceRenderTarget(
			DXGI_BBSurface,
			D2D1::RenderTargetProperties(D2D1_RENDER_TARGET_TYPE_HARDWARE,
				// CKA_NOTE: Passing in UNKONWN will just use the existing surface's format(DXGI_FORMAT_R8G8B8A8_UNORM)
				D2D1::PixelFormat(DXGI_FORMAT_R8G8B8A8_UNORM/*DXGI_FORMAT_UNKNOWN*/, D2D1_ALPHA_MODE_IGNORE/*D2D1_ALPHA_MODE_PREMULTIPLIED*/)),
			&DX_D2D_RT));

		HWND CachedHwnd = Lv_GetWindowHandle();
		RECT ClientRect{};
		GetClientRect(CachedHwnd, &ClientRect);
		D2D1_POINT_2F TopLeft_2f{ (float)ClientRect.left, (float)ClientRect.top };
		D2D1_POINT_2F BotRight_2f{ (float)ClientRect.right, (float)ClientRect.bottom };

		D2D1_GRADIENT_STOP BGGradientStops[] = { {0.0f, D2D1::ColorF::Black}, {1.0f, D2D1::ColorF::White} };
		DXCHECK(DX_D2D_RT->CreateGradientStopCollection(
			BGGradientStops,
			LV_ARRAYSIZE(BGGradientStops),
			D2D1_GAMMA_2_2, // D2D1_GAMMA_1_0
			D2D1_EXTEND_MODE_WRAP, // D2D1_EXTEND_MODE_CLAMP D2D1_EXTEND_MODE_MIRROR
			&DX_BGGradientStopCollection));
		DXCHECK(DX_D2D_RT->CreateLinearGradientBrush(
			D2D1_LINEAR_GRADIENT_BRUSH_PROPERTIES{TopLeft_2f, BotRight_2f},
			D2D1_BRUSH_PROPERTIES{1.0f, D2D1::Matrix3x2F::Scale(DX_D2D_RT->GetSize())},
			DX_BGGradientStopCollection,
			&DX_D2DBackgroundGradientBrush
		));
		DXCHECK(DX_D2D_RT->CreateSolidColorBrush(
			{ D2D1::ColorF::Green },
			&DX_GreenBrush
		));

	#if 0
		//D2D1_RECT_F LayoutRect = D2D1::RectF((float)ClientRect.left, (float)ClientRect.top, (float)(ClientRect.right - ClientRect.left), (float)(ClientRect.bottom - ClientRect.top));
		//DrawText(LvText_StringToRender, LvText_TextLength, pTextFormat, LayoutRect, pBlackBrush);
		//CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White), &DX_WhiteBrush);
		DXHandle<IDWriteFactory> DX_WriteFactory = nullptr;
		DXHandle<IDWriteTextFormat> DX_TextFormat = nullptr;
		DXHandle<ID2D1SolidColorBrush> DX_BlackBrush = nullptr;
		DXHandle<ID2D1SolidColorBrush> DX_WhiteBrush = nullptr;

		DXCHECK(DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), (IUnknown**)&DX_WriteFactory));
		DXCHECK(DX_WriteFactory->CreateTextFormat(
			L"Consolas",
			nullptr,
			DWRITE_FONT_WEIGHT_REGULAR,
			DWRITE_FONT_STYLE_NORMAL,
			DWRITE_FONT_STRETCH_NORMAL,
			64.0f,
			L"en-US",
			&DX_TextFormat));

		DXCHECK(DX_TextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER));
		DXCHECK(DX_TextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER));
	#endif // 0

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
			DXDBG_SETDBGNAMEHELPER(DX_DefaultRasterizerState);
			DXDBG_SETDBGNAMEHELPER(DX_DefaultBlendState);
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

	constexpr float FrameStep = 0.0166666666666667f / 4.0f;

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

			float Cam2DT = CurrRad / 25.0f;
			fVector ImgPos{ CenterPos.X + (LvMath::Cosf(Cam2DT) * ImgPathRadius), CenterPos.Y + (LvMath::Sinf(Cam2DT) * ImgPathRadius), 0.0f };
			float CurrScale = Scale + ((Scale / 2.0f) * LvMath::Cosf(Cam2DT / 6.0f));
			float ImgSzScaled = ImgSize * CurrScale;
			float CurrAngle = -Cam2DT/ 3.0f;
			fMatrix World = LvMath::Mult(fMatrix{ SCALE, ImgSzScaled, ImgSzScaled, 1.0f }, fMatrix{ ROT_Z, CurrAngle }, fMatrix{ TRANS, ImgPos });

			VxUVRect_DrawState.Draw(DX_ImmediateContext, pMeshRect->NumPrims, &World, &Cam2D.CamTrans.View);

			fMatrix StillRect_World = LvMath::Mult(fMatrix{ SCALE, 128.0f, 128.0f, 1.0f }, fMatrix{TRANS, +512.0f, -256.0f, 0.0f});
			VxUVRect_DrawState.Draw(DX_ImmediateContext, pMeshRect->NumPrims, &StillRect_World, &Cam2D.CamTrans.View);
		}

		DX_D2D_RT->BeginDraw();
		DX_D2D_RT->SetTransform(D2D1::Matrix3x2F::Identity());
		DX_D2D_RT->Clear(D2D1::ColorF(D2D1::ColorF::White));


		{ // Draw gradient background
			D2D1_RECT_F rect = D2D1::RectF(0.0f, 0.0f, (float)ResX, (float)ResY);
			DX_D2D_RT->FillRectangle(rect, DX_D2DBackgroundGradientBrush);
			DX_D2D_RT->DrawRectangle(rect, DX_GreenBrush);
		}
		D2D1_TAG D2DTag1;
		D2D1_TAG D2DTag2;
		HRESULT HRes = DX_D2D_RT->EndDraw(&D2DTag1, &D2DTag2);
		(void)HRes;


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


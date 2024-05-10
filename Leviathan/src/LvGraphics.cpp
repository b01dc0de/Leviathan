#include "LvCommon.h"

#include "LvGraphics.h"
#include "Leviathan.h"
#include "LvMesh.h"
#include "LvCamera.h"

namespace Leviathan
{

	LvGraphics* LvGraphics::PvInst = nullptr;

	const float ClearColor[4] = { 0.0f, 0.125f, 0.3f, 1.0f };

	void ReportLiveObjects(ID3D11Device* pDXDevice)
	{
	#if LV_DEBUG_BUILD()
		DXHandle<ID3D11Debug> DX_Debug = nullptr;
		DXCHECK(pDXDevice->QueryInterface(IID_PPV_ARGS(&DX_Debug)));
		DX_Debug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL|D3D11_RLDO_IGNORE_INTERNAL);
		DX_Debug.SafeRelease();
	#endif // LV_DEBUG_BUILD()
	}

	LvGraphics::~LvGraphics()
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

		delete pCubeMesh;
		delete pTriangleMesh;
		delete pRectUVMesh;

		// Release resources in (generally) reverse init order
		DX_DefaultTexture.SafeRelease();
		DX_DefaultTexture_SRV.SafeRelease();
		DX_DefaultSamplerState.SafeRelease();
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

	#if LV_DEBUG_BUILD()
		Outf("ReportLiveObjects -- After SafeReleasing DXHandles (except ID3D11Device):\n");
		ReportLiveObjects(DX_Device);
	#endif // LV_DEBUG

		// CKA_NOTE: Disabled the exception detailed below in VS. Throws exceptions on following line:
		DX_Device.SafeRelease();
		//		Microsoft C++ exception thrown: MONZA::DdiThreadingContext<MONZA::AdapterTraits_Gen12LP>::msg_end at memory... 
		//		Microsoft C++ exception thrown: MONZA::IgcThreadingContext<MONZA::AdapterTraits_Gen12LP>::msg_end at memory...
		//		In module: igd10um64xe.dll
		//		^ Above DLL looks to be Intel Graphics related
		// Don't care enough to solve this now but should investigate in future
		DX_Device = nullptr;
	}

	int LvGraphics::CompileShader(LPCWSTR SourceFileName, LPCSTR EntryPointFunction, LPCSTR Profile, ID3DBlob** ShaderBlob, D3D_SHADER_MACRO* pOptDefines)
	{
		if (SourceFileName == nullptr || EntryPointFunction == nullptr || Profile == nullptr || ShaderBlob == nullptr)
		{
			return E_INVALIDARG;
		}

		*ShaderBlob = nullptr;

		UINT CompileFlags = D3DCOMPILE_ENABLE_STRICTNESS|D3DCOMPILE_IEEE_STRICTNESS|D3DCOMPILE_PACK_MATRIX_ROW_MAJOR;
	#if LV_DEBUG_BUILD()
		CompileFlags |= D3DCOMPILE_WARNINGS_ARE_ERRORS;
		CompileFlags |= D3DCOMPILE_DEBUG;
		CompileFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;
	#endif

		ID3DBlob* OutBlob = nullptr;
		ID3DBlob* ErrorMsgBlob = nullptr;

		HRESULT Result = D3DCompileFromFile
		(
			SourceFileName,
			pOptDefines,
			D3D_COMPILE_STANDARD_FILE_INCLUDE,
			EntryPointFunction,
			Profile,
			CompileFlags,
			0,
			&OutBlob,
			&ErrorMsgBlob
		);
		DXCHECK(Result);

		if (ErrorMsgBlob)
		{
			Outf("%s\n", (char*)ErrorMsgBlob->GetBufferPointer());
			ErrorMsgBlob->Release();
		}

		*ShaderBlob = OutBlob;

		return Result;
	}

	void LvGraphics::PvInitFallbackTexture()
	{
		struct Color32FRGBA
		{
			float R;
			float G;
			float B;
			float A;
		};
		const Color32FRGBA Pink{ 1.0f, 73.0f / 255.0f, 173.0f / 255.0f, 1.0f};
		const Color32FRGBA Black{ 0.0f, 0.0f, 0.0f, 1.0f};

		const Color32FRGBA Red{ 1.0f, 0.0f, 0.0f, 1.0f };
		const Color32FRGBA Green{ 0.0f, 1.0f, 0.0f, 1.0f };
		const Color32FRGBA Blue{ 0.0f, 0.0f, 1.0f, 1.0f };
		const Color32FRGBA White{ 1.0f, 1.0f, 1.0f, 1.0f };

		constexpr UINT Size = 512;
		constexpr UINT MipLevels = 9;
		UINT CellSize = Size / 16;
		int MaxCell = (Size - 1) / CellSize;
		Color32FRGBA* DefaultTextureData = new Color32FRGBA[Size*Size];
		for (UINT RowIdx = 0; RowIdx < Size; RowIdx++)
		{
			for (UINT ColIdx = 0; ColIdx < Size; ColIdx++)
			{
				int CellRow = RowIdx / CellSize;
				int CellCol = ColIdx / CellSize;

				if (CellRow == 0 && CellCol == 0)
				{
					DefaultTextureData[(RowIdx * Size) + ColIdx] = Red;
					continue;
				}
				else if (CellRow == 0 && CellCol == MaxCell)
				{
					DefaultTextureData[(RowIdx * Size) + ColIdx] = Green;
					continue;
				}
				else if (CellRow == MaxCell && CellCol == 0)
				{
					DefaultTextureData[(RowIdx * Size) + ColIdx] = Blue;
					continue;
				}
				else if (CellRow == MaxCell && CellCol == MaxCell)
				{
					DefaultTextureData[(RowIdx * Size) + ColIdx] = White;
					continue;
				}

				bool bEvenCell = ((CellRow + CellCol) % 2 == 0);
				DefaultTextureData[(RowIdx * Size) + ColIdx] = bEvenCell ? Pink : Black;
			}
		}

		D3D11_SUBRESOURCE_DATA TexDataDesc[MipLevels] = { {}, {}, {}, {}, {}, {}, {}, {}, {} };
		TexDataDesc[0].pSysMem = DefaultTextureData;
		TexDataDesc[0].SysMemPitch = sizeof(Color32FRGBA) * Size;
		TexDataDesc[0].SysMemSlicePitch = sizeof(Color32FRGBA) * Size * Size;
		DXGI_FORMAT DefaultTextureFormat = DXGI_FORMAT_R32G32B32A32_FLOAT;

		D3D11_TEXTURE2D_DESC DefaultTextureDesc = {};
		DefaultTextureDesc.Width = Size;
		DefaultTextureDesc.Height = Size;
		DefaultTextureDesc.MipLevels = 1;
		DefaultTextureDesc.ArraySize = 1;
		DefaultTextureDesc.Format = DefaultTextureFormat;
		DefaultTextureDesc.SampleDesc.Count = 1;
		DefaultTextureDesc.SampleDesc.Quality = 0;
		DefaultTextureDesc.Usage = D3D11_USAGE_DEFAULT;
		DefaultTextureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		DefaultTextureDesc.CPUAccessFlags = 0;
		DefaultTextureDesc.MiscFlags = 0;
		DXCHECK(DX_Device->CreateTexture2D(&DefaultTextureDesc, &TexDataDesc[0], &DX_DefaultTexture));

		/*
		D3D11_SHADER_RESOURCE_VIEW_DESC SRV_Desc = {};
		SRV_Desc.Format = DefaultTextureFormat;
		SRV_Desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		SRV_Desc.Texture2D.MostDetailedMip = 0;
		SRV_Desc.Texture2D.MipLevels = (UINT)1;
		*/

		DXCHECK(DX_Device->CreateShaderResourceView(DX_DefaultTexture, /*&SRV_Desc*/nullptr, &DX_DefaultTexture_SRV));

		D3D11_SAMPLER_DESC DefaultTextureSamplerDesc = {};
		DefaultTextureSamplerDesc.Filter = D3D11_FILTER_MIN_MAG_POINT_MIP_LINEAR; // D3D11_FILTER_MIN_MAG_MIP_LINEAR D3D11_FILTER_MIN_MAG_MIP_POINT;
		DefaultTextureSamplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		DefaultTextureSamplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		DefaultTextureSamplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		DefaultTextureSamplerDesc.MipLODBias = 0.0f;
		DefaultTextureSamplerDesc.MaxAnisotropy = 0; //4;
		DefaultTextureSamplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
		//DefaultTextureSamplerDesc.BorderColor = { 0.0f, 0.0f, 0.0f, 1.0f };
		DefaultTextureSamplerDesc.MinLOD = 0;
		DefaultTextureSamplerDesc.MaxLOD = 0;//D3D11_FLOAT32_MAX;
		DXCHECK(DX_Device->CreateSamplerState(&DefaultTextureSamplerDesc, &DX_DefaultSamplerState));

		delete[] DefaultTextureData;
	}

	void LvGraphics::PvInit()
	{
		D3D_FEATURE_LEVEL SupportedFeatureLevels[] =
		{
			//D3D_FEATURE_LEVEL_12_2, D3D_FEATURE_LEVEL_12_1, D3D_FEATURE_LEVEL_12_0,
			D3D_FEATURE_LEVEL_11_1, D3D_FEATURE_LEVEL_11_0,
		};
		UINT NumSupportedFeatureLevels = ARRAYSIZE(SupportedFeatureLevels);

		UINT CreateDeviceFlags = 0;
	#if LV_DEBUG_BUILD()
		CreateDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
	#endif

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
			SupportedFeatureLevels,
			NumSupportedFeatureLevels,
			D3D11_SDK_VERSION,
			&DX_Device,
			&UsedFeatureLevel,
			&DX_ImmediateContext
		));
		Assert(UsedFeatureLevel == D3D_FEATURE_LEVEL_11_1);

		DXGI_SWAP_CHAIN_DESC1 SwapChainDesc1 = {};
		SwapChainDesc1.Width = ResX;
		SwapChainDesc1.Height = ResY;
		SwapChainDesc1.Format = RenderTargetFormat;
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
			LvWindow,
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
		RTT_Desc.Format = RenderTargetFormat;
		RTT_Desc.SampleDesc = Shared_RT_SampleDesc;
		RTT_Desc.Usage = D3D11_USAGE_DEFAULT;
		RTT_Desc.BindFlags = D3D11_BIND_RENDER_TARGET;
		RTT_Desc.CPUAccessFlags = 0;
		RTT_Desc.MiscFlags = 0;
		DXCHECK(DX_Device->CreateTexture2D(&RTT_Desc, nullptr, &DX_RenderTargetTexture));
		D3D11_RENDER_TARGET_VIEW_DESC RTV_Desc = {};
		RTV_Desc.Format = RenderTargetFormat;
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
		DX_ImmediateContext->OMSetRenderTargets(1, &DX_RenderTargetView, DX_DepthStencilView);

		D3D11_RASTERIZER_DESC RasterDesc = {};
		RasterDesc.FillMode = D3D11_FILL_SOLID;
		RasterDesc.CullMode = D3D11_CULL_BACK;
		RasterDesc.FrontCounterClockwise = true;
		RasterDesc.DepthClipEnable = true;
		RasterDesc.ScissorEnable = false;
		RasterDesc.MultisampleEnable = true;
		RasterDesc.AntialiasedLineEnable = true;

		DXCHECK(DX_Device->CreateRasterizerState(&RasterDesc, &DX_RasterizerState));

		DX_ImmediateContext->RSSetState(DX_RasterizerState);

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
		DX_ImmediateContext->RSSetViewports(1, &Viewport_Desc);

		D3D_SHADER_MACRO ShaderColorPoundDefines[] =
		{
			"ENABLE_WVP_TRANSFORM", "1",
			"COMBINED_WVP_BUFFER", "0",
			"ENABLE_VERTEX_TEXTURE", "0",
			"ENABLE_VERTEX_COLOR", "1",
			NULL, NULL
		};
		D3D_SHADER_MACRO ShaderTexturePoundDefines[] =
		{
			"ENABLE_WVP_TRANSFORM", "1",
			"COMBINED_WVP_BUFFER", "0",
			"ENABLE_VERTEX_TEXTURE", "1",
			"ENABLE_VERTEX_COLOR", "0",
			NULL, NULL
		};

		pCubeMesh = InitCube();
		pTriangleMesh = InitTriangle();
		pRectUVMesh = InitTextureRect();

		ID3DBlob* VSColorCodeBlob = nullptr;
		ID3DBlob* PSColorCodeBlob = nullptr;
		DXCHECK(CompileShader(L"src/HLSL/BasicShader.hlsl", "VSMain", "vs_5_0", &VSColorCodeBlob, &ShaderColorPoundDefines[0]));
		DXCHECK(CompileShader(L"src/HLSL/BasicShader.hlsl", "PSMain", "ps_5_0", &PSColorCodeBlob, &ShaderColorPoundDefines[0]));
		if (VSColorCodeBlob && PSColorCodeBlob)
		{
			DXCHECK(DX_Device->CreateVertexShader(VSColorCodeBlob->GetBufferPointer(), VSColorCodeBlob->GetBufferSize(), nullptr, &DX_VSColor));
			DXCHECK(DX_Device->CreatePixelShader(PSColorCodeBlob->GetBufferPointer(), PSColorCodeBlob->GetBufferSize(), nullptr, &DX_PSColor));

			D3D11_INPUT_ELEMENT_DESC InputLayoutDesc[] =
			{
				{"POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				// { "TEXTURE", ... }
				{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			};
			UINT NumInputElements = ARRAYSIZE(InputLayoutDesc);
			DXCHECK(DX_Device->CreateInputLayout(InputLayoutDesc, NumInputElements, VSColorCodeBlob->GetBufferPointer(), VSColorCodeBlob->GetBufferSize(), &DX_InputLayoutColor));


			D3D11_BUFFER_DESC VertexBufferDesc =
			{
				sizeof(VertexColor) * pCubeMesh->NumVerts,
				D3D11_USAGE_DEFAULT,
				D3D11_BIND_VERTEX_BUFFER,
				0,
				0
			};
			D3D11_SUBRESOURCE_DATA VertexBufferInitData = { pCubeMesh->Verts, 0, 0 };
			DXCHECK(DX_Device->CreateBuffer(&VertexBufferDesc, &VertexBufferInitData, &DX_CubeVertexBuffer));

			D3D11_BUFFER_DESC IndexBufferDesc =
			{
				sizeof(TriPrim) * pCubeMesh->NumPrims,
				D3D11_USAGE_DEFAULT,
				D3D11_BIND_INDEX_BUFFER,
				0,
				0
			};
			D3D11_SUBRESOURCE_DATA IndexBufferInitData = { pCubeMesh->Prims, 0, 0 };
			DXCHECK(DX_Device->CreateBuffer(&IndexBufferDesc, &IndexBufferInitData, &DX_CubeIndexBuffer));

			D3D11_BUFFER_DESC WorldBufferDesc = {};
			WorldBufferDesc.ByteWidth = sizeof(fMatrix);
			WorldBufferDesc.Usage = D3D11_USAGE_DEFAULT;
			WorldBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			WorldBufferDesc.CPUAccessFlags = 0;
			DXCHECK(DX_Device->CreateBuffer(&WorldBufferDesc, nullptr, &DX_WorldBuffer));

			D3D11_BUFFER_DESC ViewProjBufferDesc = {};
			ViewProjBufferDesc.ByteWidth = sizeof(fMatrix) * 2;
			ViewProjBufferDesc.Usage = D3D11_USAGE_DEFAULT;
			ViewProjBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			ViewProjBufferDesc.CPUAccessFlags = 0;
			DXCHECK(DX_Device->CreateBuffer(&ViewProjBufferDesc, nullptr, &DX_ViewProjBuffer));
		}
		if (VSColorCodeBlob) { VSColorCodeBlob->Release(); }
		if (PSColorCodeBlob) { PSColorCodeBlob->Release(); }

		ID3DBlob* VSTextureCodeBlob = nullptr;
		ID3DBlob* PSTextureCodeBlob = nullptr;
		DXCHECK(CompileShader(L"src/HLSL/BasicShader.hlsl", "VSMain", "vs_5_0", &VSTextureCodeBlob, &ShaderTexturePoundDefines[0]));
		DXCHECK(CompileShader(L"src/HLSL/BasicShader.hlsl", "PSMain", "ps_5_0", &PSTextureCodeBlob, &ShaderTexturePoundDefines[0]));
		if (VSTextureCodeBlob && PSTextureCodeBlob)
		{
			DXCHECK(DX_Device->CreateVertexShader(VSTextureCodeBlob->GetBufferPointer(), VSTextureCodeBlob->GetBufferSize(), nullptr, &DX_VSTexture));
			DXCHECK(DX_Device->CreatePixelShader(PSTextureCodeBlob->GetBufferPointer(), PSTextureCodeBlob->GetBufferSize(), nullptr, &DX_PSTexture));

			D3D11_INPUT_ELEMENT_DESC InputLayoutDesc[] =
			{
				{"POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				//{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			};
			UINT NumInputElements = ARRAYSIZE(InputLayoutDesc);
			DXCHECK(DX_Device->CreateInputLayout(InputLayoutDesc, NumInputElements, VSTextureCodeBlob->GetBufferPointer(), VSTextureCodeBlob->GetBufferSize(), &DX_InputLayoutTexture));

			D3D11_BUFFER_DESC VertexBufferDesc =
			{
				sizeof(VertexUV) * pRectUVMesh->NumVerts,
				D3D11_USAGE_DEFAULT,
				D3D11_BIND_VERTEX_BUFFER,
				0,
				0
			};
			D3D11_SUBRESOURCE_DATA VertexBufferInitData = { pRectUVMesh->Verts, 0, 0 };
			DXCHECK(DX_Device->CreateBuffer(&VertexBufferDesc, &VertexBufferInitData, &DX_RectVertexBuffer));

			D3D11_BUFFER_DESC IndexBufferDesc =
			{
				sizeof(TriPrim) * pRectUVMesh->NumPrims,
				D3D11_USAGE_DEFAULT,
				D3D11_BIND_INDEX_BUFFER,
				0,
				0
			};
			D3D11_SUBRESOURCE_DATA IndexBufferInitData = { pRectUVMesh->Prims, 0, 0 };
			DXCHECK(DX_Device->CreateBuffer(&IndexBufferDesc, &IndexBufferInitData, &DX_RectIndexBuffer));
		}
		if (VSTextureCodeBlob) { VSTextureCodeBlob->Release(); }
		if (PSTextureCodeBlob) { PSTextureCodeBlob->Release(); }

		PvInitFallbackTexture();

	#if LV_DEBUG_BUILD()
		PvSetDXDBGNames();
	#endif // LV_DEBUG
	}

#define DXDBG_SETDBGNAMEHELPER(DX_Handle) \
	static const char* DBGNAME##DX_Handle = #DX_Handle; \
	DX_Handle->SetPrivateData(WKPDID_D3DDebugObjectName, sizeof(#DX_Handle) - 1, DBGNAME##DX_Handle)

	void LvGraphics::PvSetDXDBGNames()
	{
	#if LV_DEBUG_BUILD()
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
		DXDBG_SETDBGNAMEHELPER(DX_DefaultTexture);
		DXDBG_SETDBGNAMEHELPER(DX_DefaultTexture_SRV);
		DXDBG_SETDBGNAMEHELPER(DX_DefaultSamplerState);
	#endif // LV_DEBUG
	}


	const fVector WorldPos(10.0f, 10.0f, -10.0f);
	const fVector LookAt(0.0f, 0.0f, 0.0f);
	const fVector AbsUp(0.0f, 1.0f, 0.0f, 0.0f);
	LvCameraPerspective Cam3D(WorldPos, LookAt, AbsUp);
	LvCameraOrthographic Cam2D;

	constexpr float FrameStep = 0.0166666666666667f;
	constexpr float fPI = 3.14159265359f;

	void LvGraphics::PvUpdateAndDraw()
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
			// When FakeTime == SecondsForFullRotation, then CamT == 2 * PI
			fVector CurrCamPos = {cosf(CamT) * CameraPathRadius, CamY, sinf(CamT) * CameraPathRadius * 2.0f};
			Cam3D.Orient(CurrCamPos, LookAt, AbsUp);
		}

		// CKA_NOTE:
		//	- This call is now necessary every time after calling SwapChain::Present
		//	- Necessary when using DXGI flip model
		DX_ImmediateContext->OMSetRenderTargets(1, &DX_RenderTargetView, DX_DepthStencilView);
		DX_ImmediateContext->OMSetDepthStencilState(DX_DepthStencilState, 0);

		DX_ImmediateContext->ClearRenderTargetView(DX_RenderTargetView, ClearColor);
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

			DX_ImmediateContext->DrawIndexed(pCubeMesh->NumPrims * 3, 0u, 0u);
		}

		{
			float ImgSize = 512.0f;
			float Scale = 0.25f;
			fVector CenterPos{ -512.0f, 256.0f, 0.0f };
			constexpr float ImgPathRadius = 64.0f;

			float Cam2DT = CurrRad / 5.0f;

			fVector ImgPos{CenterPos.X + (cosf(Cam2DT) * ImgPathRadius), CenterPos.Y + (sinf(Cam2DT) * ImgPathRadius), 0.0f};
			float CurrScale = Scale + ((Scale/2.0f) * cosf(CurrRad / 6.0f));
			float CurrAngle = -CurrRad / 3.0f;
			fMatrix TransMat(TRANS, ImgPos);
			fMatrix RotMat(ROT_Z, CurrAngle);
			fMatrix ScaleMat(SCALE, CurrScale * ImgSize, CurrScale * ImgSize, 1.0f);
			fMatrix World = Mult(ScaleMat, RotMat, TransMat);

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

			DX_ImmediateContext->PSSetShaderResources(0, 1, &DX_DefaultTexture_SRV);
			DX_ImmediateContext->PSSetSamplers(0, 1, &DX_DefaultSamplerState);

			DX_ImmediateContext->UpdateSubresource(DX_WorldBuffer, 0, nullptr, &World, sizeof(World), 0);
			DX_ImmediateContext->UpdateSubresource(DX_ViewProjBuffer, 0, nullptr, &Cam2D.CamTrans.View, sizeof(Cam2D.CamTrans), 0);

			DX_ImmediateContext->DrawIndexed(pRectUVMesh->NumPrims * 3, 0u, 0u);
		}


		DX_ImmediateContext->ResolveSubresource(DX_BackBuffer, 0, DX_RenderTargetTexture, 0, RenderTargetFormat);
		const DXGI_PRESENT_PARAMETERS PresentParams = {};
		DX_SwapChain1->Present1(0, DXGI_PRESENT_ALLOW_TEARING, &PresentParams);
	}

	void LvGraphics::Init()
	{
		Outf("LvGraphics::Init -- BEGIN\n");
		Inst()->PvInit();
		Outf("LvGraphics::Init -- END\n");
	}
	void LvGraphics::Term()
	{
		Outf("LvGraphics::Term -- BEGIN\n");
		Assert(PvInst);
		delete PvInst;
		PvInst = nullptr;
		Outf("LvGraphics::Term -- END\n");
	}
	void LvGraphics::UpdateAndDraw()
	{
		Inst()->PvUpdateAndDraw();
	}
}


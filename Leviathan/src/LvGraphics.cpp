#include "LvCommon.h"

#include "LvGraphics.h"
#include "Leviathan.h"

namespace Leviathan
{
	LvGraphics* LvGraphics::PvInst = nullptr;

	const VertexColor TriangleVertices[] =
	{
		{{0.0f, 0.5f, 0.5f, 1.0f}, {1.0f, 0.0f, 0.0f, 1.0f}},
		{{0.5f, -0.5f, 0.5f, 1.0f}, {0.0f, 1.0f, 0.0f, 1.0f}},
		{{-0.5f, -0.5f, 0.5f, 1.0f}, {0.0f, 0.0f, 1.0f, 1.0f}}
	};
	const UINT TriangleIndices[] = { 0, 2, 1 };
	const VertexColor SquareVertices[] =
	{
		{{-0.5f, 0.5f, 0.5f, 1.0f}, {1.0f, 1.0f, 1.0f, 1.0f}},
		{{0.5f, 0.5f, 0.5f, 1.0f}, {0.0f, 0.0f, 1.0f, 1.0f}},
		{{0.5f, -0.5f, 0.5f, 1.0f}, {0.0f, 1.0f, 0.0f, 1.0f}},
		{{-0.5f, -0.5f, 0.5f, 1.0f}, {1.0f, 0.0f, 0.0f, 1.0f}}
	};
	const UINT SquareIndices[] = { 0, 2, 1, 0, 3, 2 };

	const float ClearColor[4] = { 0.0f, 0.125f, 0.3f, 1.0f };

	LvGraphics::~LvGraphics()
	{
		DX_VertexBuffer.DxSafeRelease();
		DX_IndexBuffer.DxSafeRelease();
		DX_WorldBuffer.DxSafeRelease();
		DX_ViewProjBuffer.DxSafeRelease();

		DX_VertexShader.DxSafeRelease();
		DX_PixelShader.DxSafeRelease();
		DX_InputLayout.DxSafeRelease();

		DX_RasterizerState.DxSafeRelease();
		DX_DepthStencil.DxSafeRelease();
		DX_DepthStencilView.DxSafeRelease();
		DX_BlendState.DxSafeRelease();

		DX_BackBuffer.DxSafeRelease();
		DX_RenderTargetView.DxSafeRelease();
		DX_SwapChain.DxSafeRelease();
		DX_ImmediateContext.DxSafeRelease();

	#if LV_DEBUG_BUILD()
		DXHandle<ID3D11Debug> DX_Debug= nullptr;
		DX_Device->QueryInterface(IID_PPV_ARGS(&DX_Debug));
		DX_Debug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL|D3D11_RLDO_IGNORE_INTERNAL);
		DX_Debug.DxSafeRelease();
	#endif // LV_DEBUG_BUILD()

		DX_Device.DxSafeRelease();
	}

	int LvGraphics::CompileShader(LPCWSTR SourceFileName, LPCSTR EntryPointFunction, LPCSTR Profile, ID3DBlob** ShaderBlob)
	{
		if (SourceFileName == nullptr || EntryPointFunction == nullptr || Profile == nullptr || ShaderBlob == nullptr)
		{
			return E_INVALIDARG;
		}

		*ShaderBlob = nullptr;

		UINT CompileFlags = D3DCOMPILE_ENABLE_STRICTNESS;
	#if LV_DEBUG_BUILD()
		CompileFlags |= D3DCOMPILE_DEBUG;
		CompileFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;
	#endif

		D3D_SHADER_MACRO PoundDefines[] =
		{
			"ENABLE_WVP_TRANSFORM", "0",
			"COMBINED_WVP_BUFFER", "0",
			"ENABLE_VERTEX_TEXTURE", "0",
			"ENABLE_VERTEX_COLOR", "1",
			NULL, NULL
		};

		ID3DBlob* OutBlob = nullptr;
		ID3DBlob* ErrorMsgBlob = nullptr;

		HRESULT Result = D3DCompileFromFile
		(
			SourceFileName,
			PoundDefines,
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
			OutputDebugStringA((char*)ErrorMsgBlob->GetBufferPointer());
			ErrorMsgBlob->Release();
		}

		*ShaderBlob = OutBlob;

		return Result;
	};

	void LvGraphics::PvInit()
	{
		D3D_FEATURE_LEVEL SupportedFeatureLevels[] =
		{
			//D3D_FEATURE_LEVEL_12_2, D3D_FEATURE_LEVEL_12_1, D3D_FEATURE_LEVEL_12_0,
			D3D_FEATURE_LEVEL_11_1, D3D_FEATURE_LEVEL_11_0,
		};
		UINT NumSupportedFeatureLevels = ARRAYSIZE(SupportedFeatureLevels);
		D3D_FEATURE_LEVEL D3DFeatureLevel = D3D_FEATURE_LEVEL_11_0;

		DXGI_SWAP_CHAIN_DESC SwapChainDesc = {};
		SwapChainDesc.BufferDesc.Width = ResX;
		SwapChainDesc.BufferDesc.Height = ResY;
		SwapChainDesc.BufferDesc.Format = RenderTargetFormat;
		SwapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
		SwapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
		SwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		SwapChainDesc.SampleDesc.Count = 1;
		SwapChainDesc.SampleDesc.Quality = 0;
		SwapChainDesc.BufferCount = 2;
		SwapChainDesc.OutputWindow = LvWindow;
		SwapChainDesc.Windowed = true;
		SwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
		//SwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
		SwapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING;

		UINT CreateDeviceFlags = 0;
	#if LV_DEBUG_BUILD()
		CreateDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
	#endif

		DXCHECK(D3D11CreateDeviceAndSwapChain(
			nullptr, //IDXGIAdapter* pAdapter
			D3D_DRIVER_TYPE_HARDWARE,
			nullptr,
			CreateDeviceFlags,
			SupportedFeatureLevels,
			NumSupportedFeatureLevels,
			D3D11_SDK_VERSION,
			&SwapChainDesc,
			&DX_SwapChain,
			&DX_Device,
			&UsedFeatureLevel,
			&DX_ImmediateContext
		));

		DXCHECK(DX_SwapChain->GetBuffer(0, IID_PPV_ARGS(&DX_BackBuffer)));

		DXGI_SAMPLE_DESC Shared_RT_SampleDesc = {};
		Shared_RT_SampleDesc.Count = 4;
		Shared_RT_SampleDesc.Quality = D3D11_STANDARD_MULTISAMPLE_PATTERN;

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
		DX_Device->CreateTexture2D(&RTT_Desc, nullptr, &DX_RenderTargetTexture);
		D3D11_RENDER_TARGET_VIEW_DESC RTV_Desc = {};
		RTV_Desc.Format = RenderTargetFormat;
		RTV_Desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DMS;
		RTV_Desc.Texture2D.MipSlice = 0;
		//DXCHECK(DX_Device->CreateRenderTargetView(DX_BackBuffer, nullptr, &DX_RenderTargetView));
		//DXCHECK(DX_Device->CreateRenderTargetView(DX_RenderTargetTexture, &RTV_Desc, &DX_RenderTargetView));
		DXCHECK(DX_Device->CreateRenderTargetView(DX_RenderTargetTexture, nullptr, &DX_RenderTargetView));

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

		D3D11_TEXTURE2D_DESC DepthDesc = {};
		DepthDesc.Width = ResX;
		DepthDesc.Height = ResY;
		DepthDesc.MipLevels = 1;
		DepthDesc.ArraySize = 1;
		DepthDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		DepthDesc.SampleDesc = Shared_RT_SampleDesc;
		DepthDesc.Usage = D3D11_USAGE_DEFAULT;
		DepthDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		DepthDesc.CPUAccessFlags = 0;
		DepthDesc.MiscFlags = 0;

		DXCHECK(DX_Device->CreateTexture2D(&DepthDesc, nullptr, &DX_DepthStencil));

		D3D11_DEPTH_STENCIL_VIEW_DESC DepthStencilViewDesc = {};
		DepthStencilViewDesc.Format = DepthStencilViewDesc.Format;
		DepthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
		DepthStencilViewDesc.Texture2D.MipSlice = 0;

		DXCHECK(DX_Device->CreateDepthStencilView(DX_DepthStencil, &DepthStencilViewDesc, &DX_DepthStencilView));

		DX_ImmediateContext->OMSetRenderTargets(1, &DX_RenderTargetView, DX_DepthStencilView);

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

		D3D11_BUFFER_DESC VertexBufferDesc =
		{
			//sizeof(TriangleVertices),
			sizeof(SquareVertices),
			D3D11_USAGE_DEFAULT,
			D3D11_BIND_VERTEX_BUFFER,
			0,
			0
		};
		D3D11_SUBRESOURCE_DATA VertexBufferInitData = { /*TriangleVertices*/ SquareVertices, 0, 0};
		DXCHECK(DX_Device->CreateBuffer(&VertexBufferDesc, &VertexBufferInitData, &DX_VertexBuffer));

		D3D11_BUFFER_DESC IndexBufferDesc =
		{
			//sizeof(TriangleIndices),
			sizeof(SquareIndices),
			D3D11_USAGE_DEFAULT,
			D3D11_BIND_INDEX_BUFFER,
			0,
			0
		};
		D3D11_SUBRESOURCE_DATA IndexBufferInitData = { /*TriangleIndices*/ SquareIndices, 0, 0};
		DXCHECK(DX_Device->CreateBuffer(&IndexBufferDesc, &IndexBufferInitData, &DX_IndexBuffer));

		ID3DBlob* VSCodeBlob = nullptr;
		ID3DBlob* PSCodeBlob = nullptr;
		DXCHECK(CompileShader(L"src/BasicShader.hlsl", "VSMain", "vs_5_0", &VSCodeBlob));
		DXCHECK(CompileShader(L"src/BasicShader.hlsl", "PSMain", "ps_5_0", &PSCodeBlob));

		if (VSCodeBlob && PSCodeBlob)
		{
			DXCHECK(DX_Device->CreateVertexShader(VSCodeBlob->GetBufferPointer(), VSCodeBlob->GetBufferSize(), nullptr, &DX_VertexShader));
			DXCHECK(DX_Device->CreatePixelShader(PSCodeBlob->GetBufferPointer(), PSCodeBlob->GetBufferSize(), nullptr, &DX_PixelShader));

			D3D11_INPUT_ELEMENT_DESC InputLayoutDesc[] =
			{
				{"POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				// { "TEXTURE", ... }
				{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			};
			UINT NumInputElements = ARRAYSIZE(InputLayoutDesc);

			DXCHECK(DX_Device->CreateInputLayout(InputLayoutDesc, NumInputElements, VSCodeBlob->GetBufferPointer(), VSCodeBlob->GetBufferSize(), &DX_InputLayout));

			D3D11_BUFFER_DESC BufferDesc = {};
			BufferDesc.ByteWidth = sizeof(fMatrix) * 2;
			BufferDesc.Usage = D3D11_USAGE_DEFAULT;
			BufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			BufferDesc.CPUAccessFlags = 0;

			DXCHECK(DX_Device->CreateBuffer(&BufferDesc, nullptr, &DX_ViewProjBuffer));

			BufferDesc.ByteWidth = sizeof(fMatrix);
			DXCHECK(DX_Device->CreateBuffer(&BufferDesc, nullptr, &DX_WorldBuffer));
		}
		if (VSCodeBlob) { VSCodeBlob->Release(); }
		if (PSCodeBlob) { PSCodeBlob->Release(); }
	}

	void LvGraphics::PvUpdateState()
	{
		UINT Stride = sizeof(VertexColor);
		UINT Offset = 0;
		float fDepth = 1.0f;

		WorldViewProjData CurrWVP;
		CurrWVP.View.Identity();
		CurrWVP.Proj.Identity();
		CurrWVP.World.Identity();

		// CKA_NOTE:
		//	- This call is now necessary every time after calling SwapChain::Present
		//	- Necessary when using DXGI flip model
		DX_ImmediateContext->OMSetRenderTargets(1, &DX_RenderTargetView, DX_DepthStencilView);

		DX_ImmediateContext->IASetInputLayout(DX_InputLayout);
		DX_ImmediateContext->IASetVertexBuffers(0, 1, &DX_VertexBuffer, &Stride, &Offset);
		DX_ImmediateContext->IASetIndexBuffer(DX_IndexBuffer, DXGI_FORMAT_R32_UINT, 0);
		DX_ImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		DX_ImmediateContext->VSSetConstantBuffers(0, 1, &DX_ViewProjBuffer);
		DX_ImmediateContext->VSSetConstantBuffers(1, 1, &DX_WorldBuffer);

		DX_ImmediateContext->UpdateSubresource(DX_ViewProjBuffer, 0, nullptr, &CurrWVP.View, 0, 0);
		DX_ImmediateContext->UpdateSubresource(DX_WorldBuffer, 0, nullptr, &CurrWVP.World, 0, 0);

		DX_ImmediateContext->VSSetShader(DX_VertexShader, nullptr, 0);
		DX_ImmediateContext->PSSetShader(DX_PixelShader, nullptr, 0);
	}

	void LvGraphics::PvDraw()
	{
		PvUpdateState();

		DX_ImmediateContext->ClearRenderTargetView(DX_RenderTargetView, ClearColor);
		DX_ImmediateContext->ClearDepthStencilView(DX_DepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

		DX_ImmediateContext->DrawIndexed(ARRAYSIZE(/*TriangleIndices*/SquareIndices), 0u, 0u);

		// CKA_TODO: Perform an MSAA resolve here
		DX_ImmediateContext->ResolveSubresource(DX_BackBuffer, 0, DX_RenderTargetTexture, 0, RenderTargetFormat);

		DX_SwapChain->Present(0, DXGI_PRESENT_ALLOW_TEARING);
	}

}


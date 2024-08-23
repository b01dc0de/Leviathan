#include "AtollGraphics.h"

namespace ShaderAtoll
{
	IDXGISwapChain* AtollGraphics::DX_SwapChain = nullptr;
	ID3D11Device* AtollGraphics::DX_Device = nullptr;
	D3D_FEATURE_LEVEL AtollGraphics::UsedFeatureLevel = {};
	ID3D11DeviceContext* AtollGraphics::DX_ImmediateContext = nullptr;

	ID3D11Texture2D* AtollGraphics::DX_BackBuffer = nullptr;
	ID3D11RenderTargetView* AtollGraphics::DX_RenderTargetView = nullptr;

	IDXGIFactory1* AtollGraphics::DX_Factory = nullptr;
	std::vector<IDXGIAdapter*> AtollGraphics::DX_AdapterList = {};
	std::vector<IDXGIOutput*> AtollGraphics::DX_Outputs = {};
	std::vector<DXGI_MODE_DESC*> AtollGraphics::OutputModeDescList = {};

	ID3D11RasterizerState* AtollGraphics::DX_RasterizerState = nullptr;
	ID3D11Texture2D* AtollGraphics::DX_DepthStencil = nullptr;
	ID3D11DepthStencilView* AtollGraphics::DX_DepthStencilView = nullptr;
	ID3D11BlendState* AtollGraphics::DX_BlendState = nullptr;

	ID3D11Buffer* AtollGraphics::DX_VertexBuffer = nullptr;
	ID3D11Buffer* AtollGraphics::DX_IndexBuffer = nullptr;

	//ID3D11Buffer* AtollGraphics::DX_GlobalsBuffer = nullptr;

	ID3D11VertexShader* AtollGraphics::DX_VertexShader = nullptr;
	ID3D11PixelShader* AtollGraphics::DX_PixelShader = nullptr;
	ID3D11InputLayout* AtollGraphics::DX_InputLayout = nullptr;

	VF4 Color_Red = { 1.0f, 0.0f, 0.0f, 1.0f };
	VF4 Color_Blue = { 0.0f, 1.0f, 0.0f, 1.0f };
	VF4 Color_Green = { 0.0f, 0.0f, 1.0f, 1.0f };
	VF4 Color_White = { 1.0f, 1.0f, 1.0f, 1.0f };
	VF4 BoxP0 = { -1.0f, 1.0f, 0.5f, 1.0f };
	VF4 BoxP1 = { 1.0f, 1.0f, 0.5f, 1.0f };
	VF4 BoxP2 = { -1.0f, -1.0f, 0.5f, 1.0f };
	VF4 BoxP3 = { 1.0f, -1.0f, 0.5f, 1.0f };

	VertexColor Vertices[] =
	{
		{BoxP0, Color_Red},
		{BoxP1, Color_Blue},
		{BoxP2, Color_Green},
		{BoxP3, Color_White},
	};

	TriIx BoxIxs[] =
	{
		{0, 2, 1},
		{1, 2, 3}
	};

	int AtollGraphics::CompileShaderHelper(LPCWSTR SourceFileName, LPCSTR EntryPointFunction, LPCSTR Profile, ID3DBlob** ShaderBlob)
	{
		HRESULT Result = S_OK;

		if (SourceFileName == nullptr || EntryPointFunction == nullptr || Profile == nullptr || ShaderBlob == nullptr)
		{
			return E_INVALIDARG;
		}

		*ShaderBlob = nullptr;

		UINT CompileFlags = D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_WARNINGS_ARE_ERRORS;
#if BUILD_DEBUG()
		CompileFlags |= D3DCOMPILE_ALL_RESOURCES_BOUND;
		CompileFlags |= D3DCOMPILE_DEBUG;
		CompileFlags |= D3DCOMPILE_DEBUG_NAME_FOR_SOURCE;
		CompileFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;
#endif // BUILD_DEBUG()

		ID3DBlob* OutBlob = nullptr;
		ID3DBlob* ErrorMsgBlob = nullptr;

		static constexpr D3D_SHADER_MACRO ShaderHLSL_Defines[] =
		{
			"ENABLE_VERTEX_COLOR", "0",
			NULL, NULL
		};

		Result = D3DCompileFromFile
		(
			SourceFileName,
			ShaderHLSL_Defines,
			D3D_COMPILE_STANDARD_FILE_INCLUDE,
			EntryPointFunction,
			Profile,
			CompileFlags,
			0, //UINT Flags2
			&OutBlob,
			&ErrorMsgBlob
		);

		if (FAILED(Result) && OutBlob)
		{
			OutBlob->Release();
			OutBlob = nullptr;
		}
		if (ErrorMsgBlob)
		{
			OutputDebugStringA((char*)ErrorMsgBlob->GetBufferPointer());
			ErrorMsgBlob->Release();
		}

		*ShaderBlob = OutBlob;

		return Result;
	};

	int AtollGraphics::InitGraphics()
	{
		HRESULT Result = S_OK;

		D3D_FEATURE_LEVEL SupportedFeatureLevels[] =
		{
			D3D_FEATURE_LEVEL_11_1,
			D3D_FEATURE_LEVEL_11_0,
		};
		UINT NumSupportedFeatureLevels = ARRAY_SIZE(SupportedFeatureLevels);
		D3D_FEATURE_LEVEL D3DFeatureLevel = D3D_FEATURE_LEVEL_11_0;

		CreateDXGIFactory1(__uuidof(IDXGIFactory), (void**)&DX_Factory);
		if (nullptr != DX_Factory)
		{
			IDXGIAdapter* DX_Adapter = nullptr;
			for (UINT AdapterIdx = 0; DX_Factory->EnumAdapters(AdapterIdx, &DX_Adapter); AdapterIdx++)
			{
				DX_AdapterList.push_back(DX_Adapter);
			}
		}

		for (int AdapterIdx = 0; AdapterIdx < DX_AdapterList.size(); AdapterIdx++)
		{
			IDXGIOutput* DX_Output = nullptr;
			for (int OutputIdx = 0; DX_AdapterList[AdapterIdx]->EnumOutputs(OutputIdx, &DX_Output) != DXGI_ERROR_NOT_FOUND; OutputIdx++)
			{
				DXGI_FORMAT Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
				UINT NumModes = 0;
				Result = DX_Output->GetDisplayModeList(Format, 0, &NumModes, nullptr);
				if (NumModes != 0)
				{
					DXGI_MODE_DESC* ModeList = new DXGI_MODE_DESC[NumModes];
					Result = DX_Output->GetDisplayModeList(Format, 0, &NumModes, ModeList);
					OutputModeDescList.push_back(ModeList);
				}
			}
		}

		DXGI_SAMPLE_DESC SharedSampleDesc = {};
		SharedSampleDesc.Count = 4;
		SharedSampleDesc.Quality = (UINT)D3D11_STANDARD_MULTISAMPLE_PATTERN;

		DXGI_SWAP_CHAIN_DESC swapchain_desc = {};
		swapchain_desc.BufferCount = 2;
		swapchain_desc.BufferDesc.Width = ShaderAtoll::WinResX;
		swapchain_desc.BufferDesc.Height = ShaderAtoll::WinResY;
		swapchain_desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		swapchain_desc.BufferDesc.RefreshRate.Numerator = 60;
		swapchain_desc.BufferDesc.RefreshRate.Denominator = 1;
		swapchain_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapchain_desc.OutputWindow = ShaderAtoll::hWindow;
		swapchain_desc.SampleDesc = SharedSampleDesc;
		swapchain_desc.Windowed = true;

		UINT CreateDeviceFlags = 0;
#if BUILD_DEBUG()
		CreateDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif // BUILD_DEBUG()

		Result = D3D11CreateDeviceAndSwapChain(
			nullptr,					//IDXGIAdapter* pAdapter
			D3D_DRIVER_TYPE_HARDWARE,	//D3D_DRIVER_TYPE DriverType
			nullptr,					//HMODULE Software
			CreateDeviceFlags,			//UINT Flags
			SupportedFeatureLevels,		//const D3D_FEATURE_LEVEL* pFeatureLevels
			NumSupportedFeatureLevels,	//UINT FeatureLevels
			D3D11_SDK_VERSION,			//UINT SDKVersion
			&swapchain_desc,			//const DXGI_SWAP_CHAIN_DESC* pSwapChainDesc
			&DX_SwapChain,
			&DX_Device,
			&UsedFeatureLevel,
			&DX_ImmediateContext
		);
		DXCHECK(Result);

		Result = DX_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&DX_BackBuffer);
		DXCHECK(Result);

		Result = DX_Device->CreateRenderTargetView(DX_BackBuffer, nullptr, &DX_RenderTargetView);
		DXCHECK(Result);

		D3D11_RASTERIZER_DESC RasterDesc = {};
		RasterDesc.FillMode = D3D11_FILL_SOLID;
		RasterDesc.CullMode = D3D11_CULL_BACK;
		RasterDesc.FrontCounterClockwise = true;
		RasterDesc.DepthClipEnable = true;
		RasterDesc.DepthClipEnable = true;
		RasterDesc.ScissorEnable = false;
		RasterDesc.MultisampleEnable = true;
		RasterDesc.AntialiasedLineEnable = true;

		Result = DX_Device->CreateRasterizerState(&RasterDesc, &DX_RasterizerState);
		DXCHECK(Result);

		DX_ImmediateContext->RSSetState(DX_RasterizerState);

		D3D11_TEXTURE2D_DESC DepthDesc = {};
		DepthDesc.Width = ShaderAtoll::WinResX;
		DepthDesc.Height = ShaderAtoll::WinResY;
		DepthDesc.MipLevels = 1;
		DepthDesc.ArraySize = 1;
		DepthDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		DepthDesc.SampleDesc = SharedSampleDesc;
		DepthDesc.Usage = D3D11_USAGE_DEFAULT;
		DepthDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		DepthDesc.CPUAccessFlags = 0;
		DepthDesc.MiscFlags = 0;

		Result = DX_Device->CreateTexture2D(&DepthDesc, nullptr, &DX_DepthStencil);
		DXCHECK(Result);

		D3D11_DEPTH_STENCIL_VIEW_DESC DepthStencilViewDesc = {};
		DepthStencilViewDesc.Format = DepthStencilViewDesc.Format;
		DepthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
		DepthStencilViewDesc.Texture2D.MipSlice = 0;

		Result = DX_Device->CreateDepthStencilView(DX_DepthStencil, &DepthStencilViewDesc, &DX_DepthStencilView);
		DXCHECK(Result);

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

		Result = DX_Device->CreateBlendState(&BlendDesc, &DX_BlendState);
		DXCHECK(Result);

		D3D11_VIEWPORT Viewport_Desc = {};
		Viewport_Desc.Width = (FLOAT)ShaderAtoll::WinResX;
		Viewport_Desc.Height = (FLOAT)ShaderAtoll::WinResY;
		Viewport_Desc.MinDepth = 0.0f;
		Viewport_Desc.MaxDepth = 1.0f;
		Viewport_Desc.TopLeftX = 0;
		Viewport_Desc.TopLeftY = 0;
		DX_ImmediateContext->RSSetViewports(1, &Viewport_Desc);

		D3D11_BUFFER_DESC VertexBufferDesc =
		{
			sizeof(VertexColor) * ARRAY_SIZE(Vertices),
			D3D11_USAGE_DEFAULT,
			D3D11_BIND_VERTEX_BUFFER,
			0,
			0
		};
		D3D11_SUBRESOURCE_DATA VertexBufferInitData = { Vertices, 0, 0 };
		Result = DX_Device->CreateBuffer(&VertexBufferDesc, &VertexBufferInitData, &DX_VertexBuffer);
		DXCHECK(Result);

		D3D11_BUFFER_DESC IndexBufferDesc =
		{
			sizeof(TriIx) * ARRAY_SIZE(BoxIxs),
			D3D11_USAGE_DEFAULT,
			D3D11_BIND_INDEX_BUFFER,
			0,
			0
		};
		D3D11_SUBRESOURCE_DATA IndexBufferInitData = { BoxIxs, 0, 0 };
		Result = DX_Device->CreateBuffer(&IndexBufferDesc, &IndexBufferInitData, &DX_IndexBuffer);
		DXCHECK(Result);

		ID3DBlob* VSCodeBlob = nullptr;
		ID3DBlob* PSCodeBlob = nullptr;

		Result = CompileShaderHelper(L"src/HLSL/MinShader.hlsl", "VSMain", "vs_5_0", &VSCodeBlob);
		DXCHECKMSG(Result, "Failed to compile Vertex Shader! :(\n");

		Result = CompileShaderHelper(L"src/HLSL/MinShader.hlsl", "PSMain", "ps_5_0", &PSCodeBlob);
		DXCHECKMSG(Result, "Failed to compile Pixel Shader! :(\n");

		if (VSCodeBlob && PSCodeBlob)
		{
			Result = DX_Device->CreateVertexShader(VSCodeBlob->GetBufferPointer(), VSCodeBlob->GetBufferSize(), nullptr, &DX_VertexShader);
			DXCHECKMSG(Result, "Device could not create vertex shader! :(\n");

			Result = DX_Device->CreatePixelShader(PSCodeBlob->GetBufferPointer(), PSCodeBlob->GetBufferSize(), nullptr, &DX_PixelShader);
			DXCHECKMSG(Result, "Device could not create pixel shader! :(\n");

			D3D11_INPUT_ELEMENT_DESC InputLayoutDesc[] =
			{
				{"POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				// { "TEXTURE", ... }
				{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			};
			UINT NumInputElements = ARRAYSIZE(InputLayoutDesc);

			Result = DX_Device->CreateInputLayout(InputLayoutDesc, NumInputElements, VSCodeBlob->GetBufferPointer(), VSCodeBlob->GetBufferSize(), &DX_InputLayout);
			DXCHECKMSG(Result, "Device could not create input layout! :(\n");
		}
		if (VSCodeBlob) { VSCodeBlob->Release(); }
		if (PSCodeBlob) { PSCodeBlob->Release(); }

		return Result;
	}

	void AtollGraphics::UpdateGraphicsState()
	{
		constexpr UINT Stride = sizeof(VertexColor);
		constexpr UINT Offset = 0;
		constexpr float fDepth = 1.0f;

		DX_ImmediateContext->IASetInputLayout(DX_InputLayout);
		DX_ImmediateContext->IASetVertexBuffers(0, 1, &DX_VertexBuffer, &Stride, &Offset);
		DX_ImmediateContext->IASetIndexBuffer(DX_IndexBuffer, DXGI_FORMAT_R32_UINT, 0);
		DX_ImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		DX_ImmediateContext->VSSetShader(DX_VertexShader, nullptr, 0);
		DX_ImmediateContext->PSSetShader(DX_PixelShader, nullptr, 0);
	}

	void AtollGraphics::Draw()
	{
		float ClearColor[4] = { 0.0125f, 0.15f, 0.3f, 1.0f };
		DX_ImmediateContext->ClearRenderTargetView(DX_RenderTargetView, ClearColor);
		DX_ImmediateContext->ClearDepthStencilView(DX_DepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

		UpdateGraphicsState();

		DX_ImmediateContext->DrawIndexed(TriIxCount * ARRAY_SIZE(BoxIxs), 0u, 0u);

		DX_SwapChain->Present(0, 0);
	}
}


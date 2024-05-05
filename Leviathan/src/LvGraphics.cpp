#include "LvCommon.h"

#include "LvGraphics.h"
#include "Leviathan.h"

#include <d3d11.h>
#include <d3dcompiler.h>
#include <dxgi.h>

namespace Leviathan
{
	D3D_FEATURE_LEVEL UsedFeatureLevel;

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

	VertexColor Vertices[] =
	{
		{{0.0f, 0.5f, 0.5f, 1.0f}, {0.0f, 0.0f, 1.0f, 1.0f}},
		{{0.5f, -0.5f, 0.5f, 1.0f}, {1.0f, 0.0f, 0.0f, 1.0f}},
		{{-0.5f, -0.5f, 0.5f, 1.0f}, {0.0f, 1.0f, 0.0f, 1.0f}}
	};

	int CompileShader(LPCWSTR SourceFileName, LPCSTR EntryPointFunction, LPCSTR Profile, ID3DBlob** ShaderBlob)
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
			0, //UINT Flags2
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

	void LvInitGraphics()
	{
		D3D_FEATURE_LEVEL SupportedFeatureLevels[] =
		{
			//D3D_FEATURE_LEVEL_12_2, D3D_FEATURE_LEVEL_12_1, D3D_FEATURE_LEVEL_12_0,
			D3D_FEATURE_LEVEL_11_1, D3D_FEATURE_LEVEL_11_0,
			//D3D_FEATURE_LEVEL_10_1, D3D_FEATURE_LEVEL_10_0, D3D_FEATURE_LEVEL_9_3, D3D_FEATURE_LEVEL_9_2, D3D_FEATURE_LEVEL_9_1
		};
		UINT NumSupportedFeatureLevels = ARRAYSIZE(SupportedFeatureLevels);
		D3D_FEATURE_LEVEL D3DFeatureLevel = D3D_FEATURE_LEVEL_11_0;

		DXCHECK(CreateDXGIFactory1(__uuidof(IDXGIFactory), (void**)&DX_Factory));
		IDXGIAdapter* DX_Adapter = nullptr;
		for (UINT AdapterIdx = 0; DX_Factory->EnumAdapters(AdapterIdx, &DX_Adapter); AdapterIdx++)
		{
			DX_AdapterList.push_back(DX_Adapter);
		}

		for (int AdapterIdx = 0; AdapterIdx < DX_AdapterList.size(); AdapterIdx++)
		{
			IDXGIOutput* DX_Output = nullptr;
			for (int OutputIdx = 0; DX_AdapterList[AdapterIdx]->EnumOutputs(OutputIdx, &DX_Output) != DXGI_ERROR_NOT_FOUND; OutputIdx++)
			{
				DXGI_FORMAT Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
				UINT NumModes = 0;
				DX_Output->GetDisplayModeList(Format, 0, &NumModes, nullptr);
				if (NumModes != 0)
				{
					DXGI_MODE_DESC* ModeList = new DXGI_MODE_DESC[NumModes];
					DX_Output->GetDisplayModeList(Format, 0, &NumModes, ModeList);
					OutputModeDescList.push_back(ModeList);
				}
			}
		}

		DXGI_SAMPLE_DESC SharedSampleDesc = {};
		SharedSampleDesc.Count = 4;
		SharedSampleDesc.Quality = (UINT)D3D11_STANDARD_MULTISAMPLE_PATTERN;

		DXGI_SWAP_CHAIN_DESC swapchain_desc = {};
		swapchain_desc.BufferCount = 2;
		swapchain_desc.BufferDesc.Width = ResX;
		swapchain_desc.BufferDesc.Height = ResY;
		swapchain_desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		swapchain_desc.BufferDesc.RefreshRate.Numerator = 60;
		swapchain_desc.BufferDesc.RefreshRate.Denominator = 1;
		swapchain_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapchain_desc.OutputWindow = LvWindow;
		swapchain_desc.SampleDesc = SharedSampleDesc;
		swapchain_desc.Windowed = true;

		UINT CreateDeviceFlags = 0;
	#if LV_DEBUG_BUILD()
		CreateDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
	#endif

		DXCHECK(D3D11CreateDeviceAndSwapChain(
			nullptr,					//IDXGIAdapter* pAdapter
			D3D_DRIVER_TYPE_HARDWARE,	//D3D_DRIVER_TYPE DriverType
			nullptr,					//HMODULE Software
			CreateDeviceFlags,							//UINT Flags
			SupportedFeatureLevels,	//const D3D_FEATURE_LEVEL* pFeatureLevels
			NumSupportedFeatureLevels,	//UINT FeatureLevels
			D3D11_SDK_VERSION,			//UINT SDKVersion
			&swapchain_desc,			//const DXGI_SWAP_CHAIN_DESC* pSwapChainDesc
			&DX_SwapChain,
			&DX_Device,
			&UsedFeatureLevel,
			&DX_ImmediateContext
		));

		DXCHECK(DX_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&DX_BackBuffer));

		DXCHECK(DX_Device->CreateRenderTargetView(DX_BackBuffer, nullptr, &DX_RenderTargetView));

		D3D11_RASTERIZER_DESC RasterDesc = {};
		RasterDesc.FillMode = D3D11_FILL_SOLID;
		RasterDesc.CullMode = D3D11_CULL_BACK;
		RasterDesc.FrontCounterClockwise = true;
		RasterDesc.DepthClipEnable = true;
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
		DepthDesc.SampleDesc = SharedSampleDesc;
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
		BlendDesc.AlphaToCoverageEnable;
		BlendDesc.AlphaToCoverageEnable;
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
			sizeof(VertexColor) * ARRAYSIZE(Vertices),
			D3D11_USAGE_DEFAULT,
			D3D11_BIND_VERTEX_BUFFER,
			0,
			0
		};
		D3D11_SUBRESOURCE_DATA VertexBufferInitData = { Vertices, 0, 0 };
		DXCHECK(DX_Device->CreateBuffer(&VertexBufferDesc, &VertexBufferInitData, &DX_VertexBuffer));

		UINT Indices[] = { 0, 2, 1 };
		D3D11_BUFFER_DESC IndexBufferDesc =
		{
			sizeof(Indices),
			D3D11_USAGE_DEFAULT,
			D3D11_BIND_INDEX_BUFFER,
			0,
			0
		};
		D3D11_SUBRESOURCE_DATA IndexBufferInitData = { Indices, 0, 0 };
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
			BufferDesc.ByteWidth = sizeof(MatrixR32) * 2;
			BufferDesc.Usage = D3D11_USAGE_DEFAULT;
			BufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			BufferDesc.CPUAccessFlags = 0;

			DXCHECK(DX_Device->CreateBuffer(&BufferDesc, nullptr, &DX_ViewProjBuffer));

			BufferDesc.ByteWidth = sizeof(MatrixR32);
			DXCHECK(DX_Device->CreateBuffer(&BufferDesc, nullptr, &DX_WorldBuffer));
		}
		if (VSCodeBlob) { VSCodeBlob->Release(); }
		if (PSCodeBlob) { PSCodeBlob->Release(); }
	}

	void LvUpdateGraphicsState()
	{
		UINT Stride = sizeof(VertexColor);
		UINT Offset = 0;
		float fDepth = 1.0f;

		WorldViewProjData CurrWVP;
		CurrWVP.View.Identity();
		CurrWVP.Proj.Identity();
		CurrWVP.World.Identity();

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

	void LvDraw()
	{
		float ClearColor[4] = { 0.0f, 0.125f, 0.3f, 1.0f };
		DX_ImmediateContext->ClearRenderTargetView(DX_RenderTargetView, ClearColor);
		DX_ImmediateContext->ClearDepthStencilView(DX_DepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

		LvUpdateGraphicsState();

		DX_ImmediateContext->DrawIndexed((UINT)1 * 3, 0u, 0u);

		DX_SwapChain->Present(0, 0);
	}

}


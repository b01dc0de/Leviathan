#include <cstdio>

#include <vector>

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <d3d11.h>
#include <d3dcompiler.h>
#include <dxgi.h>

#if defined(DEBUG) || defined(_DEBUG)
#define DEBUG_BUILD 1
#else
#define DEBUG_BUILD 0
#endif

// Globals (for now)
bool bGlobalRunning = false;
HWND GlobalWindow;
LPCWSTR GlobalAppName = L"HelloTriangle";
UINT GlobalWinResX = 1600;
UINT GlobalWinResY = 900;

#define DXCHECK(Result) if (FAILED(Result)) {return (Result);}
#define DXCHECKMSG(Result, Msg) if (FAILED(Result)) {OutputDebugStringA(Msg); return (Result);}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

namespace SimpleGraphics
{
	IDXGISwapChain* DX_SwapChain = nullptr;
	ID3D11Device* DX_Device = nullptr;
	D3D_FEATURE_LEVEL UsedFeatureLevel;
	ID3D11DeviceContext* DX_ImmediateContext = nullptr;

	ID3D11Texture2D* DX_BackBuffer = nullptr;
	ID3D11RenderTargetView* DX_RenderTargetView = nullptr;

	IDXGIFactory1* DX_Factory = nullptr;
	std::vector<IDXGIAdapter*> DX_AdapterList;
	std::vector<IDXGIOutput*> DX_Outputs;
	std::vector<DXGI_MODE_DESC*> OutputModeDescList;

	ID3D11RasterizerState* DX_RasterizerState = nullptr;
	ID3D11Texture2D* DX_DepthStencil = nullptr;
	ID3D11DepthStencilView* DX_DepthStencilView = nullptr;
	ID3D11BlendState* DX_BlendState = nullptr;

	ID3D11Buffer* DX_VertexBuffer = nullptr;
	ID3D11Buffer* DX_IndexBuffer = nullptr;

	ID3D11VertexShader* DX_VertexShader = nullptr;
	ID3D11PixelShader* DX_PixelShader = nullptr;
	ID3D11InputLayout* DX_InputLayout = nullptr;

	ID3D11Buffer* DX_WorldBuffer = nullptr;
	ID3D11Buffer* DX_ViewProjBuffer = nullptr;

int InitD3D11()
{
	HRESULT Result = S_OK;

	D3D_FEATURE_LEVEL SupportedFeatureLevels[] =
	{
		//D3D_FEATURE_LEVEL_12_2
		//D3D_FEATURE_LEVEL_12_1,
		//D3D_FEATURE_LEVEL_12_0,
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
		//D3D_FEATURE_LEVEL_10_1,
		//D3D_FEATURE_LEVEL_10_0,
		//D3D_FEATURE_LEVEL_9_3,
		//D3D_FEATURE_LEVEL_9_2,
		//D3D_FEATURE_LEVEL_9_1
	};
	UINT NumSupportedFeatureLevels = ARRAYSIZE(SupportedFeatureLevels);
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
	swapchain_desc.BufferDesc.Width = GlobalWinResX;
	swapchain_desc.BufferDesc.Height = GlobalWinResY;
	swapchain_desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapchain_desc.BufferDesc.RefreshRate.Numerator = 60;
	swapchain_desc.BufferDesc.RefreshRate.Denominator = 1;
	swapchain_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapchain_desc.OutputWindow = GlobalWindow;
	swapchain_desc.SampleDesc = SharedSampleDesc;
	swapchain_desc.Windowed = true;

	UINT CreateDeviceFlags = 0;
#if DEBUG_BUILD
	CreateDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	Result = D3D11CreateDeviceAndSwapChain(
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
	DepthDesc.Width = GlobalWinResX;
	DepthDesc.Height = GlobalWinResY;
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
	BlendDesc.AlphaToCoverageEnable;
	BlendDesc.AlphaToCoverageEnable;
	BlendDesc.RenderTarget[0] = RTVBlendDesc;

	Result = DX_Device->CreateBlendState(&BlendDesc, &DX_BlendState);
	DXCHECK(Result);

	D3D11_VIEWPORT Viewport_Desc = {};
	Viewport_Desc.Width = (FLOAT)GlobalWinResX;
	Viewport_Desc.Height = (FLOAT)GlobalWinResY;
	Viewport_Desc.MinDepth = 0.0f;
	Viewport_Desc.MaxDepth = 1.0f;
	Viewport_Desc.TopLeftX = 0;
	Viewport_Desc.TopLeftY = 0;
	DX_ImmediateContext->RSSetViewports(1, &Viewport_Desc);

	return Result;
}

int CompileShaderHelper(LPCWSTR SourceFileName, LPCSTR EntryPointFunction, LPCSTR Profile, ID3DBlob** ShaderBlob)
{
	HRESULT Result = S_OK;

	if (SourceFileName == nullptr || EntryPointFunction == nullptr || Profile == nullptr || ShaderBlob == nullptr)
	{
		return E_INVALIDARG;
	}

	*ShaderBlob = nullptr;

	UINT CompileFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if DEBUG_BUILD
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

	Result = D3DCompileFromFile
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

struct VertexColor
{
	float Position[4];
	float Color[4];
};

VertexColor Vertices[] =
{
	{{0.0f, 0.5f, 0.5f, 1.0f}, {0.0f, 0.0f, 1.0f, 1.0f}},
	{{0.5f, -0.5f, 0.5f, 1.0f}, {1.0f, 0.0f, 0.0f, 1.0f}},
	{{-0.5f, -0.5f, 0.5f, 1.0f}, {0.0f, 1.0f, 0.0f, 1.0f}}
};

int InitDrawPipeline()
{
	HRESULT Result = S_OK;

	D3D11_BUFFER_DESC VertexBufferDesc =
	{
		sizeof(VertexColor) * ARRAYSIZE(Vertices),
		D3D11_USAGE_DEFAULT,
		D3D11_BIND_VERTEX_BUFFER,
		0,
		0
	};
	D3D11_SUBRESOURCE_DATA VertexBufferInitData = { Vertices, 0, 0 };
	Result = DX_Device->CreateBuffer(&VertexBufferDesc, &VertexBufferInitData, &DX_VertexBuffer);
	DXCHECK(Result);

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
	Result = DX_Device->CreateBuffer(&IndexBufferDesc, &IndexBufferInitData, &DX_IndexBuffer);
	DXCHECK(Result);

	ID3DBlob* VSCodeBlob = nullptr;
	ID3DBlob* PSCodeBlob = nullptr;

	Result = CompileShaderHelper(L"src/SimpleShader.hlsl", "VSMain", "vs_5_0"/*"vs_5_0"*/, &VSCodeBlob);
	DXCHECKMSG(Result, "Failed to compile Vertex Shader! :(\n");

	Result = CompileShaderHelper(L"src/SimpleShader.hlsl", "PSMain", "ps_5_0"/*"ps_5_0"*/, &PSCodeBlob);
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

		const size_t Size4x4Matrix = sizeof(float) * 4 * 4;

		D3D11_BUFFER_DESC BufferDesc = {};
		BufferDesc.ByteWidth = Size4x4Matrix * 2;
		BufferDesc.Usage = D3D11_USAGE_DEFAULT;
		BufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		BufferDesc.CPUAccessFlags = 0;

		Result = DX_Device->CreateBuffer(&BufferDesc, nullptr, &DX_ViewProjBuffer);
		DXCHECKMSG(Result, "Could not create ViewProj cbuffer! :(\n");

		BufferDesc.ByteWidth = Size4x4Matrix;
		Result = DX_Device->CreateBuffer(&BufferDesc, nullptr, &DX_WorldBuffer);
		DXCHECKMSG(Result, "Could not create World cbuffer! :(\n");
	}
	if (VSCodeBlob) { VSCodeBlob->Release(); }
	if (PSCodeBlob) { PSCodeBlob->Release(); }

	return Result;
}

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

void UpdateGraphicsState()
{
	UINT Stride = sizeof(VertexColor);
	UINT Offset = 0;
	float fDepth = 1.0f;

	WorldViewProjData CurrWVP;
	CurrWVP.View.Identity();
	CurrWVP.Proj.Identity();
	CurrWVP.Proj.Row0.X = +2.0f / GlobalWinResX;
	CurrWVP.Proj.Row1.Y = +2.0f / GlobalWinResY;
	CurrWVP.Proj.Row2.Z = -2.0f / fDepth;
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

	return;
};

void Draw()
{
	float ClearColor[4] = { 0.0f, 0.125f, 0.3f, 1.0f };
	DX_ImmediateContext->ClearRenderTargetView(DX_RenderTargetView, ClearColor);
	DX_ImmediateContext->ClearDepthStencilView(DX_DepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

	UpdateGraphicsState();

	DX_ImmediateContext->DrawIndexed((UINT)1 * 3, 0u, 0u);

	DX_SwapChain->Present(0, 0);
}


int InitGraphics()
{
	InitD3D11();
	InitDrawPipeline();

	return 0; //
}

};

HWND InitWindow(HINSTANCE hInstance)
{
	WNDCLASSEX WndClass = {};
	WndClass.cbSize = sizeof(WNDCLASSEX);
	WndClass.style = CS_GLOBALCLASS | CS_HREDRAW | CS_VREDRAW;
	WndClass.lpfnWndProc = WindowProc;
	WndClass.hInstance = hInstance;
	WndClass.lpszClassName = GlobalAppName;

	ATOM RegClassResult = RegisterClassEx(&WndClass);

	RECT WndRect = { 0, 0, (LONG)GlobalWinResX, (LONG)GlobalWinResY };
	UINT WndStyle = WS_CAPTION;
	UINT WndExStyle = WS_EX_OVERLAPPEDWINDOW;
	AdjustWindowRectEx(&WndRect, WndStyle, FALSE, WndExStyle);

	HWND hWindow = CreateWindowEx(
		WndExStyle,
		GlobalAppName,
		GlobalAppName,
		WndStyle,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		WndRect.right - WndRect.left,
		WndRect.bottom - WndRect.top,
		nullptr,
		nullptr,
		hInstance,
		nullptr // CKA_NOTE: Might want to use this in the future
	);

	return hWindow;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow)
{
	HWND hWindow = InitWindow(hInstance);

	if (hWindow)
	{
		GlobalWindow = hWindow;

		SimpleGraphics::InitGraphics();

		ShowWindow(hWindow, nCmdShow);

		bGlobalRunning = true;
		while (bGlobalRunning)
		{
			// Get input
			MSG msg;
			BOOL MsgResult;
			while ((MsgResult = PeekMessage(&msg, hWindow, 0, 0, PM_REMOVE)) > 0)
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			if (MsgResult == -1)
			{
				// CKA_TODO: Error handling?
				//bGlobalRunning = false;
				break;
			}

			// Update
			UpdateWindow(hWindow);

			SimpleGraphics::Draw();
		}
	}
	else
	{
		// Error
	}

	return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT Result;
	switch (uMsg)
	{
		case WM_LBUTTONDOWN:
		case WM_RBUTTONDOWN:
		case WM_KEYDOWN:
		{
			bGlobalRunning = false;
		}
		default:
		{
			Result = DefWindowProc(hwnd, uMsg, wParam, lParam);
		} break;
	}
	
	return Result;
}

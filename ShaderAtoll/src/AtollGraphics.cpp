#include "AtollGraphics.h"
#include "AtollDrawPipelineState.h"

namespace ShaderAtoll
{
	template <>
	DXHandle<ID3D11Device>::DXHandle(ID3D11Device* InHandle)
		: Handle(InHandle)
	{
	}

	DXHandle<IDXGISwapChain1> AtollGraphics::DX_SwapChain1;
	DXHandle<ID3D11Device> AtollGraphics::DX_Device;
	DXHandle<ID3D11DeviceContext> AtollGraphics::DX_ImmediateContext;
	DXHandle<ID3D11Texture2D> AtollGraphics::DX_BackBuffer;
	DXHandle<ID3D11Texture2D> AtollGraphics::DX_RenderTargetTexture;
	DXHandle<ID3D11RenderTargetView> AtollGraphics::DX_RenderTargetView;
	DXHandle<IDXGIFactory2> AtollGraphics::DX_Factory2;
	DXHandle<ID3D11RasterizerState> AtollGraphics::DX_RasterizerState;
	DXHandle<ID3D11Texture2D> AtollGraphics::DX_DepthStencil;
	DXHandle<ID3D11DepthStencilView> AtollGraphics::DX_DepthStencilView;
	DXHandle<ID3D11BlendState> AtollGraphics::DX_BlendState;
	DXHandle<ID3D11Buffer> AtollGraphics::DX_VertexBuffer;
	DXHandle<ID3D11Buffer> AtollGraphics::DX_IndexBuffer;
	DXHandle<ID3D11Buffer> AtollGraphics::DX_GlobalsBuffer;

	D3D_FEATURE_LEVEL AtollGraphics::UsedFeatureLevel = {};

	DrawPipelineState AtollGraphics::Live_DrawState = {};
	DrawPipelineState AtollGraphics::Error_DrawState = {};
	DrawPipelineState AtollGraphics::Example_DrawState = {};
	int AtollGraphics::SelectedExampleNum = 1;
	DrawPipelineState* AtollGraphics::CurrActive_DrawState = nullptr;

	DXHandleMgr* DXHandleMgr::_Inst = nullptr;
	DXHandleMgr* DXHandleMgr::Inst()
	{
		if (nullptr == _Inst)
		{
			_Inst = new DXHandleMgr;
		}
		return _Inst;
	}

	void DXHandleMgr::LocalAdd(IUnknown** InHandle)
	{
		CHECK(nullptr != InHandle);
		HandleList.AddItem(InHandle);
	}
	void DXHandleMgr::LocalSafeReleaseAll()
	{
		for (int HandleIdx = 0; HandleIdx < HandleList.NumItems; HandleIdx++)
		{
			IUnknown*& CurrHandle = (*HandleList[HandleIdx]);
			if (CurrHandle)
			{
				CurrHandle->Release();
				CurrHandle = nullptr;
			}
		}
		HandleList.Reset();
	}

	void DXHandleMgr::Add(IUnknown** InHandle)
	{
		Inst()->LocalAdd(InHandle);
	}
	void DXHandleMgr::SafeReleaseAll()
	{
		Inst()->LocalSafeReleaseAll();
	}
	void DXHandleMgr::Term()
	{
		if (_Inst)
		{
			_Inst->SafeReleaseAll();
			delete _Inst;
			_Inst = nullptr;
		}
	}

	ShaderGlobals GlobalsData
	{
		0.0f,
		msPerFrame_60,
		WinResX,
		WinResY,
		MousePosX,
		MousePosY
	};

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

	D3D11_INPUT_ELEMENT_DESC VxColor_InputLayoutDesc[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		// { "TEXTURE", ... }
		{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT VxColor_NumInputElements = ARRAYSIZE(VxColor_InputLayoutDesc);

	namespace ShaderUtils
	{
		LPCWSTR Base_ShaderFilename = L"src/HLSL/BaseShader.hlsl";

		bool CompileShader(ID3D11Device* Device, const D3D_SHADER_MACRO* InMacroDefines, DrawPipelineState& Out_DrawState)
		{
			DrawPipelineState TmpDrawState = {};
			bool bResult = CompileDrawPipeline
			(
				Device,
				Base_ShaderFilename,
				InMacroDefines,
				VxColor_InputLayoutDesc,
				VxColor_NumInputElements,
				&TmpDrawState
			);
			if (TmpDrawState.VertexShader && TmpDrawState.InputLayout && TmpDrawState.PixelShader)
			{
				Out_DrawState = TmpDrawState;
			}
			return bResult;
		}

		bool CompileErrorShader(ID3D11Device* Device, DrawPipelineState& Out_ErrorDrawState)
		{
			static constexpr D3D_SHADER_MACRO HLSL_MacroDefines[] =
			{
				"ENABLE_VERTEX_COLOR", "0",
				"SELECT_ERROR_SHADER", "1",
				"SELECT_LIVE_SHADER", "0",
				"SELECT_EXAMPLE_SHADER", "0",
				NULL, NULL
			};

			bool bResult = CompileShader(Device, HLSL_MacroDefines, Out_ErrorDrawState);
			CHECK(bResult);

			return bResult;
		}

		bool CompileLiveShader(ID3D11Device* Device, bool bAutoLive, DrawPipelineState& Out_LiveDrawState)
		{
			D3D_SHADER_MACRO HLSL_MacroDefines[] =
			{
				"ENABLE_VERTEX_COLOR", "0",
				"SELECT_ERROR_SHADER", "0",
				"SELECT_LIVE_SHADER", "1",
				"SELECT_EXAMPLE_SHADER", "0",
				"AUTO_LIVE", bAutoLive ? "1" : "0",
				NULL, NULL
			};

			bool bResult = CompileShader(Device, HLSL_MacroDefines, Out_LiveDrawState);
			return bResult;
		}

		bool CompileExampleShader(ID3D11Device* Device, int ExampleNum, DrawPipelineState& Out_ExampleDrawState)
		{
			static constexpr int ExampleMacroIdx = 3;
			static constexpr int DefaultExampleNum = 1;

			D3D_SHADER_MACRO HLSL_MacroDefines[] =
			{
				"ENABLE_VERTEX_COLOR", "0",
				"SELECT_ERROR_SHADER", "0",
				"SELECT_LIVE_SHADER", "0",
				"SELECT_EXAMPLE_SHADER", NULL,
				NULL, NULL
			};
			static LPCSTR SelectedExampleDefVals[] = { "0", "1", "2", "3", "4", "5", "6", "7", "8", "9" };
			int SelectedExampleNum = DefaultExampleNum;
			if (0 <= ExampleNum && ExampleNum <= 9)
			{
				SelectedExampleNum = ExampleNum;
			}
			HLSL_MacroDefines[ExampleMacroIdx].Definition = SelectedExampleDefVals[SelectedExampleNum];

			bool bResult = CompileShader(Device, HLSL_MacroDefines, Out_ExampleDrawState);
			CHECK(bResult);

			return bResult;
		}
	}


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

		CreateDXGIFactory1(__uuidof(IDXGIFactory2), (void**)&DX_Factory2);

		UINT CreateDeviceFlags = 0;
#if BUILD_DEBUG()
		CreateDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif // BUILD_DEBUG()

		Result = D3D11CreateDevice(
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
		);
		DXCHECK(Result);

		DXGI_SWAP_CHAIN_DESC1 SwapChainDesc1 = {};
		SwapChainDesc1.Width = WinResX;
		SwapChainDesc1.Height = WinResX;
		SwapChainDesc1.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		SwapChainDesc1.Stereo = FALSE;
		SwapChainDesc1.SampleDesc.Count = 1;
		SwapChainDesc1.SampleDesc.Quality = 0;
		SwapChainDesc1.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		SwapChainDesc1.BufferCount = 2;
		SwapChainDesc1.Scaling = DXGI_SCALING_NONE;
		SwapChainDesc1.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
		SwapChainDesc1.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
		SwapChainDesc1.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING;
		DXGI_SWAP_CHAIN_FULLSCREEN_DESC SwapChainFullscreenDesc = {};

		CHECK(DX_Factory2 && DX_Device);
		Result = DX_Factory2->CreateSwapChainForHwnd(
			DX_Device, 
			hWindow,
			&SwapChainDesc1,
			nullptr, //&SwapChainFullscreenDesc,
			nullptr,
			&DX_SwapChain1
		);
		DXCHECK(Result);

		Result = DX_SwapChain1->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&DX_BackBuffer);
		DXCHECK(Result);

		DXGI_SAMPLE_DESC SampleDescMS = {};
		SampleDescMS.Count = 4;
		SampleDescMS.Quality = (UINT)D3D11_STANDARD_MULTISAMPLE_PATTERN;

		D3D11_TEXTURE2D_DESC RTT_Desc;
		RTT_Desc.Width = WinResX;
		RTT_Desc.Height = WinResY;
		RTT_Desc.MipLevels = 1;
		RTT_Desc.ArraySize = 1;
		RTT_Desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		RTT_Desc.SampleDesc = SampleDescMS;
		RTT_Desc.Usage = D3D11_USAGE_DEFAULT;
		RTT_Desc.BindFlags = D3D11_BIND_RENDER_TARGET;
		RTT_Desc.CPUAccessFlags = 0;
		RTT_Desc.MiscFlags = 0;

		Result = DX_Device->CreateTexture2D(&RTT_Desc, nullptr, &DX_RenderTargetTexture);
		Result = DX_Device->CreateRenderTargetView(DX_RenderTargetTexture, nullptr, &DX_RenderTargetView);
		DXCHECK(Result);

		D3D11_TEXTURE2D_DESC DepthDesc = {};
		DepthDesc.Width = ShaderAtoll::WinResX;
		DepthDesc.Height = ShaderAtoll::WinResY;
		DepthDesc.MipLevels = 1;
		DepthDesc.ArraySize = 1;
		DepthDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		DepthDesc.SampleDesc = SampleDescMS;
		DepthDesc.Usage = D3D11_USAGE_DEFAULT;
		DepthDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		DepthDesc.CPUAccessFlags = 0;
		DepthDesc.MiscFlags = 0;

		Result = DX_Device->CreateTexture2D(&DepthDesc, nullptr, &DX_DepthStencil);
		DXCHECK(Result);

		Result = DX_Device->CreateDepthStencilView(DX_DepthStencil, nullptr, &DX_DepthStencilView);
		DXCHECK(Result);

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

		D3D11_BUFFER_DESC GlobalsBufferDesc =
		{
			sizeof(ShaderGlobals),
			D3D11_USAGE_DEFAULT,
			D3D11_BIND_CONSTANT_BUFFER,
			0,
			0
		};
		D3D11_SUBRESOURCE_DATA GlobalsBufferInitData = {&GlobalsData, 0, 0};
		Result = DX_Device->CreateBuffer(&GlobalsBufferDesc, &GlobalsBufferInitData, &DX_GlobalsBuffer);
		DXCHECK(Result);

		bool bShaderCompiled = ShaderUtils::CompileErrorShader
		(
			DX_Device,
			Error_DrawState
		);
		if (!bShaderCompiled)
		{
			DebugBreak();
			return 0;
		}

		bShaderCompiled = ShaderUtils::CompileLiveShader
		(
			DX_Device,
			true,
			Live_DrawState
		);
		if (!bShaderCompiled) { DebugBreak(); return 0; }

		bShaderCompiled = ShaderUtils::CompileExampleShader
		(
			DX_Device,
			1,
			Example_DrawState
		);
		if (!bShaderCompiled) { DebugBreak(); return 0; }

		//CurrActive_DrawState = &Live_DrawState;
		CurrActive_DrawState = &Error_DrawState;
		//CurrActive_DrawState = &Example_DrawState;

		return SUCCEEDED(Result);
	}

	void AtollGraphics::UpdateGraphicsState()
	{
		constexpr UINT Stride = sizeof(VertexColor);
		constexpr UINT Offset = 0;
		constexpr float fDepth = 1.0f;

		CHECK(CurrActive_DrawState
			&& CurrActive_DrawState->VertexShader
			&& CurrActive_DrawState->InputLayout
			&& CurrActive_DrawState->PixelShader);

		GlobalsData =
		{
			(float)AppTime_s,
			(float)DeltaTime_ms,
			WinResX,
			WinResY,
			MousePosX,
			MousePosY
		};

		DX_ImmediateContext->IASetInputLayout(CurrActive_DrawState->InputLayout);
		DX_ImmediateContext->IASetVertexBuffers(0, 1, &DX_VertexBuffer, &Stride, &Offset);
		DX_ImmediateContext->IASetIndexBuffer(DX_IndexBuffer, DXGI_FORMAT_R32_UINT, 0);
		DX_ImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		DX_ImmediateContext->UpdateSubresource(DX_GlobalsBuffer, 0, nullptr, &GlobalsData, sizeof(ShaderGlobals), 0);

		DX_ImmediateContext->VSSetShader(CurrActive_DrawState->VertexShader, nullptr, 0);
		DX_ImmediateContext->VSSetConstantBuffers(0, 1, &DX_GlobalsBuffer);

		const D3D11_VIEWPORT Viewport_Desc = { 0, 0, (FLOAT)WinResX, (FLOAT)WinResY, 0.0f, 1.0f };
		DX_ImmediateContext->RSSetViewports(1, &Viewport_Desc);

		DX_ImmediateContext->PSSetShader(CurrActive_DrawState->PixelShader, nullptr, 0);
		DX_ImmediateContext->PSSetConstantBuffers(0, 1, &DX_GlobalsBuffer);

		DX_ImmediateContext->OMSetRenderTargets(1, &DX_RenderTargetView, DX_DepthStencilView);

		float ClearColor[4] = { 0.025f, 0.15f, 0.3f, 1.0f };
		DX_ImmediateContext->ClearRenderTargetView(DX_RenderTargetView, ClearColor);
		DX_ImmediateContext->ClearDepthStencilView(DX_DepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
	}

	void AtollGraphics::RecompileShaders()
	{
		bool bResult = false;

		DrawPipelineState Old_DrawState = *CurrActive_DrawState;

		if (CurrActive_DrawState == &Live_DrawState)
		{
			bResult = ShaderUtils::CompileLiveShader(DX_Device, false, Live_DrawState);
		}
		else if (CurrActive_DrawState == &Error_DrawState)
		{
			bResult = ShaderUtils::CompileErrorShader(DX_Device, Error_DrawState);
		}
		else if (CurrActive_DrawState == &Example_DrawState)
		{
			bResult = ShaderUtils::CompileExampleShader(DX_Device, SelectedExampleNum, Example_DrawState);
			if (!bResult)
			{
			}
		}
		else
		{
			CHECK(false);
			return;
		}

		if (bResult)
		{
			Old_DrawState.Release();
		}
		else
		{
			CurrActive_DrawState->Release();
			*CurrActive_DrawState = Old_DrawState;
			CurrActive_DrawState = &Error_DrawState;
		}
	}

	void AtollGraphics::Draw()
	{
		UpdateGraphicsState();

		DX_ImmediateContext->DrawIndexed(TriIxCount * ARRAY_SIZE(BoxIxs), 0u, 0u);
		DX_ImmediateContext->ResolveSubresource(DX_BackBuffer, 0, DX_RenderTargetTexture, 0, DXGI_FORMAT_R8G8B8A8_UNORM);

		const DXGI_PRESENT_PARAMETERS PresentParameters = {};
		const UINT PresentFlags = DXGI_PRESENT_ALLOW_TEARING;
		DX_SwapChain1->Present1(0, PresentFlags, &PresentParameters);
	}

	int AtollGraphics::TermGraphics()
	{
		DX_ImmediateContext->ClearState();
		Live_DrawState.Release();
		Error_DrawState.Release();
		Example_DrawState.Release();
		DXHandleMgr::Term();

#if BUILD_DEBUG()
		ID3D11Debug* DX_Debug = nullptr;
		DX_Device->QueryInterface(&DX_Debug);
		if (DX_Debug)
		{
			DX_Debug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL | D3D11_RLDO_IGNORE_INTERNAL);
			DX_Debug->Release();
		}
#endif // BUILD_DEBUG()
		DX_Device->Release();

		return 0;
	}
}


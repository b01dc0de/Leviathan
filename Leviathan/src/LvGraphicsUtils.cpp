#include "LvCommon.h"

#include "LvGraphics.h"
#include "LvGraphicsUtils.h"

namespace Leviathan
{
	namespace LvGraphicsUtils
	{
		ID3D11Device* OwnedDxRefs::DxDeviceRef = nullptr;
		LvArray<IUnknown*> OwnedDxRefs::DxRefsList{};

		void OwnedDxRefs::Add(IUnknown* NewRef)
		{
			if (nullptr == DxDeviceRef)
			{
				Assert(0 == DxRefsList.NumItems);
				DxDeviceRef = (ID3D11Device*)NewRef;
				// Downcast for now
				// CKA_TODO: Is there a better way to verify that this first ref is actually a DXDevice
			}
			else
			{
				DxRefsList.AddItem(NewRef);
			}
		}

		void OwnedDxRefs::ReleaseList()
		{
			// CKA_NOTE: Release in reverse allocation order
			for (int RefIdx = DxRefsList.NumItems - 1; RefIdx >= 0; RefIdx--)
			{
				DXSAFERELEASE(DxRefsList[RefIdx]);
			}
			DxRefsList.Empty();
		}

		void OwnedDxRefs::ReleaseDevice()
		{
			Assert(0 == DxRefsList.NumItems && nullptr != DxDeviceRef);
			DXSAFERELEASE(DxDeviceRef);
		}

		D3D11_BUFFER_DESC GetDefaultBufferDesc(unsigned int SizeBytes)
		{
			const D3D11_BUFFER_DESC DefaultBufferDesc = 
			{
				SizeBytes,
				D3D11_USAGE_DEFAULT,
				D3D11_BIND_VERTEX_BUFFER,
				0,
				0
			};
			return DefaultBufferDesc;
		}

		void CreateWVPBuffers(ID3D11Device* DXDevice, ID3D11Buffer** Out_WBuffer, ID3D11Buffer** Out_VPBuffer)
		{
			Assert(DXDevice);
			ID3D11Buffer* WBuffer = nullptr;
			ID3D11Buffer* VPBuffer = nullptr;

			D3D11_BUFFER_DESC WorldBufferDesc = {};
			WorldBufferDesc.ByteWidth = sizeof(fMatrix);
			WorldBufferDesc.Usage = D3D11_USAGE_DEFAULT;
			WorldBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			WorldBufferDesc.CPUAccessFlags = 0;
			DXCHECK(DXDevice->CreateBuffer(&WorldBufferDesc, nullptr, &WBuffer));

			D3D11_BUFFER_DESC ViewProjBufferDesc = {};
			ViewProjBufferDesc.ByteWidth = sizeof(fMatrix) * 2;
			ViewProjBufferDesc.Usage = D3D11_USAGE_DEFAULT;
			ViewProjBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			ViewProjBufferDesc.CPUAccessFlags = 0;
			DXCHECK(DXDevice->CreateBuffer(&ViewProjBufferDesc, nullptr, &VPBuffer));

			Assert(WBuffer && VPBuffer);
			*Out_WBuffer = WBuffer;
			*Out_VPBuffer = VPBuffer;
		}

		void CreateDefaultSwapChainRenderTarget
		(
			ID3D11Device* DXDevice,
			HWND WindowHandle,
			IDXGISwapChain1** Out_SwapChain1,
			ID3D11Texture2D** Out_BackBuffer,
			ID3D11Texture2D** Out_RenderTargetTexture,
			ID3D11RenderTargetView** Out_RenderTargetView
		)
		{
			Assert(DXDevice);
			IDXGISwapChain1* SwapChain1 = nullptr;
			ID3D11Texture2D* BackBuffer = nullptr;
			ID3D11Texture2D* RenderTargetTexture = nullptr;
			ID3D11RenderTargetView* RenderTargetView = nullptr;

			DXGI_SAMPLE_DESC Shared_RT_SampleDesc = {};
			Shared_RT_SampleDesc.Count = 4;
			Shared_RT_SampleDesc.Quality = (UINT)D3D11_STANDARD_MULTISAMPLE_PATTERN;

			DXGI_FORMAT Default_RenderTargetFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
			DXGI_SWAP_CHAIN_DESC1 SwapChainDesc1 = {};
			SwapChainDesc1.Width = ResX;
			SwapChainDesc1.Height = ResY;
			SwapChainDesc1.Format = Default_RenderTargetFormat;
			SwapChainDesc1.Stereo = FALSE;
			SwapChainDesc1.SampleDesc.Count = 1;
			SwapChainDesc1.SampleDesc.Quality = 0;
			SwapChainDesc1.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
			SwapChainDesc1.BufferCount = 2;
			SwapChainDesc1.Scaling = DXGI_SCALING_STRETCH;
			SwapChainDesc1.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
			SwapChainDesc1.AlphaMode = DXGI_ALPHA_MODE_IGNORE;
			SwapChainDesc1.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING;
			IDXGIFactory2* DXGI_Factory2 = nullptr;
			DXCHECK(CreateDXGIFactory1(IID_PPV_ARGS(&DXGI_Factory2)));
			DXCHECK(DXGI_Factory2->CreateSwapChainForHwnd(
				DXDevice,
				WindowHandle,
				&SwapChainDesc1,
				nullptr,
				nullptr,
				&SwapChain1
			));
			DXSAFERELEASE(DXGI_Factory2);
			DXCHECK(SwapChain1->GetBuffer(0, IID_PPV_ARGS(&BackBuffer)));

			// CKA_TODO: Figure out a better system for LvGraphics defaults
			D3D11_TEXTURE2D_DESC RTT_Desc;
			RTT_Desc.Width = ResX;
			RTT_Desc.Height = ResY;
			RTT_Desc.MipLevels = 1;
			RTT_Desc.ArraySize = 1;
			RTT_Desc.Format = Default_RenderTargetFormat;
			RTT_Desc.SampleDesc = Shared_RT_SampleDesc;
			RTT_Desc.Usage = D3D11_USAGE_DEFAULT;
			RTT_Desc.BindFlags = D3D11_BIND_RENDER_TARGET;
			RTT_Desc.CPUAccessFlags = 0;
			RTT_Desc.MiscFlags = 0;
			DXCHECK(DXDevice->CreateTexture2D(&RTT_Desc, nullptr, &RenderTargetTexture));
			D3D11_RENDER_TARGET_VIEW_DESC RTV_Desc = {};
			RTV_Desc.Format = RTT_Desc.Format;
			RTV_Desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DMS;
			RTV_Desc.Texture2D.MipSlice = 0;
			DXCHECK(DXDevice->CreateRenderTargetView(RenderTargetTexture, nullptr, &RenderTargetView));

			Assert(SwapChain1 && BackBuffer && RenderTargetTexture && RenderTargetView);
			*Out_SwapChain1 = SwapChain1;
			*Out_BackBuffer = BackBuffer;
			*Out_RenderTargetTexture = RenderTargetTexture;
			*Out_RenderTargetView = RenderTargetView;
		}
		void CreateDefaultDepthStencilState
		(
			ID3D11Device* DXDevice,
			ID3D11DepthStencilState** Out_DepthStencilState,
			ID3D11Texture2D** Out_DepthStencilTexture,
			ID3D11DepthStencilView** Out_DepthStencilView
		)
		{
			Assert(DXDevice);
			ID3D11DepthStencilState* DepthStencilState = nullptr;
			ID3D11Texture2D* DepthStencilTexture = nullptr;
			ID3D11DepthStencilView* DepthStencilView = nullptr;

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
			DXCHECK(DXDevice->CreateDepthStencilState(&DepthStencilStateDesc, &DepthStencilState));

			DXGI_SAMPLE_DESC Shared_RT_SampleDesc = {};
			Shared_RT_SampleDesc.Count = 4;
			Shared_RT_SampleDesc.Quality = (UINT)D3D11_STANDARD_MULTISAMPLE_PATTERN;
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
			DXCHECK(DXDevice->CreateTexture2D(&DepthStencilTextureDesc, nullptr, &DepthStencilTexture));

			D3D11_DEPTH_STENCIL_VIEW_DESC DepthStencilViewDesc = {};
			DepthStencilViewDesc.Format = DepthStencilTextureDesc.Format;
			DepthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
			DepthStencilViewDesc.Texture2D.MipSlice = 0;
			DXCHECK(DXDevice->CreateDepthStencilView(DepthStencilTexture, &DepthStencilViewDesc, &DepthStencilView));

			Assert(DepthStencilState && DepthStencilTexture && DepthStencilView);
			*Out_DepthStencilState = DepthStencilState;
			*Out_DepthStencilTexture = DepthStencilTexture;
			*Out_DepthStencilView = DepthStencilView;
		}
		void CreateDefaultRasterizerBlendState
		(
			ID3D11Device* DXDevice,
			ID3D11RasterizerState** Out_RasterizerState,
			ID3D11BlendState** Out_BlendState
		)
		{
			Assert(DXDevice);
			ID3D11RasterizerState* RasterizerState = nullptr;
			ID3D11BlendState* BlendState = nullptr;

			D3D11_RASTERIZER_DESC RasterDesc = {};
			RasterDesc.FillMode = D3D11_FILL_SOLID;
			RasterDesc.CullMode = D3D11_CULL_BACK;
			RasterDesc.FrontCounterClockwise = true;
			RasterDesc.DepthClipEnable = true;
			RasterDesc.ScissorEnable = false;
			RasterDesc.MultisampleEnable = true;
			RasterDesc.AntialiasedLineEnable = true;
			DXCHECK(DXDevice->CreateRasterizerState(&RasterDesc, &RasterizerState));

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
			DXCHECK(DXDevice->CreateBlendState(&BlendDesc, &BlendState));

			Assert(RasterizerState && BlendState);
			*Out_RasterizerState = RasterizerState;
			*Out_BlendState = BlendState;
		}

		void ReportLiveObjects(ID3D11Device* DXDevice)
		{
		#if LV_CONFIG_DEBUG()
			DxHandle<ID3D11Debug> DX_Debug = nullptr;
			DXCHECK(DXDevice->QueryInterface(IID_PPV_ARGS(&DX_Debug)));
			DX_Debug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL|D3D11_RLDO_IGNORE_INTERNAL);
			DX_Debug.SafeRelease();
		#else
			LV_UNUSED_VAR(DXDevice);
		#endif // LV_CONFIG_DEBUG()
		}

		int CompileShader(LPCWSTR SourceFileName, LPCSTR EntryPointFunction, LPCSTR Profile, ID3DBlob** ShaderBlob, const D3D_SHADER_MACRO* pOptDefines)
		{
			if (SourceFileName == nullptr || EntryPointFunction == nullptr || Profile == nullptr || ShaderBlob == nullptr)
			{
				return E_INVALIDARG;
			}

			*ShaderBlob = nullptr;

			UINT CompileFlags = D3DCOMPILE_ENABLE_STRICTNESS|D3DCOMPILE_IEEE_STRICTNESS|D3DCOMPILE_PACK_MATRIX_ROW_MAJOR;
		#if LV_CONFIG_DEBUG()
			CompileFlags |= D3DCOMPILE_WARNINGS_ARE_ERRORS|D3DCOMPILE_DEBUG|D3DCOMPILE_SKIP_OPTIMIZATION;
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
	}
}
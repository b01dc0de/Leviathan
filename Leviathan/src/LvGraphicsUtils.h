#ifndef LVGRAPHICSUTILS_H
#define LVGRAPHICSUTILS_H

#include "LvGraphicsTypes.h"
#include "LvGraphics.h"

namespace Leviathan
{
	#define DXCHECK(DXResult) Assert(!FAILED((DXResult)))
	#define DXSAFERELEASE(DXPtr) if (DXPtr) {DXPtr->Release(); DXPtr = nullptr;}

	namespace LvGraphicsUtils
	{
		struct OwnedDxRefs
		{
			static ID3D11Device* DxDeviceRef;
			static LvArray<IUnknown*> DxRefsList;

			static void Add(IUnknown* NewRef);
			static void ReleaseList();
			static void ReleaseDevice();
		};

		template <typename DxType>
		struct DxHandle
		{
			DxType* Ptr = nullptr;

			operator DxType*() { return Ptr; }
			DxType* operator ->() { return Ptr; }
			DxType* operator *() { return Ptr; }
			DxType** operator &() { return &Ptr; }

			void SafeRelease() { DXSAFERELEASE(Ptr); }
			DxHandle(DxType* InPtr) : Ptr(InPtr) { if (nullptr != Ptr) OwnedDxRefs::Add(Ptr); }
			~DxHandle()
			{ 
				// CKA_NOTE: We don't want to SafeRelase here so we can have full control of our COM references
				// - Instead for now just assert that the handle has been cleared
				Assert(nullptr == Ptr);
				//SafeRelease();
			}
		};

		D3D11_BUFFER_DESC GetDefaultBufferDesc(unsigned int SizeBytes);
		void CreateWVPBuffers(ID3D11Device* DXDevice, ID3D11Buffer** Out_WBuffer, ID3D11Buffer** Out_VPBuffer);
		void CreateDefaultSwapChainRenderTarget
		(
			ID3D11Device* DXDevice,
			HWND WindowHandle,
			IDXGISwapChain1** Out_SwapChain1,
			ID3D11Texture2D** Out_BackBuffer,
			ID3D11Texture2D** Out_RenderTargetTexture,
			ID3D11RenderTargetView** Out_RenderTargetView
		);
		void CreateDefaultDepthStencilState
		(
			ID3D11Device* DXDevice,
			ID3D11DepthStencilState** Out_DepthStencilState,
			ID3D11Texture2D** Out_DepthStencilTexture,
			ID3D11DepthStencilView** Out_DepthStencilView
		);
		void CreateDefaultRasterizerBlendState
		(
			ID3D11Device* DXDevice,
			ID3D11RasterizerState** Out_RasterizerState,
			ID3D11BlendState** Out_BlendState
		);
		void ReportLiveObjects(ID3D11Device* DXDevice);

		int CompileShader(LPCWSTR SourceFileName, LPCSTR EntryPointFunction, LPCSTR Profile, ID3DBlob** ShaderBlob, const D3D_SHADER_MACRO* pOptDefines = nullptr);
		template <typename VxType>
		void CompileShaderPipeline_Defaults
		(
			ID3D11Device* DXDevice,
			LPCWSTR SourceFileName,
			const D3D_SHADER_MACRO* pOptDefines,
			ID3D11VertexShader** Out_VertexShader,
			ID3D11PixelShader** Out_PixelShader,
			ID3D11InputLayout** Out_InputLayout
		)
		{
			static const char* Def_VSEntryPoint = "VSMain";
			static const char* Def_PSEntryPoint = "PSMain";
			static const char* Def_VSShaderModel = "vs_5_0";
			static const char* Def_PSShaderModel = "ps_5_0";
			
			ID3D11VertexShader* VertexShader = nullptr;
			ID3D11PixelShader* PixelShader = nullptr;
			ID3D11InputLayout* InputLayout = nullptr;

			ID3DBlob* VSCodeBlob = nullptr;
			ID3DBlob* PSCodeBlob = nullptr;
			DXCHECK(CompileShader(SourceFileName, Def_VSEntryPoint, Def_VSShaderModel, &VSCodeBlob, pOptDefines));
			DXCHECK(CompileShader(SourceFileName, Def_PSEntryPoint, Def_PSShaderModel, &PSCodeBlob, pOptDefines));

			Assert(VSCodeBlob && PSCodeBlob);
			DXCHECK(DXDevice->CreateVertexShader
			(
				VSCodeBlob->GetBufferPointer(),
				VSCodeBlob->GetBufferSize(),
				nullptr,
				&VertexShader
			));
			DXCHECK(DXDevice->CreatePixelShader
			(
				PSCodeBlob->GetBufferPointer(),
				PSCodeBlob->GetBufferSize(),
				nullptr,
				&PixelShader
			));
			DXCHECK(DXDevice->CreateInputLayout
			(
				VxTypeDef<VxType>::InputDescDef,
				VxTypeDef<VxType>::NumInputElements,
				VSCodeBlob->GetBufferPointer(),
				VSCodeBlob->GetBufferSize(),
				&InputLayout
			));
			VSCodeBlob->Release();
			PSCodeBlob->Release();

			Assert(VertexShader && PixelShader && InputLayout);
			*Out_VertexShader = VertexShader;
			*Out_PixelShader = PixelShader;
			*Out_InputLayout = InputLayout;
		}

		template <typename MeshType>
		void CreateVxIxBuffers
		(
			ID3D11Device* DXDevice,
			MeshType* InMesh,
			ID3D11Buffer** Out_VxBuffer,
			ID3D11Buffer** Out_IxBuffer
		)
		{
			ID3D11Buffer* VxBuffer = nullptr;
			ID3D11Buffer* IxBuffer = nullptr;

			D3D11_BUFFER_DESC VxBufferDesc = GetDefaultBufferDesc(InMesh->SzVxDataBytes());
			D3D11_SUBRESOURCE_DATA VxBufferInitData = { InMesh->VxData, 0, 0 };
			DXCHECK(DXDevice->CreateBuffer(&VxBufferDesc, &VxBufferInitData, &VxBuffer));

			D3D11_BUFFER_DESC IxBufferDesc = GetDefaultBufferDesc(InMesh->SzIxDataBytes());
			D3D11_SUBRESOURCE_DATA IxBufferInitData = { InMesh->IxData, 0, 0 };
			DXCHECK(DXDevice->CreateBuffer(&IxBufferDesc, &IxBufferInitData, &IxBuffer));

			Assert(VxBuffer && IxBuffer);
			*Out_VxBuffer = VxBuffer;
			*Out_IxBuffer = IxBuffer;
		}
	}

	template <typename T>
	using DXHandle = LvGraphicsUtils::DxHandle<T>;
}

#endif // LVGRAPHICSUTILS_H
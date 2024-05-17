#ifndef LVGRAPHICSUTILS_H
#define LVGRAPHICSUTILS_H

#include "LvGraphics.h"

namespace Leviathan
{
	#define DXCHECK(DXResult) Assert(!FAILED((DXResult)))
	#define DXSAFERELEASE(DXPtr) if (DXPtr) {DXPtr->Release(); DXPtr = nullptr;}

	namespace LvGraphicsUtils
	{
		template <typename DxType>
		struct DxHandle
		{
			DxType* Ptr = nullptr;

			operator DxType*() { return Ptr; }
			DxType* operator ->() { return Ptr; }
			DxType* operator *() { return Ptr; }
			DxType** operator &() { return &Ptr; }

			void SafeRelease() { DXSAFERELEASE(Ptr); }
			DxHandle(DxType* InPtr) : Ptr(InPtr) { }
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
			ID3D11RasterizerState** Out_DefaultRasterizerState,
			ID3D11BlendState** Out_DefaultBlendState
		);
		void ReportLiveObjects(ID3D11Device* DXDevice);

		int CompileShader(LPCWSTR SourceFileName, LPCSTR EntryPointFunction, LPCSTR Profile, ID3DBlob** ShaderBlob, const D3D_SHADER_MACRO* pOptDefines = nullptr);
		void CompileShaderPipeline_Defaults
		(
			ID3D11Device* DXDevice,
			LPCWSTR SourceFileName,
			LvVxType VxType,
			const D3D_SHADER_MACRO* pOptDefines,
			ID3D11VertexShader** Out_VertexShader,
			ID3D11PixelShader** Out_PixelShader,
			ID3D11InputLayout** Out_InputLayout
		);

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
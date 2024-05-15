#include "LvCommon.h"

#include "LvGraphics.h"
#include "LvGraphicsUtils.h"

namespace Leviathan
{
	namespace LvGraphicsUtils
	{
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

		void ReportLiveObjects(ID3D11Device* DXDevice)
		{
		#if LV_CONFIG_DEBUG()
			DXHandle<ID3D11Debug> DX_Debug = nullptr;
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
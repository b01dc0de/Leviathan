#include "LvCommon.h"

#include "LvGraphicsUtils.h"

namespace Leviathan
{
	namespace LvGraphicsUtils
	{
		int CompileShader(LPCWSTR SourceFileName, LPCSTR EntryPointFunction, LPCSTR Profile, ID3DBlob** ShaderBlob, D3D_SHADER_MACRO* pOptDefines)
		{
			if (SourceFileName == nullptr || EntryPointFunction == nullptr || Profile == nullptr || ShaderBlob == nullptr)
			{
				return E_INVALIDARG;
			}

			*ShaderBlob = nullptr;

			UINT CompileFlags = D3DCOMPILE_ENABLE_STRICTNESS|D3DCOMPILE_IEEE_STRICTNESS|D3DCOMPILE_PACK_MATRIX_ROW_MAJOR;
		#if LV_CONFIG_DEBUG()
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
	}
}
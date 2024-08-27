#include "AtollDrawPipelineState.h"
#include "AtollGraphics.h"

namespace ShaderAtoll
{
	int CompileShaderHelper(LPCWSTR SourceFileName, LPCSTR EntryPointFunction, LPCSTR Profile, const D3D_SHADER_MACRO* MacroDefines, ID3DBlob** ShaderBlob)
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

		Result = D3DCompileFromFile
		(
			SourceFileName,
			MacroDefines,
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

	namespace ShaderCompileDefaults
	{
		LPCSTR VS_Model= "vs_5_0";
		LPCSTR PS_Model = "ps_5_0";
		LPCSTR VS_Main = "VSMain";
		LPCSTR PS_Main = "PSMain";
	}

	int CompileVertexShaderInputLayout_Defaults
	(
		ID3D11Device* InDevice,
		LPCWSTR SrcFilename,
		const D3D_SHADER_MACRO* MacroDefines,
		const D3D11_INPUT_ELEMENT_DESC* InputLayoutDesc,
		uint NumInputElements,
		ID3D11VertexShader** Out_VS,
		ID3D11InputLayout** Out_InputLayout
	)
	{
		ID3DBlob* VSCodeBlob = nullptr;
		int Result = CompileShaderHelper(
			SrcFilename,
			ShaderCompileDefaults::VS_Main,
			ShaderCompileDefaults::VS_Model,
			MacroDefines,
			&VSCodeBlob);
		DXCHECKMSG(VSCodeBlob && Result, "Failed to compile Vertex Shader! :(\n");

		CHECK(InDevice);
		Result = InDevice->CreateVertexShader(VSCodeBlob->GetBufferPointer(), VSCodeBlob->GetBufferSize(), nullptr, Out_VS);
		DXCHECKMSG(Result, "Device could not create vertex shader! :(\n");

		Result = InDevice->CreateInputLayout(InputLayoutDesc, NumInputElements, VSCodeBlob->GetBufferPointer(), VSCodeBlob->GetBufferSize(), Out_InputLayout);
		DXCHECKMSG(Result, "Device could not create input layout! :(\n");

		if (VSCodeBlob) { VSCodeBlob->Release(); }

		return SUCCEEDED(Result);
	}

	int CompilePixelShader_Defaults
	(
		ID3D11Device* InDevice,
		LPCWSTR SrcFilename,
		const D3D_SHADER_MACRO* MacroDefines,
		ID3D11PixelShader** Out_PS
	)
	{
		ID3DBlob* PSCodeBlob = nullptr;
		int Result = Result = CompileShaderHelper(
			SrcFilename,
			ShaderCompileDefaults::PS_Main,
			ShaderCompileDefaults::PS_Model,
			MacroDefines,
			&PSCodeBlob);
		DXCHECKMSG(PSCodeBlob && Result, "Failed to compile Pixel Shader! :(\n");

		CHECK(InDevice);
		Result = InDevice->CreatePixelShader(PSCodeBlob->GetBufferPointer(), PSCodeBlob->GetBufferSize(), nullptr, Out_PS);
		DXCHECKMSG(Result, "Device could not create pixel shader! :(\n");

		if (PSCodeBlob) { PSCodeBlob->Release(); }

		return SUCCEEDED(Result);
	}

	bool CompileDrawPipeline
	(
		ID3D11Device* InDevice,
		LPCWSTR SrcFilename,
		const D3D_SHADER_MACRO* MacroDefines,
		const D3D11_INPUT_ELEMENT_DESC* InputLayoutDesc,
		uint NumInputElements,
		DrawPipelineState* OutDrawPipelineState
	)
	{
		CHECK(InDevice);

		OutDrawPipelineState->VertexShader = nullptr;
		OutDrawPipelineState->PixelShader = nullptr;
		OutDrawPipelineState->InputLayout = nullptr;

		HRESULT Result = CompileVertexShaderInputLayout_Defaults(
			InDevice,
			SrcFilename,
			MacroDefines,
			InputLayoutDesc,
			NumInputElements,
			&OutDrawPipelineState->VertexShader,
			&OutDrawPipelineState->InputLayout
		);
		DXCHECK(Result);

		Result = CompilePixelShader_Defaults(
			InDevice,
			SrcFilename,
			MacroDefines,
			&OutDrawPipelineState->PixelShader);
		DXCHECK(Result);

		return SUCCEEDED(Result);
	}
}
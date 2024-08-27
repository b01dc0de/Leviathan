#ifndef ATOLLDRAWPIPELINESTATE_H
#define ATOLLDRAWPIPELINESTATE_H

#include "AtollCommon.h"

namespace ShaderAtoll
{
	struct DrawPipelineState
	{
		ID3D11VertexShader* VertexShader = nullptr;
		ID3D11PixelShader* PixelShader = nullptr;
		ID3D11InputLayout* InputLayout = nullptr;
	};

	bool CompileDrawPipeline(
		ID3D11Device* InDevice,
		LPCWSTR SrcFilename,
		const D3D_SHADER_MACRO* MacroDefines,
		const D3D11_INPUT_ELEMENT_DESC* InputLayoutDesc,
		uint NumInputElements,
		DrawPipelineState* OutDrawPipelineState
	);
}

#endif // ATOLLGPUPIPELINESTATE_H
#ifndef LVGRAPHICSUTILS_H
#define LVGRAPHICSUTILS_H

#include "LvGraphics.h"

namespace Leviathan
{
	#define DXCHECK(DXResult) Assert(!FAILED((DXResult)))

	namespace LvGraphicsUtils
	{
		int CompileShader(LPCWSTR SourceFileName, LPCSTR EntryPointFunction, LPCSTR Profile, ID3DBlob** ShaderBlob, D3D_SHADER_MACRO* pOptDefines = nullptr);
	}
}

#endif // LVGRAPHICSUTILS_H
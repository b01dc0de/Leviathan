#ifndef LVGRAPHICSTYPES_H
#define LVGRAPHICSTYPES_H

#include "LvGraphics.h"

namespace Leviathan
{
	struct VertexColor
	{
		fVector Position;
		fVector Color;
	};

	struct VertexUV
	{
		fVector Position;
		float fU = 0.0f;
		float fV = 0.0f;
	};

	using IxType = unsigned int;
	struct PrimIx
	{
		IxType V_0;
		IxType V_1;
		IxType V_2;
	};
	using PrimType = PrimIx;

	using InputDescType = D3D11_INPUT_ELEMENT_DESC;
	template <typename VxType>
	struct VxTypeDef
	{
		static const InputDescType* InputDescDef;
		static const int NumInputElements;
	};
}

#endif // LVGRAPHICSTYPES_H
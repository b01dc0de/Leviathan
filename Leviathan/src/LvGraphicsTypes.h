#ifndef LVGRAPHICSTYPES_H
#define LVGRAPHICSTYPES_H

namespace Leviathan
{
	enum struct LvVxType
	{
		LVVX_COLOR,
		LVVX_UV,
		LVVX_NUM,
	};

	struct VertexColor
	{
		static constexpr LvVxType VxType = LvVxType::LVVX_COLOR;
		fVector Position;
		fVector Color;
	};

	struct VertexUV
	{
		static constexpr LvVxType VxType = LvVxType::LVVX_UV;
		fVector Position;
		float fU = 0.0f;
		float fV = 0.0f;
	};

	using VxInputDescType = D3D11_INPUT_ELEMENT_DESC;
	struct LvVxInputDef
	{
		static const LvVxInputDef VxTypeDefTable[(ullong)LvVxType::LVVX_NUM];
		const VxInputDescType* InputDesc;
		const int ElementCount;
	};

	using IxType = uint;
	struct PrimIx
	{
		IxType V_0;
		IxType V_1;
		IxType V_2;
	};
	using PrimType = PrimIx;

	enum struct LvRenderPass
	{
		LV_RENDERPASS_DEFAULT_3D,
		LV_RENDERPASS_DEFAULT_2D,
		LV_RENDERPASS_NUM
	};
}

#endif // LVGRAPHICSTYPES_H
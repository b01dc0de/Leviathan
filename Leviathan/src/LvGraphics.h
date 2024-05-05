#ifndef LVGRAPHICS_H
#define LVGRAPHICS_H

#include "LvCommon.h"

#if PLATFORM_WINDOWS()
	#define DXCHECK(DXResult) Assert(!FAILED((DXResult)))
#else
	#define DXCHECK(DXResult) void(0)
#endif

namespace Leviathan
{
	template <typename T>
	struct DXHandle
	{
		T* Ptr = nullptr;

		DXHandle(T* InPtr) : Ptr(InPtr) {};
		~DXHandle() { if (Ptr) { Ptr->Release(); } }
		operator T*() { return Ptr; }
		T* operator ->() { return Ptr; }
		T* operator *() { return Ptr; }
		T** operator &() { return &Ptr; }
	};

	using R32 = float;

	struct VertexR32
	{
		R32 X = 0.0f;
		R32 Y = 0.0f;
		R32 Z = 0.0f;
		R32 W = 0.0f;
	};

	struct MatrixR32
	{
		VertexR32 Row0;
		VertexR32 Row1;
		VertexR32 Row2;
		VertexR32 Row3;

		void Identity()
		{
			Row0 = { 1.0f, 0.0f, 0.0f, 0.0f };
			Row1 = { 0.0f, 1.0f, 0.0f, 0.0f };
			Row2 = { 0.0f, 0.0f, 1.0f, 0.0f };
			Row3 = { 0.0f, 0.0f, 0.0f, 1.0f };
		}
	};


	struct WorldViewProjData
	{
		MatrixR32 World;
		MatrixR32 View;
		MatrixR32 Proj;
	};

	struct VertexColor
	{
		VertexR32 Position;
		VertexR32 Color;
	};

	void LvInitGraphics();
	void LvUpdateGraphicsState();
	void LvDraw();
}

#endif // LVGRAPHICS_H
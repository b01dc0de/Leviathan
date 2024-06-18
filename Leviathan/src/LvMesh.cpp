#include "LvCommon.h"

#include "LvMesh.h"

namespace Leviathan
{
	const fVector ColorWhite{ 1.0f, 1.0f, 1.0f, 1.0f };
	const fVector ColorBlue{ 0.0f, 0.0f, 1.0f, 1.0f };
	const fVector ColorGreen{ 0.0f, 1.0f, 0.0f, 1.0f };
	const fVector ColorRed{ 1.0f, 0.0f, 0.0f, 1.0f };

	BasicMeshColor* InitCube()
	{
		int NumVerts = 24;
		int NumPrims = 12;
		VertexColor* Verts = new VertexColor[(size_t)NumVerts];
		PrimType* Prims = new PrimType[(size_t)NumPrims];

		IxType vStride = 4;
		IxType tStride = 2;

		// Setting up per-face
		// Forward
		IxType vIdx = 0;
		IxType tIdx = 0;
		Verts[vIdx + 0] = { Vector{ 0.5f, 0.5f, 0.5f }, ColorWhite };
		Verts[vIdx + 1] = { Vector{ -0.5f, 0.5f, 0.5f }, ColorBlue };
		Verts[vIdx + 2] = { Vector{ -0.5f, -0.5f, 0.5f }, ColorGreen };
		Verts[vIdx + 3] = { Vector{ 0.5f, -0.5f, 0.5f }, ColorRed };
		Prims[tIdx + 0] = { vIdx, vIdx + 1, vIdx + 2 };
		Prims[tIdx + 1] = { vIdx, vIdx + 2, vIdx + 3 };

		// Back
		vIdx += vStride;
		tIdx += tStride;
		Verts[vIdx + 0] = { Vector(0.5f, 0.5f, -0.5f), ColorWhite };
		Verts[vIdx + 1] = { Vector(-0.5f, 0.5f, -0.5f), ColorBlue };
		Verts[vIdx + 2] = { Vector(-0.5f, -0.5f, -0.5f), ColorGreen };
		Verts[vIdx + 3] = { Vector(0.5f, -0.5f, -0.5f), ColorRed };
		Prims[tIdx + 0] = { vIdx + 2, vIdx + 1, vIdx };
		Prims[tIdx + 1] = { vIdx + 3, vIdx + 2, vIdx };

		// Left
		vIdx += vStride;
		tIdx += tStride;
		Verts[vIdx + 0] = { Vector(0.5f, 0.5f, -0.5f), ColorWhite };
		Verts[vIdx + 1] = { Vector(0.5f, 0.5f, 0.5f), ColorBlue };
		Verts[vIdx + 2] = { Vector(0.5f, -0.5f, 0.5f), ColorGreen };
		Verts[vIdx + 3] = { Vector(0.5f, -0.5f, -0.5f), ColorRed };
		Prims[tIdx + 0] = { vIdx, vIdx + 1, vIdx + 2 };
		Prims[tIdx + 1] = { vIdx, vIdx + 2, vIdx + 3 };

		// Right
		vIdx += vStride;
		tIdx += tStride;
		Verts[vIdx + 0] = { Vector(-0.5f, 0.5f, 0.5f), ColorWhite };
		Verts[vIdx + 1] = { Vector(-0.5f, 0.5f, -0.5f), ColorBlue };
		Verts[vIdx + 2] = { Vector(-0.5f, -0.5f, -0.5f), ColorGreen };
		Verts[vIdx + 3] = { Vector(-0.5f, -0.5f, 0.5f), ColorRed };
		Prims[tIdx + 0] = { vIdx, vIdx + 1, vIdx + 2 };
		Prims[tIdx + 1] = { vIdx, vIdx + 2, vIdx + 3 };

		// Top
		vIdx += vStride;
		tIdx += tStride;
		Verts[vIdx + 0] = { Vector(0.5f, 0.5f, -0.5f), ColorWhite };
		Verts[vIdx + 1] = { Vector(-0.5f, 0.5f, -0.5f), ColorBlue };
		Verts[vIdx + 2] = { Vector(-0.5f, 0.5f, 0.5f), ColorGreen };
		Verts[vIdx + 3] = { Vector(0.5f, 0.5f, 0.5f), ColorRed };
		Prims[tIdx + 0] = { vIdx, vIdx + 1, vIdx + 2 };
		Prims[tIdx + 1] = { vIdx, vIdx + 2, vIdx + 3 };

		// Bottom
		vIdx += vStride;
		tIdx += tStride;
		Verts[vIdx + 0] = { Vector(0.5f, -0.5f, 0.5f), ColorWhite };
		Verts[vIdx + 1] = { Vector(-0.5f, -0.5f, 0.5f), ColorBlue };
		Verts[vIdx + 2] = { Vector(-0.5f, -0.5f, -0.5f), ColorGreen };
		Verts[vIdx + 3] = { Vector(0.5f, -0.5f, -0.5f), ColorRed };
		Prims[tIdx + 0] = { vIdx, vIdx + 1, vIdx + 2 };
		Prims[tIdx + 1] = { vIdx, vIdx + 2, vIdx + 3 };

		return new BasicMeshColor{ NumVerts, NumPrims, Verts, Prims };
	}

	BasicMeshColor* InitTriangle()
	{
		int NumVerts = 3;
		int NumPrims = 1;
		VertexColor* Verts = new VertexColor[(size_t)NumVerts];
		PrimType* Prims = new PrimType[(size_t)NumPrims];

		Verts[0] = {fVector{0.0f, 0.5f, 0.5f}, ColorRed};
		Verts[1] = {fVector{0.5f, -0.5f, 0.5f}, ColorBlue};
		Verts[2] = {fVector{-0.5f, -0.5f, 0.5f}, ColorGreen};
		Prims[0] = { 0, 2, 1 };

		return new BasicMeshColor{ NumVerts, NumPrims, Verts, Prims };
	}

	BasicMeshUV* InitTextureRect()
	{
		int NumVerts = 4;
		int NumPrims = 2;
		VertexUV* Verts = new VertexUV[(size_t)NumVerts];
		PrimType* Prims = new PrimType[(size_t)NumPrims];

		/*
			CKA_NOTE:
			  2	+------+ 3		Y ^
				|      |		  |
				|      |		X --->
			  0	+------+ 1


			ALSO:
					D + - - - - - + C
					  |         / |
					  |        /  |
					  |       /   |
					  |      /    |
					  |     /     |
					  |    /      |
					  |   /       |
					  |  /        |
					  | /         |
					A + - - - - - + B

				T0 : A -> C -> D
				T1 : A -> B -> C

		*/

		Verts[0] = { fVector{-0.5f, -0.5f, 0.0f}, 0.0f, 1.0f };
		Verts[1] = { fVector{+0.5f, -0.5f, 0.0f}, 1.0f, 1.0f };
		Verts[2] = { fVector{-0.5f, +0.5f, 0.0f}, 0.0f, 0.0f };
		Verts[3] = { fVector{+0.5f, +0.5f, 0.0f}, 1.0f, 0.0f };

		Prims[0] = { 0, 3, 2 };
		Prims[1] = { 0, 1, 3 };

		return new BasicMeshUV{ NumVerts, NumPrims, Verts, Prims };
	}
}

#include "LvCommon.h"

#include "LvMesh.h"

namespace Leviathan
{
	void BasicMeshColor::Init(int InNumVerts, int InNumPrims, VertexColor* InVerts, TriPrim* InPrims)
	{
		NumVerts = InNumVerts;
		NumPrims = InNumPrims;
		Verts = InVerts;
		Prims = InPrims;
	}

	BasicMeshColor::BasicMeshColor(int InNumVerts, int InNumPrims, VertexColor* InVerts, TriPrim* InPrims)
	{
		Init(InNumVerts, InNumPrims, InVerts, InPrims);
	}

	BasicMeshColor::~BasicMeshColor()
	{
		if (Verts) { delete[] Verts; Verts = nullptr; }
		if (Prims) { delete[] Prims; Prims = nullptr; }
	}

	BasicMeshColor::BasicMeshColor(BasicMeshColor&& rValueMesh)
		: NumVerts(rValueMesh.NumVerts)
		, NumPrims(rValueMesh.NumPrims)
		, Verts(rValueMesh.Verts)
		, Prims(rValueMesh.Prims)
	{
		rValueMesh.Verts = nullptr;
		rValueMesh.Prims = nullptr;
	}

	BasicMeshColor& BasicMeshColor::operator=(BasicMeshColor&& rValueMesh)
	{
		NumVerts = rValueMesh.NumVerts;
		NumPrims = rValueMesh.NumPrims;
		Verts = rValueMesh.Verts;
		Prims = rValueMesh.Prims;

		rValueMesh.NumVerts = 0;
		rValueMesh.NumPrims = 0;
		rValueMesh.Verts = nullptr;
		rValueMesh.Prims = nullptr;
		return *this;
	}

	const fVector ColorWhite{ 1.0f, 1.0f, 1.0f, 1.0f };
	const fVector ColorBlue{ 0.0f, 0.0f, 1.0f, 1.0f };
	const fVector ColorGreen{ 0.0f, 1.0f, 0.0f, 1.0f };
	const fVector ColorRed{ 1.0f, 0.0f, 0.0f, 1.0f };

	BasicMeshColor* InitCube()
	{
		int NumVerts = 24;
		int NumPrims = 12;
		VertexColor* Verts = new VertexColor[(size_t)NumVerts];
		TriPrim * Prims = new TriPrim[(size_t)NumPrims];

		using uint = unsigned int;

		uint vStride = 4;
		uint tStride = 2;

		// Setting up per-face
		// Forward
		uint vIdx = 0;
		uint tIdx = 0;
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
		TriPrim * Prims = new TriPrim[(size_t)NumPrims];

		Verts[0] = {fVector{0.0f, 0.5f, 0.5f}, ColorRed};
		Verts[1] = {fVector{0.5f, -0.5f, 0.5f}, ColorBlue};
		Verts[2] = {fVector{-0.5f, -0.5f, 0.5f}, ColorGreen};
		Prims[0] = { 0, 2, 1 };

		return new BasicMeshColor{ NumVerts, NumPrims, Verts, Prims };
	}

	BasicMeshColor* InitRect()
	{
		int NumVerts = 4;
		int NumPrims = 2;
		VertexColor* Verts = new VertexColor[(size_t)NumVerts];
		TriPrim * Prims = new TriPrim[(size_t)NumPrims];

		Verts[0] = {fVector{-0.5f, 0.5f, 0.5f}, ColorWhite};
		Verts[1] = {fVector{0.5f, 0.5f, 0.5f}, ColorRed};
		Verts[2] = {fVector{0.5f, -0.5f, 0.5f}, ColorGreen};
		Verts[3] = {fVector{-0.5f, -0.5f, 0.5f}, ColorBlue};

		Prims[0] = { 0, 2, 1 };
		Prims[1] = { 0, 3, 2 };

		return new BasicMeshColor{ NumVerts, NumPrims, Verts, Prims };
	}
}

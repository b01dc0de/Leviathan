#ifndef LVMESH_H
#define LVMESH_H

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

	struct TriPrim
	{
		unsigned int v0;
		unsigned int v1;
		unsigned int v2;
	};

	struct BasicMeshColor
	{
		int NumVerts = 0;
		int NumPrims = 0;
		VertexColor* Verts = nullptr;
		TriPrim* Prims = nullptr;

		void Init(int InNumVerts, int InNumPrims, VertexColor* InVerts, TriPrim* InPrims);

		BasicMeshColor(int InNumVerts, int InNumPrims, VertexColor* InVerts, TriPrim* InPrims);
		BasicMeshColor() = default;
		~BasicMeshColor();
		BasicMeshColor(BasicMeshColor&& rValueMesh);
		BasicMeshColor& operator=(BasicMeshColor&& rValueMesh);
		BasicMeshColor(const BasicMeshColor&) = delete;
		BasicMeshColor& operator=(const BasicMeshColor&) = delete;
	};

	struct BasicMeshUV
	{
		int NumVerts = 0;
		int NumPrims = 0;
		VertexUV* Verts = nullptr;
		TriPrim* Prims = nullptr;

		void Init(int InNumVerts, int InNumPrims, VertexUV* InVerts, TriPrim* InPrims);

		BasicMeshUV(int InNumVerts, int InNumPrims, VertexUV* InVerts, TriPrim* InPrims);
		BasicMeshUV() = default;
		~BasicMeshUV();
		BasicMeshUV(BasicMeshUV&& rValueMesh);
		BasicMeshUV& operator=(BasicMeshUV&& rValueMesh);
		BasicMeshUV(const BasicMeshUV&) = delete;
		BasicMeshUV& operator=(const BasicMeshUV&) = delete;
	};

	BasicMeshColor* InitCube();
	BasicMeshColor* InitTriangle();
	BasicMeshUV* InitTextureRect();
}

#endif // LVMESH_H
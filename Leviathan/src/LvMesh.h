#ifndef LVMESH_H
#define LVMESH_H

namespace Leviathan
{
	struct VertexColor
	{
		fVector Position;
		fVector Color;
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

	BasicMeshColor* InitCube();
	BasicMeshColor* InitTriangle();
	BasicMeshColor* InitRect();
}

#endif // LVMESH_H
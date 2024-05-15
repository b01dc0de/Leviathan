#ifndef LVMESH_H
#define LVMESH_H

#include "LvGraphicsTypes.h"

namespace Leviathan
{
	template <typename Vx_T>
	struct LvMeshBase
	{
		using VxType = Vx_T;
		int NumVerts = 0;
		int NumPrims = 0;
		VxType* VxData = nullptr;
		PrimType* IxData = nullptr;

		unsigned int SzVxDataBytes() { return sizeof(VxType) * NumVerts; }
		unsigned int SzIxDataBytes() { return sizeof(PrimType) * NumPrims; }
		unsigned int NumIx() { return NumPrims * 3; }
		void Init(int InNumVs, int InNumPs, VxType* InVxData, PrimType* InIxData)
		{
			NumVerts = InNumVs;
			NumPrims = InNumPs;
			VxData = InVxData;
			IxData = InIxData;
		}

		LvMeshBase(int InNumVs, int InNumPs, VxType* InVxData, PrimType* InIxData)
		{
			Init(InNumVs, InNumPs, InVxData, InIxData);
		}
		~LvMeshBase()
		{
			if (VxData) { delete[] VxData; VxData = nullptr; }
			if (IxData) { delete[] IxData; IxData = nullptr; }
		}
		LvMeshBase& operator=(LvMeshBase&& rVMesh)
		{
			Init(rVMesh.NumVerts, rVMesh.NumPrims, rVMesh.VxData, rVMesh.IxData);
			rVMesh.NumVerts = 0;
			rVMesh.NumPrims = 0;
			rVMesh.VxData = nullptr;
			rVMesh.IxData = nullptr;
			return *this;
		}
		LvMeshBase(LvMeshBase&& rVMesh)
		{
			(void)operator=(rVMesh);
		}
	};

	using BasicMeshColor = LvMeshBase<VertexColor>;
	using BasicMeshUV = LvMeshBase<VertexUV>;

	BasicMeshColor* InitCube();
	BasicMeshColor* InitTriangle();
	BasicMeshUV* InitTextureRect();
}

#endif // LVMESH_H
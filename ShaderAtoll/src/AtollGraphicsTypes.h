#ifndef ATOLLGRAPHICSTYPES_H
#define ATOLLGRAPHICSTYPES_H

#include "LvShared.h"

namespace ShaderAtoll
{
	struct VF4
	{
		float X;
		float Y;
		float Z;
		float W;
	};

	struct VertexColor
	{
		VF4 Position;
		VF4 Color;
	};

	struct TriIx
	{
		using IxType = unsigned int;
		IxType Ix0;
		IxType Ix1;
		IxType Ix2;
	};
	constexpr unsigned int TriIxCount = sizeof(TriIx) / sizeof(TriIx::IxType);

	constexpr float msPerFrame_60 = 1000.0f / 60.0f;
	struct ShaderGlobals
	{
		float AppTime_s;
		float DeltaTime_ms;
		int FrameWidth;
		int FrameHeight;
		int MouseX;
		int MouseY;
		int Pad0;
		int Pad1;
	};

	struct DXHandleMgr
	{
		Lv::DynArray<IUnknown**> HandleList;

		void LocalAdd(IUnknown** InHandle);
		void LocalSafeReleaseAll();

		static DXHandleMgr* _Inst;
		static DXHandleMgr* Inst();

		static void Add(IUnknown** InHandle);
		static void SafeReleaseAll();
		static void Term();
	};

	template <typename DX_T>
	struct DXHandle
	{
		DX_T* Handle = nullptr;

		DXHandle(DX_T* InHandle = nullptr)
			: Handle(InHandle)
		{
			DXHandleMgr::Add((IUnknown**)&Handle);
		}
		DXHandle(const DXHandle&) = default;
		DXHandle& operator=(const DXHandle&) = default;
		~DXHandle() = default;
		DXHandle(DXHandle&&) = delete;
		DXHandle& operator=(DXHandle&&) = delete;
		DXHandle& operator=(DX_T* InHandle)
		{
			Handle = InHandle;
			return *this;
		}
		DX_T** operator&() { return &Handle; }
		DX_T* operator->() { return Handle; }
		operator bool() { return nullptr != Handle; }
		operator DX_T*() { return Handle; }
	};
}

#endif // ATOLLGRAPHICSTYPES_H

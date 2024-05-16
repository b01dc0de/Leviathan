#ifndef LVUTILS_INL
#define LVUTILS_INL

namespace Leviathan
{
	struct LvArray_Utils
	{
		using SzType = __int64;

		static constexpr float LvArrDef_IncCoef = 1.5f;
		static constexpr SzType LvArrDef_InitCapMin = 32;
		static constexpr SzType LvArrDef_InitCapBytes = sizeof(SzType) * LvArrDef_InitCapMin;

		static bool IsValidIdx(SzType ItemIdx, SzType NumItems)
		{
			return (0 <= ItemIdx) && (ItemIdx < NumItems);
		}

		template <typename T>
		static bool IsValidInitParams(T* InList, SzType InNumItems, SzType InCapacity)
		{
			return (nullptr != InList) && (0 <= InNumItems) && (InNumItems <= InCapacity);
		}

		static SzType GetIncreaseCapacity(SzType CurrCap)
		{
			return (SzType)(CurrCap * LvArrDef_IncCoef);
		}

		template <typename T>
		static SzType GetInitCapacity()
		{
			SzType NumItems_InitCap = LvArrDef_InitCapBytes / sizeof(T);
			return LvMath::Max<SzType>(LvArrDef_InitCapMin, NumItems_InitCap);
		}

		template <typename T>
		static T* NewArray(SzType NumItems)
		{
			return new T[NumItems];
		}

		template <typename T>
		static void DelArray(T* List)
		{
			delete[] List;
		}

		template <typename T>
		static void CopyArray(T* SrcList, T* DstList, SzType NumItems)
		{
			for (SzType ItemIdx = 0; ItemIdx < NumItems; ItemIdx++)
			{
				DstList[ItemIdx] = SrcList[ItemIdx];
			}
		}

		template <typename T>
		static T* IncCapacity(T* OldList, SzType OldNumItems, SzType NewCapacity)
		{
			T* NewList = NewArray<T>(NewCapacity);
			CopyArray<T>(OldList, NewList, OldNumItems);
			DelArray(OldList);
			return NewList;
		}
	};

	template <typename POD_T> // CKA_NOTE: POD_T should be a POD type
	struct LvArray
	{
		using T = POD_T;
		using SzType = LvArray_Utils::SzType;

		T* List = nullptr;
		SzType NumItems = 0;
		SzType Capacity = 0;

		// CKA_NOTE: This does NOT delete any items, only resets NumItems to 0
		void Empty()
		{
			NumItems = 0;
		}
		void AddItem(const T& NewItem)
		{
			if (!(NumItems + 1 <= Capacity))
			{
				SzType NewCapacity = LvArray_Utils::GetIncreaseCapacity(Capacity);
				List = LvArray_Utils::IncCapacity(List, NumItems, NewCapacity);
			}
			List[NumItems++] = NewItem;
		}
		const T& operator[](SzType ItemIdx) const
		{
			Assert(LvArray_Utils::IsValidIdx(ItemIdx, NumItems));
			return List[ItemIdx];
		}
		T& operator[](SzType ItemIdx)
		{
			Assert(LvArray_Utils::IsValidIdx(ItemIdx, NumItems));
			return List[ItemIdx];
		}
		void Init(T* InList, SzType InNumItems, SzType InCapacity)
		{
			Assert(LvArray_Utils::IsValidInitParams(InList, InNumItems, InCapacity));
			List = InList;
			NumItems = InNumItems;
			Capacity = InCapacity;
		}
		LvArray(SzType InitCap = LvArray_Utils::GetInitCapacity<T>())
		{
			Init(LvArray_Utils::NewArray<T>(InitCap), 0, InitCap);
		}
		~LvArray()
		{
			if (List) { LvArray_Utils::DelArray(List); }
		}
		LvArray(LvArray&& RVal_Array)
		{
			(void)operator=(RVal_Array);
		}
		LvArray& operator=(LvArray&& RVal_Array)
		{
			Init(RVal_Array.List, RVal_Array.NumItems, RVal_Array.Capacity);
			RVal_Array.List = nullptr;
			RVal_Array.NumItems = 0;
			RVal_Array.Capacity = 0;
			return *this;
		}
		LvArray(const LvArray&) = delete;
		LvArray& operator=(const LvArray&) = delete;
	};
}

#endif // LVUTILS_INL
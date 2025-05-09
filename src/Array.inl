#ifndef LEVIATHAN_ARRAY_INL
#define LEVIATHAN_ARRAY_INL

namespace Leviathan
{

constexpr float Array_IncRate = 2.0f;
constexpr int Array_DefaultInitSize = 16;

template <typename T>
struct Array
{
    T* Data = nullptr;
    int Num = 0;
    int Capacity = 0;

    void Init(int InitCapacity)
    {
        ASSERT(Data == nullptr);

        Data = new T[InitCapacity];
        Num = 0;
        Capacity = InitCapacity;
    }
    void Term()
    {
        if (Data)
        {
            delete[] Data;
            Data = nullptr;
        }
        Num = 0;
        Capacity = 0;
    }
    void Empty()
    {
        ASSERT(Data);
        ASSERT(Capacity > 0);
        Num = 0;
    }
    void Inc(int MinCapacity)
    {
        ASSERT(MinCapacity > Capacity);
        int OldCapacity = Capacity;
        T* OldData = Data;

        while (Capacity < MinCapacity) { Capacity = (int)(Capacity * Array_IncRate); }
        Data = new T[Capacity];
        for (int ItemIdx = 0; ItemIdx < Num; ItemIdx++)
        {
            Data[ItemIdx] = OldData[ItemIdx];
        }

        delete[] OldData;
    }
    void Reserve(int NewCapacity)
    {
        ASSERT(NewCapacity > Capacity);

        T* OldData = Data;
        int OldCapacity = Capacity;

        Data = new T[NewCapacity];
        Capacity = NewCapacity;

        for (int Idx = 0; Idx < Num; Idx++)
        {
            Data[Idx] = OldData[Idx];
        }

        delete[] OldData;
    }
    bool CheckIdx(int Idx)
    {
        bool bValidIdx = (0 <= Idx) && (Idx < Num);
        return bValidIdx;
    }
    int Add(const T& Item)
    {
        if (Num == Capacity)
        {
            Inc(Num + 1);
        }
        int ItemIdx = Num;
        Data[Num++] = Item;
        return ItemIdx;
    }
    int Add(const T* Items, int NumItems)
    {
        int NewNum = Num + NumItems;
        if (NewNum > Capacity)
        {
            Inc(NewNum);
        }
        for (int ItemIdx = 0; ItemIdx < NumItems; ItemIdx++)
        {
            Data[Num++] = Items[ItemIdx];
        }
        return Num - 1;
    }
    void Remove(int Idx)
    {
        if (CheckIdx(Idx))
        {
            int RemoveIdx = Idx;
            while (RemoveIdx < Num - 1)
            {
                Data[RemoveIdx] = Data[RemoveIdx + 1];

                RemoveIdx++;
            }
            Num--;
        }
    }

    T& Get(int Idx)
    {
        ASSERT(CheckIdx(Idx));
        return Data[Idx];
    }
    const T& Get(int Idx) const
    {
        ASSERT(CheckIdx(Idx));
        return Data[Idx];
    }
    T& operator[](int Idx)
    {
        return Get(Idx);
    }
    const T& operator[](int Idx) const
    {
        return Get(Idx);
    }
    T& Last()
    {
        ASSERT(Num > 0);
        return Get(Num > 0 ? Num - 1 : 0);
    }

    Array(int InitSize)
    {
        Init(InitSize);
    }
    Array()
    {
        Init(Array_DefaultInitSize);
    }
    ~Array()
    {
        if (Data)
        {
            delete[] Data;
            Data = nullptr;
            Num = 0;
            Capacity = 0;
        }
    }

    Array(const Array& Other) = delete;
    Array& operator=(const Array& Other) = delete;
    Array& operator=(Array&& Other)
    {
        if (Data) { Term(); }

        this->Data = Other.Data;
        this->Num = Other.Num;
        this->Capacity = Other.Capacity;
        Other.Data = nullptr;
        Other.Num = 0;
        Other.Capacity = 0;
        return *this;
    }
    Array(Array&& Other)
    {
        *this = Other;
    }
};

}

#endif // LEVIATHAN_ARRAY_INL


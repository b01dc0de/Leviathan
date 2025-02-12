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

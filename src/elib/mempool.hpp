
#pragma once

#include <array>
#include <cassert>
#include <cstdint>
#include <iterator>
#include <type_traits>

namespace elib
{
    template<const size_t ELEMENT_SIZE, const size_t ALIGNMENT>
    struct AlignedStorage
    {
        template<typename T>
        operator T&()
        {
            T* t = *this;
            return *t;
        }

        template<typename T>
        operator const T&() const
        {
            const T* t = *this;
            return *t;
        }

        template<typename T>
        operator T*()
        {
            return reinterpret_cast<T*>(&data);
        }

        template<typename T>
        operator const T*() const
        {
            return reinterpret_cast<const T*>(&data);
        }

        template<typename T>
        T& get_reference()
        {
            T* t = *this;
            return *t;
        }

        template<typename T>
        const T& get_reference() const
        {
            const T* t = *this;
            return *t;
        }

        template<typename T>
        T* get_address()
        {
            return reinterpret_cast<T*>(&data);
        }

        template<typename T>
        const T* get_address() const
        {
            return reinterpret_cast<const T*>(&data);
        }

        typename std::aligned_storage<ELEMENT_SIZE, ALIGNMENT>::type data;
    };

	
    template<const size_t ELEMENT_SIZE, const size_t ALIGNMENT, const size_t SIZE>
    struct MempoolGeneric;

    template<typename T, const size_t SIZE>
    struct Mempool;

    struct MempoolBase
    {
        using pointer_type = std::uint8_t*;

        template<typename T>
        [[nodiscard]] T* Allocate()
		{
            assert(sizeof(T) <= maximumElementSize);

            return reinterpret_cast<T*>(AllocateItem());
        }

        template<typename T, typename... Args>
        [[nodiscard]] T* Create(Args&&... args)
		{
            auto ptr = Allocate<T>();

            if (ptr != nullptr)
            {
                ::new (ptr) T(std::forward<Args>(args)...);
            }

            return ptr;
        }

        template<typename T>
        void Destroy(const T* const element)
        {
            assert(sizeof(T) <= maximumElementSize);

            const_cast<T*>(element)->~T();

            Release(element);
        }

        template<typename T>
        void Release(const T* const element)
        {
            ReleaseItem(reinterpret_cast<pointer_type>(const_cast<void*>(element)));
        }

        void ReleaseAll()
		{
            elementsAllocated = 0;
            elementsInitialised = 0;
            bufferNext = bufferStart;
		}

        [[nodiscard]] size_t MaxSize() const
        {
            return maximumElementCount;
        }

        template<typename T>
        [[nodiscard]] bool IsItemInPool(const T* const element)
		{
            return IsItemInPool(reinterpret_cast<pointer_type>(const_cast<T*>(element)));
        }

        [[nodiscard]] size_t Available() const
        {
            return maximumElementCount - elementsAllocated;
        }

        [[nodiscard]] size_t Size() const
		{
			return elementsAllocated;
		}

        [[nodiscard]] bool IsEmpty() const
        {
            return elementsAllocated == 0;
        }

        [[nodiscard]] bool IsFull() const
		{
			return elementsAllocated == maximumElementCount;
		}

    protected: 
		MempoolBase(pointer_type buffer, const size_t elementSize, const size_t elementCount)
        : bufferStart(buffer)
        , bufferNext(buffer)
        , maximumElementSize(elementSize)
        , maximumElementCount(elementCount)
        {
        }

    private:
        [[nodiscard]] pointer_type AllocateItem()
        {
            pointer_type value = nullptr;

            if (elementsAllocated < maximumElementCount)
            {
                if (elementsInitialised < maximumElementCount)
                {
                    std::uintptr_t ptr
                    = reinterpret_cast<std::uintptr_t>(bufferStart + (elementsInitialised * maximumElementSize));

                    *reinterpret_cast<uintptr_t*>(ptr) = ptr + maximumElementSize;

                    ++elementsInitialised;
                }

                value = bufferNext;

                ++elementsAllocated;

                if (elementsAllocated < maximumElementCount)
                {
                    bufferNext = *reinterpret_cast<pointer_type*>(bufferNext);
                }
                else
                {
                    bufferNext = nullptr;
                }
            }

            return value;
        }

        void ReleaseItem(pointer_type element)
        {
            assert(IsItemInPool(element));

            if (bufferNext != nullptr)
            {
                *reinterpret_cast<uintptr_t*>(element) = reinterpret_cast<uintptr_t>(bufferNext);
            }
            else
            {
                *reinterpret_cast<uintptr_t*>(element) = 0;
            }

            bufferNext = element;

            --elementsAllocated;
        }

        [[nodiscard]] bool IsItemInPool(pointer_type element)
        {
            auto distance = std::distance(bufferStart, element);

            bool isInRange = (distance >= 0)
                             && (distance <= static_cast<decltype(distance)>(
                                             ((maximumElementSize * maximumElementCount) - maximumElementSize)));

            return isInRange;
        }

        pointer_type bufferStart;
        pointer_type bufferNext;

        size_t elementsAllocated = 0;
        size_t elementsInitialised = 0;

        const size_t maximumElementSize;
        const size_t maximumElementCount;
    };

    template<const size_t ELEMENT_SIZE, const size_t ALIGNMENT, const size_t SIZE>
    struct MempoolGeneric : MempoolBase
    {
        using base = MempoolBase;

        MempoolGeneric()
            : MempoolBase(reinterpret_cast<base::pointer_type>(pool.data()), ELEMENT_SIZE, SIZE)
        {
        }

        template<typename U>
        [[nodiscard]] U* Allocate()
        {
            static_assert(alignof(U) <= ALIGNMENT, "Incompatible alignment");
            static_assert(sizeof(U) <= ELEMENT_SIZE, "Size too large");

            return base::template Allocate<U>();
        }

        template<typename U, typename... Args>
        [[nodiscard]] U* Create(Args&&... args)
        {
            static_assert(alignof(U) <= ALIGNMENT, "Incompatible alignment");
            static_assert(sizeof(U) <= ELEMENT_SIZE, "Size too large");

            return base::template Create<U>(std::forward<Args>(args)...);
        }

        template<typename U>
        void Destroy(const U* const element)
        {
            static_assert(alignof(U) <= ALIGNMENT, "Incompatible alignment");
            static_assert(sizeof(U) <= ELEMENT_SIZE, "Size too large");

			base::template Destroy<U>(element);
		}

    private:
        union Element
        {
            std::uintptr_t next;

            typename std::aligned_storage<ELEMENT_SIZE, ALIGNMENT>::type data;
        };

        std::array<Element, SIZE> pool;
    };

    template<typename T, const size_t SIZE>
    struct Mempool : MempoolGeneric<sizeof(T), std::alignment_of<T>::value, SIZE>
    {
        using base = MempoolGeneric<sizeof(T), std::alignment_of<T>::value, SIZE>;

        template<typename U>
        [[nodiscard]] U* Allocate()
        {
            static_assert(alignof(U) <= alignof(T), "Incompatible alignment");
            static_assert(sizeof(U) <= sizeof(T), "Size too large");

            return base::template Allocate<U>();
        }

        template<typename U, typename... Args>
        [[nodiscard]] U* Create(Args&&... args)
        {
            static_assert(alignof(U) <= alignof(T), "Incompatible alignment");
            static_assert(sizeof(U) <= sizeof(T), "Size too large");

            return base::template Create<U>(std::forward<Args>(args)...);
        }

        template<typename U>
        void Destroy(const U* const element)
        {
            static_assert(alignof(U) <= alignof(T), "Incompatible alignment");
            static_assert(sizeof(U) <= sizeof(T), "Size too large");

            base::template Destroy<U>(element);
        }

        Mempool() = default;
    };
} // namespace elib

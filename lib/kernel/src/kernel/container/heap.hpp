#ifndef CONTAINER_HEAP_HPP
#define CONTAINER_HEAP_HPP

#include "infra/util/variant.hpp"
#include "kernel/getkernel.hpp"
#include "kernel/interruptMasking.hpp"
#include "kernel/scheduler.hpp"
#include "kernel/status.hpp"
#include "kernel/taskList.hpp"
#include <array>
#include <cstdint>
#include <functional>
#include <numeric>

namespace kernel
{
    struct HeapControlBlock
    {
    protected:
        kernel::StatusCode InternalBlock(const kernel::UnblockFunction& unblockFunctionHook);
        void InternalUnblock();

    private:
        kernel::TaskList<> blockList;
    };

    template <class T>

    struct Heap: private HeapControlBlock
    {
        using value_type = infra::Variant<std::size_t, T>;
        using reference = infra::Variant<kernel::StatusCode, std::reference_wrapper<T>>;

        template <std::size_t Size>
        struct WithSize;

        template <size_t Size>
        Heap(std::array<value_type, Size>& buffer);

        reference Claim();
        reference TryClaim();
        void Release(const T&);

    private:
        reference InternalTryClaim();

        value_type* begin;
        value_type* end;

        std::size_t nextFree{};
    };

    template <class TValue, std::size_t Size>
    struct Storage
    {
    protected:
        std::array<TValue, Size> buffer;
    };

    template <class T>
    template <std::size_t Size>
    struct Heap<T>::WithSize
        : Storage<Heap<T>::value_type, Size>
        , Heap<T>
    {
        WithSize()
            : Heap(Storage<Heap<T>::value_type, Size>::buffer)
        {}
    };

    kernel::StatusCode HeapControlBlock::InternalBlock(const kernel::UnblockFunction& unblockFunctionHook)
    {
        auto& scheduler = kernel::GetScheduler();

        return scheduler.Block(blockList, unblockFunctionHook);
    }

    void HeapControlBlock::InternalUnblock()
    {
        auto& scheduler = kernel::GetScheduler();
        auto& task = *blockList.front().task;

        scheduler.Unblock(task);
    }

    template <class T>
    template <size_t Size>
    Heap<T>::Heap(std::array<value_type, Size>& buffer)
        : begin{buffer.begin()}
        , end{buffer.end()}
    {
        std::iota(begin, end, 1u);
    }

    template <class T>
    typename Heap<T>::reference Heap<T>::Claim()
    {
        const kernel::InterruptMasking interruptMasking{};

        auto ret = reference{kernel::StatusCode::Undefined};

        do
        {
            ret = InternalTryClaim();
            if (ret.template Is<std::reference_wrapper<T>>())
            {
                break;
            }

            if (ret == kernel::StatusCode::Busy)
            {
                T* tptr = nullptr;
                ret = InternalBlock([this, &tptr](auto&, kernel::UnblockReason reason) {
                    auto retFromOther = InternalTryClaim();
                    if (retFromOther.template Is<std::reference_wrapper<T>>())
                    {
                        tptr = &(retFromOther.template Get<std::reference_wrapper<T>>().get());
                    }
                });

                if (ret == kernel::StatusCode::Ok)
                {
                    return std::ref(*tptr);
                }
            }
        } while (ret == kernel::StatusCode::Interrupted);

        return ret;
    }

    template <class T>
    typename Heap<T>::reference Heap<T>::TryClaim()
    {
        const kernel::InterruptMasking interruptMasking{};

        return InternalTryClaim();
    }

    template <class T>
    void Heap<T>::Release(const T& item)
    {
        const kernel::InterruptMasking interruptMasking{};

        const auto iter = std::find_if(begin, end, [&item](auto& bufferItem) {
            if (bufferItem.template Is<T>())
            {
                if (&(bufferItem.template Get<T>()) == &item)
                {
                    return true;
                }
            }
            return false;
        });

        if (iter != end)
        {
            auto index = std::distance(begin, iter);
            begin[index] = nextFree;
            nextFree = index;

            InternalUnblock();
        }
    }

    template <class T>
    typename Heap<T>::reference Heap<T>::InternalTryClaim()
    {
        auto size = static_cast<std::size_t>(std::distance(begin, end));
        if (nextFree == size)
        {
            return kernel::StatusCode::Busy;
        }

        const auto currentIndex = nextFree;

        nextFree = begin[nextFree].template Get<std::size_t>();
        begin[currentIndex] = T{};

        return std::ref<T>(begin[currentIndex].template Get<T>());
    }
}

#endif

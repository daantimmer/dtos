#ifndef KERNEL_STACK_HPP
#define KERNEL_STACK_HPP

#include "kernel/port/stack.hpp"
#include <cstddef>
#include <cstdint>
#include <type_traits>

namespace kernel
{
    struct RunnableTask;

    template <std::size_t TStackSize>
    struct StaticStack
    {
        alignas(8) std::uint32_t stack[TStackSize];
    };

    struct Stack
    {
        Stack(std::uint32_t* top, std::size_t size);
        // Stack(const Stack& stack) = delete;
        // Stack(Stack&& stack);

        void Initialize(RunnableTask*);

        bool IsSafe() const;

        std::size_t Size();
        std::size_t Used();
        std::size_t Available();

        void* GetStackPointer() const;
        void SetStackPointer(void*);

    private:
        std::uint32_t* const top;
        std::size_t size;

        void* stackPointer;
        std::uint32_t* edge;
        const std::uint32_t* const stackGuard_begin;
        const std::uint32_t* const stackGuard_end;
    };

    // template <std::size_t TStackSize>
    // struct Stack
    //     : private StackStorage<TStackSize>
    //     , public StackBase
    // {
    //     Stack()
    //         : Storage{}
    //         , Base{reinterpret_cast<std::uint32_t*>(&Storage::stack[0]), sizeof(Storage::stack)}
    //     {}

    // private:
    //     using Storage = StackStorage<TStackSize>;
    //     using Base = StackBase;
    // };
}

#endif

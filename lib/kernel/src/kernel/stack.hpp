#ifndef KERNEL_STACK_HPP
#define KERNEL_STACK_HPP

#include "kernel/port/stack.hpp"
#include <cstddef>
#include <cstdint>

namespace kernel
{
    struct RunnableTask;

    struct Stack
    {
        Stack(std::uint32_t* top, std::size_t size);
        Stack(Stack&& stack);

        bool IsSafe() const;

        std::size_t Size();
        std::size_t Used();
        std::size_t Available();

        std::uint32_t* GetStackPointer() const;
        void SetStackPointer(std::uint32_t*);

    private:
        std::uint32_t* stackPointer;
        std::uint32_t* edge;
        const std::uint32_t* const stackGuard_begin;
        const std::uint32_t* const stackGuard_end;
        const volatile std::uint32_t* const top;
        const volatile std::uint32_t* const end;
    };
}

#endif

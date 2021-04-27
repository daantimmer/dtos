#include "stack.hpp"
#include "kernel/port/stackframeinitialiser.hpp"
#include <algorithm>
#include <utility>

namespace kernel
{
    Stack::Stack(std::uint32_t* stack, std::size_t size)
        : stackPointer{kernel::port::InitialiseStack(stack + 1, size - 2)}
        , edge{nullptr}
        , stackGuard_begin{stack}
        , stackGuard_end{stack + size - 1}
        , top{stackGuard_begin + 1}
        , end{stackGuard_end - 1}
    {
        std::fill(stack, stackPointer, 0xDEADBEEF);
        stack[0] = 0xCCCCCCCC;
        stack[size - 1] = 0xCDCDCDCD;
    }

    Stack::Stack(Stack&& other)
        : stackPointer{std::move(other.stackPointer)}
        , top{std::move(top)}
        , edge{std::move(other.edge)}
        , stackGuard_begin{std::move(other.stackGuard_begin)}
        , stackGuard_end{std::move(other.stackGuard_end)}
        , end{std::move(end)}
    {}

    bool Stack::IsSafe() const
    {
        return true;
    }

    std::size_t Stack::Size()
    {
        return end - top;
    }

    std::size_t Stack::Used()
    {
        return end - edge;
    }

    std::size_t Stack::Available()
    {
        return edge - top;
    }

    std::uint32_t* Stack::GetStackPointer() const
    {
        return stackPointer;
    }

    void Stack::SetStackPointer(std::uint32_t* stackPointer)
    {
        this->stackPointer = stackPointer;
    }
}
#include "stack.hpp"
#include "kernel/port/stackframeinitialiser.hpp"
#include <algorithm>
#include <utility>

namespace kernel
{
    Stack::Stack(std::uint32_t* stack, std::size_t size)
        : top{stack}
        , size{size}
        , stackPointer{nullptr}
        , edge{nullptr}
        , stackGuard_begin{stack}
        , stackGuard_end{stack + size - 1}
    {}

    void Stack::Initialize(RunnableTask* task)
    {
        std::fill(&top[0], &top[size - 1], 0xDEADBEEF);
        top[0] = 0xCCCCCCCC;
        top[size - 1] = 0xCDCDCDCD;

        auto* initialStackPointer = kernel::port::InitialiseStack(top + 1, size - 2);

        stackPointer = initialStackPointer;
        kernel::port::SetEntry(task, initialStackPointer);
    }

    bool Stack::IsSafe() const // NOLINT(readability-convert-member-functions-to-static)
    {
        return true;
    }

    std::size_t Stack::Size() // NOLINT(readability-make-member-function-const)
    {
        return size;
        // return end - top;
    }

    std::size_t Stack::Used() // NOLINT(readability-convert-member-functions-to-static)
    {
        return 0;
        // return end - edge;
    }

    std::size_t Stack::Available() // NOLINT(readability-make-member-function-const)
    {
        return size;
        // return edge - top;
    }

    void* Stack::GetStackPointer() const
    {
        return stackPointer;
    }

    void Stack::SetStackPointer(void* stackPointer)
    {
        this->stackPointer = stackPointer;
    }
}
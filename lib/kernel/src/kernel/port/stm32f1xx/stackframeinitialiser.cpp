#include "kernel/port/stackframeinitialiser.hpp"
#include "kernel/port/stm32f1xx/stackframe.hpp"
#include "kernel/task.hpp"
#include <type_traits>

static void ThreadRunner(kernel::RunnableTask* runnable)
{
    runnable->Run();
}

std::uint32_t* kernel::port::InitialiseStack(void* entry, std::uint32_t* stack, StackSize_t size)
{
    auto const stackSize = type_safe::get(size);
    // Ignore 'do not use pointer arithmetic' warning
    // NOLINTNEXTLINE
    auto* const stackFrame = reinterpret_cast<kernel::port::StackFrame*>(stack + stackSize) - 1;

    constexpr auto castwr = [](auto data) {
        if constexpr (std::is_same_v<std::nullptr_t, decltype(data)>)
        {
            return data;
        }
        else
        {
            return reinterpret_cast<kernel::port::wr>(data);
        }
    };

    *stackFrame = kernel::port::StackFrame{{
                                               castwr(0x44'44'44'44),
                                               castwr(0x55'55'55'55),
                                               castwr(0x66'66'66'66),
                                               castwr(0x77'77'77'77),
                                               castwr(0x88'88'88'88),
                                               castwr(0x99'99'99'99),
                                               castwr(0xaa'aa'aa'aa),
                                               castwr(0xbb'bb'bb'bb),
                                           },
                                           {
                                               castwr(entry),
                                               castwr(0x11111111),
                                               castwr(0x22222222),
                                               castwr(0x33333333),
                                               castwr(0xcccccccc),
                                               castwr(nullptr),
                                               castwr(&ThreadRunner),
                                               castwr(kernel::port::ExceptionStackFrame::defaultXpsr),
                                           }};

    return reinterpret_cast<std::uint32_t*>(stackFrame);
}

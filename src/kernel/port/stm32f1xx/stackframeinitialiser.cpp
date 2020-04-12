#include "kernel/port/stackframeinitialiser.hpp"
#include "kernel/port/stm32f1xx/stackframe.hpp"
#include "kernel/task.hpp"
#include <type_traits>

static void ThreadRunner(RunnableTask* runnable)
{
    runnable->Run();
}

kernel::Status<std::uint32_t*> kernel::port::InitialiseStack(void* entry, std::uint32_t* stack, StackSize_t size)
{
    const auto stackSize = type_safe::get(size);
    const auto stackFrame = reinterpret_cast<kernel::port::StackFrame*>(stack + stackSize) - 1;

    constexpr auto initwr = [](kernel::port::wr& reg, auto data) {
        if constexpr (std::is_same_v<std::nullptr_t, decltype(data)>)
        {
            reg = data;
        }
        else
        {
            reg = reinterpret_cast<kernel::port::wr>(data);
        }
    };

    initwr(stackFrame->softwareStackFrame.r4, 0x44'44'44'44);
    initwr(stackFrame->softwareStackFrame.r5, 0x55'55'55'55);
    initwr(stackFrame->softwareStackFrame.r6, 0x66'66'66'66);
    initwr(stackFrame->softwareStackFrame.r7, 0x77'77'77'77);
    initwr(stackFrame->softwareStackFrame.r8, 0x88'88'88'88);
    initwr(stackFrame->softwareStackFrame.r9, 0x99'99'99'99);
    initwr(stackFrame->softwareStackFrame.r10, 0xaa'aa'aa'aa);
    initwr(stackFrame->softwareStackFrame.r11, 0xbb'bb'bb'bb);

    initwr(stackFrame->exceptionStackFrame.r0, entry);
    initwr(stackFrame->exceptionStackFrame.r1, 0x11111111);
    initwr(stackFrame->exceptionStackFrame.r2, 0x22222222);
    initwr(stackFrame->exceptionStackFrame.r3, 0x33333333);
    initwr(stackFrame->exceptionStackFrame.r12, 0xcccccccc);
    initwr(stackFrame->exceptionStackFrame.lr, nullptr);
    initwr(stackFrame->exceptionStackFrame.pc, &ThreadRunner);
    initwr(stackFrame->exceptionStackFrame.xpsr, kernel::port::ExceptionStackFrame::defaultXpsr);

    return {StatusCode::OK, reinterpret_cast<std::uint32_t*>(stackFrame)};
}

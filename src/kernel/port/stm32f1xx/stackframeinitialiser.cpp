#include "kernel/port/stackframeinitialiser.hpp"

#include "kernel/port/stm32f1xx/stackframe.hpp"
#include "kernel/task.hpp"

static void ThreadRunner(RunnableTask* runnable)
{
    runnable->Run();
}

kernel::Status<std::uint32_t*> kernel::port::InitialiseStack(void* entry, std::uint32_t* stack, std::size_t size)
{
    const auto stackFrame = reinterpret_cast<kernel::port::StackFrame*>(stack + size) - 1;

    stackFrame->softwareStackFrame.r4 = reinterpret_cast<kernel::port::wr>(0x44444444);
    stackFrame->softwareStackFrame.r5 = reinterpret_cast<kernel::port::wr>(0x55555555);
    stackFrame->softwareStackFrame.r6 = reinterpret_cast<kernel::port::wr>(0x66666666);
    stackFrame->softwareStackFrame.r7 = reinterpret_cast<kernel::port::wr>(0x77777777);
    stackFrame->softwareStackFrame.r8 = reinterpret_cast<kernel::port::wr>(0x88888888);
    stackFrame->softwareStackFrame.r9 = reinterpret_cast<kernel::port::wr>(0x99999999);
    stackFrame->softwareStackFrame.r10 = reinterpret_cast<kernel::port::wr>(0xaaaaaaaa);
    stackFrame->softwareStackFrame.r11 = reinterpret_cast<kernel::port::wr>(0xbbbbbbbb);

    stackFrame->exceptionStackFrame.r0 = reinterpret_cast<kernel::port::wr>(entry);
    stackFrame->exceptionStackFrame.r1 = reinterpret_cast<kernel::port::wr>(0x11111111);
    stackFrame->exceptionStackFrame.r2 = reinterpret_cast<kernel::port::wr>(0x22222222);
    stackFrame->exceptionStackFrame.r3 = reinterpret_cast<kernel::port::wr>(0x33333333);
    stackFrame->exceptionStackFrame.r12 = reinterpret_cast<kernel::port::wr>(0xcccccccc);
    stackFrame->exceptionStackFrame.lr = nullptr;
    stackFrame->exceptionStackFrame.pc = reinterpret_cast<kernel::port::wr>(&ThreadRunner);
    stackFrame->exceptionStackFrame.xpsr
    = reinterpret_cast<kernel::port::wr>(kernel::port::ExceptionStackFrame::defaultXpsr);

    stackFrame->exceptionStackFrame.xpsr = reinterpret_cast<kernel::port::wr>(0x01'00'00'00UL);

    return {StatusCode::OK, reinterpret_cast<std::uint32_t*>(stackFrame)};
}

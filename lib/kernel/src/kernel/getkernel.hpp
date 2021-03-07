#ifndef KERNEL_GETKERNEL_HPP
#define KERNEL_GETKERNEL_HPP

namespace kernel
{
    struct Scheduler;

    [[deprecated]] Scheduler& GetKernel();
    Scheduler& GetScheduler();
}

#endif

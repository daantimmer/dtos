#pragma once

namespace kernel
{
    struct MainThread;

    struct Kernel
    {
        Kernel(MainThread&);
    };

    Kernel& GetKernel();
}
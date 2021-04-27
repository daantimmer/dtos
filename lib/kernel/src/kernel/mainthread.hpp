#ifndef KERNEL_MAINTHREAD_HPP
#define KERNEL_MAINTHREAD_HPP

#include "kernel/task.hpp"
#include <cstdint>

namespace kernel
{
    struct MainThread: TaskBase
    {
        MainThread();

        MainThread(const MainThread&) = delete;
        MainThread(MainThread&&) = delete;

        MainThread& operator=(const MainThread&) = delete;
        MainThread& operator=(MainThread&&) = delete;

        void Run() final;

        // bool StackSafe() const final;

        // std::size_t StackAvailable() final;
    };

}

#endif

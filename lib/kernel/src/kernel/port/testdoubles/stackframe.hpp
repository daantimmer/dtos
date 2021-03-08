#pragma once

#include <cstdint>

namespace kernel::port
{
    using wr = void*;

    struct SoftwareStackFrame
    {
        wr r4;
        wr r5;
        wr r6;
        wr r7;
        wr r8;
        wr r9;
        wr r10;
        wr r11;
    };

    struct ExceptionStackFrame
    {
        static constexpr std::uintptr_t defaultXpsr{0x01'00'00'00};

        wr r0;
        wr r1;
        wr r2;
        wr r3;
        wr r12;
        wr lr;
        wr pc;
        wr xpsr;
    };

    struct StackFrame
    {
        SoftwareStackFrame softwareStackFrame;
        ExceptionStackFrame exceptionStackFrame;
    };
}

#ifndef KERNEL_UNBLOCKREASON_HPP
#define KERNEL_UNBLOCKREASON_HPP

#include <cstdint>

namespace kernel
{
    enum class UnblockReason : std::uint8_t
    {
        Undefined,
        Request,
        Timeout
    };
}

#endif

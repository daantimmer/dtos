#ifndef KERNEL_SYSTICKS_T_HPP
#define KERNEL_SYSTICKS_T_HPP

#include "type_safe/strong_typedef.hpp"
#include <cstddef>

namespace kernel
{
    struct SysTicks_t
        : type_safe::strong_typedef<SysTicks_t, std::uint32_t>
        , type_safe::strong_typedef_op::increment<SysTicks_t>
    {
        using type_safe::strong_typedef<SysTicks_t, std::uint32_t>::strong_typedef;
    };
}

#endif

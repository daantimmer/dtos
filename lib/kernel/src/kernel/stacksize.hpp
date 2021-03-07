#pragma once

#include "type_safe/strong_typedef.hpp"
#include <cstddef>

namespace kernel
{
    struct StackSize_t
        : type_safe::strong_typedef<StackSize_t, std::size_t>
        , type_safe::strong_typedef_op::addition<StackSize_t>
        , type_safe::strong_typedef_op::subtraction<StackSize_t>
    {
        using type_safe::strong_typedef<StackSize_t, std::size_t>::strong_typedef;
    };
}
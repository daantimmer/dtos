#pragma once

#include <tuple>

namespace kernel
{
enum struct StatusCode
{
    OK,
    NOK
};

template<class T>
using Status = std::tuple<StatusCode, T>;

}
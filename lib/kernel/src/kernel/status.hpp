#ifndef KERNEL_STATUS_HPP
#define KERNEL_STATUS_HPP

#include <tuple>

namespace kernel
{
    enum struct StatusCode
    {
        Ok,
        Undefined,
        Busy,
        Timeout,
        Interrupted,

    };
    // enum struct StatusCode
    // {
    //     OK,
    //     NOK
    // };

    // template <class T>
    // using Status = std::tuple<StatusCode, T>;

}

#endif

#pragma once

#include <cstdint>

namespace hal
{
template<class T, std::uint32_t Addr>
struct Memory
{
    static constexpr auto Address() -> T*
    {
        return reinterpret_cast<T*>(Addr);
    }

    constexpr auto operator*() const -> T&
    {
        return *Address();
    }

    constexpr auto operator-> () const -> T*
    {
        return Address();
    }
};

}
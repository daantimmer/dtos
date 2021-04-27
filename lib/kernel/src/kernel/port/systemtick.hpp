#pragma once

#include <chrono>
#include <cstdint>
#include <ratio>

namespace systemtick
{
    constexpr uint32_t tickfrequency = 1000;

    using tickratio = std::ratio<1, tickfrequency>;
    using ticks = std::chrono::duration<std::int64_t, tickratio>;

    // void Setup();
}
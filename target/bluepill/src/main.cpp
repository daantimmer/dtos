
#include "main.h"
#include "SEGGER_RTT.h"
#include "hal/datamodel.hpp"
#include "hal/port/peripherals.hpp"
#include "kernel/container/heap.hpp"
#include "kernel/getkernel.hpp"
#include "kernel/lockable.hpp"
#include "kernel/mainthread.hpp"
#include "kernel/port/systemclock.hpp"
#include "kernel/port/systemtick.hpp"
#include "kernel/scheduler.hpp"
#include "kernel/synchronisation/fifo.hpp"
#include "kernel/synchronisation/mutex.hpp"
#include "kernel/synchronisation/semaphore.hpp"
#include "stm32f1xx.h"
#include "stm32f1xx_hal.h"
#include <array>
#include <atomic>
#include <cstdint>

extern "C"
{
    extern TIM_HandleTypeDef htim3;
    extern ADC_HandleTypeDef hadc1;
}

namespace
{
    constexpr std::uint32_t dec_to_bcd(std::uint32_t value)
    {
        std::uint32_t result{0};
        std::uint32_t shift{0};

        while (value > 0)
        {
            const auto remainder = value % 10;
            result |= remainder << (shift << 2u);

            ++shift;
            value /= 10;
        }

        return result;
    }

    constexpr std::uint32_t segDot = 0b1000'0000;
    constexpr std::uint32_t segA = 0b0100'0000;
    constexpr std::uint32_t segB = 0b0010'0000;
    constexpr std::uint32_t segC = 0b0001'0000;
    constexpr std::uint32_t segD = 0b0000'1000;
    constexpr std::uint32_t segE = 0b0000'0100;
    constexpr std::uint32_t segF = 0b0000'0010;
    constexpr std::uint32_t segG = 0b0000'0001;

    constexpr std::uint32_t number_to_7seg(std::uint8_t value)
    {
        constexpr std::array<std::uint32_t, 16> map{{0b0111'1110,
                                                     0b0011'0000,
                                                     0b0110'1101,
                                                     0b0111'1001,
                                                     0b0011'0011,
                                                     0b0101'1011,
                                                     0b0101'1111,
                                                     0b0111'0000,
                                                     0b0111'1111,
                                                     0b0111'1011,
                                                     0b0111'0111,
                                                     0b0001'1111,
                                                     0b0100'1110,
                                                     0b0011'1101,
                                                     0b0100'1111,
                                                     0b0100'0111}};
        return map[value];
    }

    volatile std::uint32_t count = 0;
    auto temperatureValueSemaphore = kernel::Semaphore{};

    std::atomic<bool> doprint{false};

    auto task1Handler = [](const kernel::RunnableTask&, void*) // NOLINT
    {
        while (true)
        {
            SEGGER_RTT_TerminalOut(0, "Task1\n");
            LL_GPIO_TogglePin(LED_BLUE_GPIO_Port, LED_BLUE_Pin);
            DelayTask(std::chrono::milliseconds{1000});
            doprint = true;
        }
    };

    volatile std::uint8_t ping = 0;
    volatile std::uint8_t readIndex = 0;
    std::array<std::array<std::uint16_t, 10>, 2> data;

    auto task2Handler = [](const kernel::RunnableTask&, void* semaphoreVoidPtr) // NOLINT
    {
        auto& semaphore = *static_cast<kernel::Semaphore*>(semaphoreVoidPtr);

        HAL_TIM_Base_Start_IT(&htim3);
        HAL_ADC_Start_DMA(&hadc1, reinterpret_cast<std::uint32_t*>(data[ping].data()), 10);

        std::uint32_t col = 0;
        std::uint16_t adcValue = 0u;

        while (true)
        {
            semaphore.Acquire();

            const auto avg = static_cast<std::uint16_t>(
                std::accumulate(data[readIndex].begin(), data[readIndex].end(), 0) / data[readIndex].size());

            if (std::abs(adcValue - avg) > 2)
            {
                adcValue = avg;
            }
            const auto disp = dec_to_bcd(adcValue);

            for (col = 0; col < 7; ++col)
            {
                {
                    auto mask = COL1_Pin | COL2_Pin | COL3_Pin | COL4_Pin | COL5_Pin | COL6_Pin;
                    auto setValue = mask
                        & ((col == 0 ? COL1_Pin : 0u) | (col == 1 ? COL2_Pin : 0u) | (col == 2 ? COL3_Pin : 0u)
                           | (col == 3 ? COL4_Pin : 0u) | (col == 4 ? COL5_Pin : 0u) | (col == 5 ? COL6_Pin : 0u));
                    auto resValue = mask ^ setValue;

                    LL_GPIO_ResetOutputPin(GPIOA, resValue);
                    LL_GPIO_SetOutputPin(GPIOA, setValue);
                }

                if (col == 6)
                {
                    continue;
                }

                const auto value = (disp >> (4 * (5 - col))) & 0xF;
                const auto segm = number_to_7seg(value);

                {
                    auto mask = SegA_Pin | SegB_Pin | SegC_Pin;
                    auto setValue = mask
                        & (((segm & segA) == segA ? SegA_Pin : 0) | ((segm & segB) == segB ? SegB_Pin : 0)
                           | ((segm & segC) == segC ? SegC_Pin : 0));
                    auto resValue = mask ^ setValue;

                    LL_GPIO_ResetOutputPin(GPIOA, resValue);
                    LL_GPIO_SetOutputPin(GPIOA, setValue);
                }

                {
                    auto mask = SegD_Pin | SegE_Pin | SegF_Pin | SegG_Pin | DOT_Pin;
                    auto setValue = mask
                        & (((segm & segD) == segD ? SegD_Pin : 0) | ((segm & segE) == segE ? SegE_Pin : 0)
                           | ((segm & segF) == segF ? SegF_Pin : 0) | ((segm & segG) == segG ? SegG_Pin : 0)
                           | ((segm & segDot) == segDot ? DOT_Pin : 0)

                        );
                    auto resValue = mask ^ setValue;

                    LL_GPIO_ResetOutputPin(GPIOB, resValue);
                    LL_GPIO_SetOutputPin(GPIOB, setValue);
                }

                DelayTask(std::chrono::milliseconds{5});
            }
        }
    };

    kernel::MainThread mainThread{};

    constexpr auto defaultstacksize = 256U;
    kernel::StaticTask<defaultstacksize> task1{"task1",
                                               +task1Handler,
                                               nullptr}; // NOLINT(cppcoreguidelines-pro-type-cstyle-cast)

    kernel::StaticTask<defaultstacksize> task2{
        "task2",
        +task2Handler,
        &temperatureValueSemaphore}; // NOLINT(cppcoreguidelines-pro-type-cstyle-cast)
}

extern "C" void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* /*hadc*/)
{
    readIndex = ping;
    ping = ping ^ 1;

    HAL_ADC_Start_DMA(&hadc1, reinterpret_cast<std::uint32_t*>(data[ping].data()), 10);
    temperatureValueSemaphore.Release();
}

extern "C" void RealMain()
{
    SEGGER_RTT_Init();

    kernel::Scheduler kernel{mainThread};

    task1.GetTaskControlBlock().Priority(1);
    task2.GetTaskControlBlock().Priority(0);

    kernel::GetScheduler().readyTasksV2.push(task1.GetTaskControlBlock());
    kernel::GetScheduler().readyTasksV2.push(task2.GetTaskControlBlock());

    __enable_irq();

    while (true)
    {
    }
}
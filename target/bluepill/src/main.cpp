
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
    volatile std::uint32_t adcValue = 0;
    auto temperatureValueSemaphore = kernel::Semaphore{};

    std::atomic<bool> doprint{false};

    auto task1Handler = [](const kernel::RunnableTask&, void*) // NOLINT
    {
        while (true)
        {
            SEGGER_RTT_TerminalOut(0, "Task1\n");
            HAL_GPIO_TogglePin(LED_BLUE_GPIO_Port, LED_BLUE_Pin);
            DelayTask(std::chrono::milliseconds{1000});
            doprint = true;
        }
    };

    auto task2Handler = [](const kernel::RunnableTask&, void* semaphoreVoidPtr) // NOLINT
    {
        auto& semaphore = *static_cast<kernel::Semaphore*>(semaphoreVoidPtr);

        HAL_TIM_Base_Start_IT(&htim3);
        HAL_ADC_Start_IT(&hadc1);

        // static /*constexpr */ auto disp = dec_to_bcd(123456u);
        // static auto disp = 0xabcdef;

        std::uint32_t col = 0;

        while (true)
        {
            semaphore.Acquire();

            const auto disp = adcValue;
            const auto value = (disp >> (4 * (5 - col))) & 0xF;
            const auto segm = number_to_7seg(value);

            HAL_GPIO_WritePin(COL1_GPIO_Port,
                              COL1_Pin,
                              col == 0 ? GPIO_PinState::GPIO_PIN_SET : GPIO_PinState::GPIO_PIN_RESET);
            HAL_GPIO_WritePin(COL2_GPIO_Port,
                              COL2_Pin,
                              col == 1 ? GPIO_PinState::GPIO_PIN_SET : GPIO_PinState::GPIO_PIN_RESET);
            HAL_GPIO_WritePin(COL3_GPIO_Port,
                              COL3_Pin,
                              col == 2 ? GPIO_PinState::GPIO_PIN_SET : GPIO_PinState::GPIO_PIN_RESET);
            HAL_GPIO_WritePin(COL4_GPIO_Port,
                              COL4_Pin,
                              col == 3 ? GPIO_PinState::GPIO_PIN_SET : GPIO_PinState::GPIO_PIN_RESET);
            HAL_GPIO_WritePin(COL5_GPIO_Port,
                              COL5_Pin,
                              col == 4 ? GPIO_PinState::GPIO_PIN_SET : GPIO_PinState::GPIO_PIN_RESET);
            HAL_GPIO_WritePin(COL6_GPIO_Port,
                              COL6_Pin,
                              col == 5 ? GPIO_PinState::GPIO_PIN_SET : GPIO_PinState::GPIO_PIN_RESET);

            HAL_GPIO_WritePin(SegA_GPIO_Port,
                              SegA_Pin,
                              segm & segA ? GPIO_PinState::GPIO_PIN_SET : GPIO_PinState::GPIO_PIN_RESET);
            HAL_GPIO_WritePin(SegB_GPIO_Port,
                              SegB_Pin,
                              segm & segB ? GPIO_PinState::GPIO_PIN_SET : GPIO_PinState::GPIO_PIN_RESET);
            HAL_GPIO_WritePin(SegC_GPIO_Port,
                              SegC_Pin,
                              segm & segC ? GPIO_PinState::GPIO_PIN_SET : GPIO_PinState::GPIO_PIN_RESET);
            HAL_GPIO_WritePin(SegD_GPIO_Port,
                              SegD_Pin,
                              segm & segD ? GPIO_PinState::GPIO_PIN_SET : GPIO_PinState::GPIO_PIN_RESET);
            HAL_GPIO_WritePin(SegE_GPIO_Port,
                              SegE_Pin,
                              segm & segE ? GPIO_PinState::GPIO_PIN_SET : GPIO_PinState::GPIO_PIN_RESET);
            HAL_GPIO_WritePin(SegF_GPIO_Port,
                              SegF_Pin,
                              segm & segF ? GPIO_PinState::GPIO_PIN_SET : GPIO_PinState::GPIO_PIN_RESET);
            HAL_GPIO_WritePin(SegG_GPIO_Port,
                              SegG_Pin,
                              segm & segG ? GPIO_PinState::GPIO_PIN_SET : GPIO_PinState::GPIO_PIN_RESET);

            HAL_GPIO_WritePin(DOT_GPIO_Port,
                              DOT_Pin,
                              segm & segDot ? GPIO_PinState::GPIO_PIN_SET : GPIO_PinState::GPIO_PIN_RESET);

            ++col;
            if (col == 6)
            {
                col = 0;
            }

            if (doprint)
            {
                SEGGER_RTT_TerminalOut(0, "Task2\n");
                doprint = false;
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

extern "C" void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
    const auto value = HAL_ADC_GetValue(hadc);
    adcValue = (value + adcValue) / 2;
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

    // hal::Notification::Callback l1cb{[] { SEGGER_RTT_printf(0, "INIT L1\r\n"); }};
    // hal::Notification::Callback l2cb{[] { SEGGER_RTT_printf(0, "INIT L2\r\n"); }};

    // hal::Value<hal::Foo>::Attach(l1cb);
    // hal::Value<hal::Foo>::Attach(l2cb);

    __enable_irq();

    while (true)
    {
    }
}
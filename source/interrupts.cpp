
#include <cstdint>
#include <array>
#include <algorithm>

#include "stm32f103xb.h"
#include "core_cm3.h"
#include "stm32f1xx_ll_gpio.h"

#include "assert.h"

constexpr std::uint32_t MAIN_RETURN{ 0xFFFFFFF9ul };  //Tells the handler to return using the MSP
constexpr std::uint32_t THREAD_RETURN{ 0xFFFFFFFDul }; //Tells the handler to return using the PSP

inline __attribute__((always_inline)) void DisableInterrupts()
{
    uint32_t scratch;

    asm volatile(
        "mov %0, %1\n"
        "msr basepri, %0\n"
        "isb\n"
        "dsb\n"
        : "=r" (scratch) : "i" (1) : "memory");
}

inline __attribute__((always_inline)) void EnableInterrupts(uint32_t maskValue = 0)
{
    asm volatile("msr basepri, %0" :: "r" (maskValue) : "memory");
}

uint32_t criticalNestingCounter = 0;

void EnterCriticalSection()
{
    DisableInterrupts();

    criticalNestingCounter++;
}
void ExitCriticalSection()
{
    if (criticalNestingCounter > 0)
    {
        criticalNestingCounter--;

        if (criticalNestingCounter == 0)
        {
            EnableInterrupts();
        }
    }
    else
    {
        assert(false);
    }
}

static void TaskExitError()
{
    while (1)
    {
        assert(false);
    }
}

struct Task
{
    Task(void(*entry)(), uint32_t* stackTop, size_t stackSize)
        : stackPointer(stackTop + stackSize - 1)
        , stackTop(stackTop + 1)
        , stackSize(stackSize - 2)
    {
        *(this->stackTop - 1) = 0xCCCCCCCC;
        std::fill(this->stackTop, stackPointer, 0xDEADBEEF);
        *(this->stackPointer) = 0xCDCDCDCD;

        stackPointer--;
        *stackPointer = 0x21'00'00'00UL;   /* xPSR */

        stackPointer--;
        *stackPointer = ((uint32_t) entry) & 0xfffffffeUL;	/* PC */

        stackPointer--;
        *stackPointer = (uint32_t) TaskExitError;	/* LR */

        stackPointer -= 5;	/* R12, R3, R2 and R1. */
        *stackPointer = 0x0;	/* R0 */

        stackPointer -= 8;	/* R11, R10, R9, R8, R7, R6, R5 and R4. */
    }

    uint32_t* stackPointer;
    uint32_t* stackTop;
    size_t stackSize;
};

extern "C"
{
    Task * volatile currentTaskControlBlock = nullptr;
}

static std::array<uint32_t, 32+2> stackTask1;
static std::array<uint32_t, 32+2> stackTask2;

void task1Handler()
{
    while (1)
    {
        LL_GPIO_ResetOutputPin(GPIOC, LL_GPIO_PIN_13);
        __WFE();
    }
}

void task2Handler()
{
    while (1)
    {
        LL_GPIO_SetOutputPin(GPIOC, LL_GPIO_PIN_13);
        __WFE();
    }
}

static Task task1{ task1Handler, stackTask1.data(), stackTask1.size() };
static Task task2{ task2Handler, stackTask2.data(), stackTask2.size() };

extern "C"
{
    void TaskScheduler()
    {
        if ((currentTaskControlBlock == nullptr) ||
            (currentTaskControlBlock == &task2))
        {
            currentTaskControlBlock = &task1;
            return;
        }
        else
        {
            currentTaskControlBlock = &task2;
            return;
        }
    }
}

static void scheduler_yield()
{
    SCB->ICSR |= SCB_ICSR_PENDSVSET_Msk;

    asm volatile("dsb" ::: "memory");
    asm volatile("isb");
}

__attribute__((naked)) void startFirstTask()
{
    currentTaskControlBlock = &task1;

    asm volatile(
        "cpsie i        \n"
        "cpsie f        \n"
        "dsb            \n"
        "isb            \n"
        "svc 0          \n"
        "nop            \n"
        "bx lr");
}

extern "C"
{
    void NMI_Handler(){ while (1) { assert(0); } }
    void HardFault_Handler(){ while (1) { assert(0); } }
    void MemManage_Handler(){ while (1) { assert(0); } }
    void BusFault_Handler(){ while (1) { assert(0); } }
    void UsageFault_Handler(){ while (1) { assert(0); } }
    
    void __attribute__((naked)) SVC_Handler()
    {
        asm volatile(
            "ldr r3, currentTaskControlBlockSVC       \n"
            "ldr r1, [r3]                               \n"
            "ldr r0, [r1]                               \n"
            "ldmia r0!, {r4-r11}                        \n"
            "msr psp, r0                                \n"
            "isb                                        \n"
            "orr lr, #0xd                               \n"
            "bx lr                                      \n"
            ".align 4                                   \n"
            "currentTaskControlBlockSVC: .word currentTaskControlBlock");
    }

    void DebugMon_Handler(){ while (1) { assert(0); } }

#define MAX_SYSCALL_INTERRUPT_PRIORITY  1

    void __attribute__((naked)) PendSV_Handler()
    {
        asm volatile(
            "mrs r0, psp\n"
            "isb\n"
            "\n"
            "ldr r3, currentTaskControlBlockPendSV\n"
            "ldr r2, [r3]\n"
            "\n"
            "stmdb r0!, {r4-r11}\n"
            "str r0, [r2]\n"
            "\n"
            "stmdb sp!, {r3, r14}\n"
            "mov r0, %0\n"
            "msr basepri, r0\n"
            "bl TaskScheduler\n"
            "mov r0, #0\n"
            "msr basepri, r0\n"
            "ldmia sp!, {r3, r14}\n"
            "\n"
            "ldr r1, [r3]\n"
            "ldr r0, [r1]\n"
            "ldmia r0!, {r4-r11}\n"
            "msr psp, r0\n"
            "isb\n"
            "bx lr\n"
            "\n"
            ".align 4                                   \n"
            "currentTaskControlBlockPendSV: .word currentTaskControlBlock"
            :: "i" (MAX_SYSCALL_INTERRUPT_PRIORITY));
    }

    void SysTick_Handler()
    {
        DisableInterrupts();

        scheduler_yield();

        EnableInterrupts();
    }

    void WWDG_IRQHandler(){ while (1) { assert(0); } }
    void PVD_IRQHandler(){ while (1) { assert(0); } }
    void TAMPER_IRQHandler(){ while (1) { assert(0); } }
    void RTC_IRQHandler(){ while (1) { assert(0); } }
    void FLASH_IRQHandler(){ while (1) { assert(0); } }
    void RCC_IRQHandler(){ while (1) { assert(0); } }
    void EXTI0_IRQHandler(){ while (1) { assert(0); } }
    void EXTI1_IRQHandler(){ while (1) { assert(0); } }
    void EXTI2_IRQHandler(){ while (1) { assert(0); } }
    void EXTI3_IRQHandler(){ while (1) { assert(0); } }
    void EXTI4_IRQHandler(){ while (1) { assert(0); } }
    void DMA1_Channel1_IRQHandler(){ while (1) { assert(0); } }
    void DMA1_Channel2_IRQHandler(){ while (1) { assert(0); } }
    void DMA1_Channel3_IRQHandler(){ while (1) { assert(0); } }
    void DMA1_Channel4_IRQHandler(){ while (1) { assert(0); } }
    void DMA1_Channel5_IRQHandler(){ while (1) { assert(0); } }
    void DMA1_Channel6_IRQHandler(){ while (1) { assert(0); } }
    void DMA1_Channel7_IRQHandler(){ while (1) { assert(0); } }
    void ADC1_2_IRQHandler(){ while (1) { assert(0); } }
    void USB_HP_CAN1_TX_IRQHandler(){ while (1) { assert(0); } }
    void USB_LP_CAN1_RX0_IRQHandler(){ while (1) { assert(0); } }
    void CAN1_RX1_IRQHandler(){ while (1) { assert(0); } }
    void CAN1_SCE_IRQHandler(){ while (1) { assert(0); } }
    void EXTI9_5_IRQHandler(){ while (1) { assert(0); } }
    void TIM1_BRK_IRQHandler(){ while (1) { assert(0); } }
    void TIM1_UP_IRQHandler(){ while (1) { assert(0); } }
    void TIM1_TRG_COM_IRQHandler(){ while (1) { assert(0); } }
    void TIM1_CC_IRQHandler(){ while (1) { assert(0); } }
    void TIM2_IRQHandler(){ while (1) { assert(0); } }
    void TIM3_IRQHandler(){ while (1) { assert(0); } }
    void TIM4_IRQHandler(){ while (1) { assert(0); } }
    void I2C1_EV_IRQHandler(){ while (1) { assert(0); } }
    void I2C1_ER_IRQHandler(){ while (1) { assert(0); } }
    void I2C2_EV_IRQHandler(){ while (1) { assert(0); } }
    void I2C2_ER_IRQHandler(){ while (1) { assert(0); } }
    void SPI1_IRQHandler(){ while (1) { assert(0); } }
    void SPI2_IRQHandler(){ while (1) { assert(0); } }
    void USART1_IRQHandler(){ while (1) { assert(0); } }
    void USART2_IRQHandler(){ while (1) { assert(0); } }
    void USART3_IRQHandler(){ while (1) { assert(0); } }
    void EXTI15_10_IRQHandler(){ while (1) { assert(0); } }
    void RTCAlarm_IRQHandler(){ while (1) { assert(0); } }
    void USBWakeUp_IRQHandler(){ while (1) { assert(0); } }
}

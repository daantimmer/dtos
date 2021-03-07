/**
 ******************************************************************************
 * @file      startup_stm32f10x_md.c
 * @author    Coocox
 * @version   V1.0
 * @date      12/23/2009
 * @brief     STM32F10x Medium Density Devices Startup code.
 *            This module performs:
 *                - Set the initial SP
 *                - Set the vector table entries with the exceptions ISR address
 *                - Initialize data and bss
 *                - Setup the microcontroller system.
 *                - Call the application's entry point.
 *            After Reset the Cortex-M3 processor is in Thread mode,
 *            priority is Privileged, and the Stack is set to Main.
 *******************************************************************************
 */

#include "stm32f1xx.h"
#include <algorithm>
#include <array>
#include <cassert>
#include <cstddef>
#include <cstdint>

extern int main(); /*!< The entry point for the application.    */

#ifdef IDE
#define __attribute__(param)
#define used
#define weak
#endif

extern "C"
{
    void __libc_init_array(void);

    /*----------Stack Configuration-----------------------------------------------*/
    // #define STACK_SIZE       0x00000400      /*!< The Stack size suggest using even number     */
    // __attribute__((section(".co_stack")))
    // std::uint32_t pulStack[STACK_SIZE];

    /*----------Declaration of the default fault handlers-------------------------*/
    /* System exception vector handler */
    [[noreturn]] __attribute__((used)) void __attribute__((weak)) Reset_Handler(void);
    void __attribute__((weak)) NMI_Handler(void);
    void __attribute__((weak)) HardFault_Handler(void);
    void __attribute__((weak)) MemManage_Handler(void);
    void __attribute__((weak)) BusFault_Handler(void);
    void __attribute__((weak)) UsageFault_Handler(void);
    void __attribute__((weak)) SVC_Handler(void);
    void __attribute__((weak)) DebugMon_Handler(void);
    void __attribute__((weak)) PendSV_Handler(void);
    void __attribute__((weak)) SysTick_Handler(void);
    void __attribute__((weak)) WWDG_IRQHandler(void);
    void __attribute__((weak)) PVD_IRQHandler(void);
    void __attribute__((weak)) TAMPER_IRQHandler(void);
    void __attribute__((weak)) RTC_IRQHandler(void);
    void __attribute__((weak)) FLASH_IRQHandler(void);
    void __attribute__((weak)) RCC_IRQHandler(void);
    void __attribute__((weak)) EXTI0_IRQHandler(void);
    void __attribute__((weak)) EXTI1_IRQHandler(void);
    void __attribute__((weak)) EXTI2_IRQHandler(void);
    void __attribute__((weak)) EXTI3_IRQHandler(void);
    void __attribute__((weak)) EXTI4_IRQHandler(void);
    void __attribute__((weak)) DMA1_Channel1_IRQHandler(void);
    void __attribute__((weak)) DMA1_Channel2_IRQHandler(void);
    void __attribute__((weak)) DMA1_Channel3_IRQHandler(void);
    void __attribute__((weak)) DMA1_Channel4_IRQHandler(void);
    void __attribute__((weak)) DMA1_Channel5_IRQHandler(void);
    void __attribute__((weak)) DMA1_Channel6_IRQHandler(void);
    void __attribute__((weak)) DMA1_Channel7_IRQHandler(void);
    void __attribute__((weak)) ADC1_2_IRQHandler(void);
    void __attribute__((weak)) USB_HP_CAN1_TX_IRQHandler(void);
    void __attribute__((weak)) USB_LP_CAN1_RX0_IRQHandler(void);
    void __attribute__((weak)) CAN1_RX1_IRQHandler(void);
    void __attribute__((weak)) CAN1_SCE_IRQHandler(void);
    void __attribute__((weak)) EXTI9_5_IRQHandler(void);
    void __attribute__((weak)) TIM1_BRK_IRQHandler(void);
    void __attribute__((weak)) TIM1_UP_IRQHandler(void);
    void __attribute__((weak)) TIM1_TRG_COM_IRQHandler(void);
    void __attribute__((weak)) TIM1_CC_IRQHandler(void);
    void __attribute__((weak)) TIM2_IRQHandler(void);
    void __attribute__((weak)) TIM3_IRQHandler(void);
    void __attribute__((weak)) TIM4_IRQHandler(void);
    void __attribute__((weak)) I2C1_EV_IRQHandler(void);
    void __attribute__((weak)) I2C1_ER_IRQHandler(void);
    void __attribute__((weak)) I2C2_EV_IRQHandler(void);
    void __attribute__((weak)) I2C2_ER_IRQHandler(void);
    void __attribute__((weak)) SPI1_IRQHandler(void);
    void __attribute__((weak)) SPI2_IRQHandler(void);
    void __attribute__((weak)) USART1_IRQHandler(void);
    void __attribute__((weak)) USART2_IRQHandler(void);
    void __attribute__((weak)) USART3_IRQHandler(void);
    void __attribute__((weak)) EXTI15_10_IRQHandler(void);
    void __attribute__((weak)) RTCAlarm_IRQHandler(void);
    void __attribute__((weak)) USBWakeUp_IRQHandler(void);

    /*----------Symbols defined in linker script----------------------------------*/
    extern std::uint32_t _sidata; /*!< Start address for the initialization
                                        values of the .data section.            */
    extern std::uint32_t _sdata; /*!< Start address for the .data section     */
    extern std::uint32_t _edata; /*!< End address for the .data section       */
    extern std::uint32_t _sbss; /*!< Start address for the .bss section      */
    extern std::uint32_t _ebss; /*!< End address for the .bss section        */
    // extern void _eram;               /*!< End address for ram                     */

    /*----------Function prototypes-----------------------------------------------*/
    // extern void SystemInit(void);      /*!< Setup the microcontroller system(CMSIS) */
    [[noreturn]] void Default_Reset_Handler(); /*!< Default reset handler                */
    static void Default_Handler(); /*!< Default exception handler            */

    using ExceptionVector = void (*)();

    extern std::uint32_t __processStack_start[];
    extern std::uint32_t __processStack_end[];

    extern const char* __privelegedStack_end[];

    /**
     *@brief The minimal vector table for a Cortex M3.  Note that the proper constructs
     *       must be placed on this to ensure that it ends up at physical address
     *       0x00000000.
     */
    const std::array<ExceptionVector, 67> g_pfnVectors __attribute__((used, section(".isr_vector"), used)){
        /*----------Core Exceptions-------------------------------------------------*/
        // (void*)& pulStack[STACK_SIZE],     /*!< The initial stack pointer         */
        reinterpret_cast<ExceptionVector>(__privelegedStack_end),
        Reset_Handler, /*!< Reset Handler                            */
        NMI_Handler, /*!< NMI Handler                              */
        HardFault_Handler, /*!< Hard Fault Handler                       */
        MemManage_Handler, /*!< MPU Fault Handler                        */
        BusFault_Handler, /*!< Bus Fault Handler                        */
        UsageFault_Handler, /*!< Usage Fault Handler                      */
        0,
        0,
        0,
        0, /*!< Reserved                                 */
        SVC_Handler, /*!< SVCall Handler                           */
        DebugMon_Handler, /*!< Debug Monitor Handler                    */
        0, /*!< Reserved                                 */
        PendSV_Handler, /*!< PendSV Handler                           */
        SysTick_Handler, /*!< SysTick Handler                          */

        /*----------External Exceptions---------------------------------------------*/
        WWDG_IRQHandler, /*!<  0: Window Watchdog                      */
        PVD_IRQHandler, /*!<  1: PVD through EXTI Line detect         */
        TAMPER_IRQHandler, /*!<  2: Tamper                               */
        RTC_IRQHandler, /*!<  3: RTC                                  */
        FLASH_IRQHandler, /*!<  4: Flash                                */
        RCC_IRQHandler, /*!<  5: RCC                                  */
        EXTI0_IRQHandler, /*!<  6: EXTI Line 0                          */
        EXTI1_IRQHandler, /*!<  7: EXTI Line 1                          */
        EXTI2_IRQHandler, /*!<  8: EXTI Line 2                          */
        EXTI3_IRQHandler, /*!<  9: EXTI Line 3                          */
        EXTI4_IRQHandler, /*!< 10: EXTI Line 4                          */
        DMA1_Channel1_IRQHandler, /*!< 11: DMA1 Channel 1                       */
        DMA1_Channel2_IRQHandler, /*!< 12: DMA1 Channel 2                       */
        DMA1_Channel3_IRQHandler, /*!< 13: DMA1 Channel 3                       */
        DMA1_Channel4_IRQHandler, /*!< 14: DMA1 Channel 4                       */
        DMA1_Channel5_IRQHandler, /*!< 15: DMA1 Channel 5                       */
        DMA1_Channel6_IRQHandler, /*!< 16: DMA1 Channel 6                       */
        DMA1_Channel7_IRQHandler, /*!< 17: DMA1 Channel 7                       */
        ADC1_2_IRQHandler, /*!< 18: ADC1 & ADC2                          */
        USB_HP_CAN1_TX_IRQHandler, /*!< 19: USB High Priority or CAN1 TX         */
        USB_LP_CAN1_RX0_IRQHandler, /*!< 20: USB Low  Priority or CAN1 RX0        */
        CAN1_RX1_IRQHandler, /*!< 21: CAN1 RX1                             */
        CAN1_SCE_IRQHandler, /*!< 22: CAN1 SCE                             */
        EXTI9_5_IRQHandler, /*!< 23: EXTI Line 9..5                       */
        TIM1_BRK_IRQHandler, /*!< 24: TIM1 Break                           */
        TIM1_UP_IRQHandler, /*!< 25: TIM1 Update                          */
        TIM1_TRG_COM_IRQHandler, /*!< 26: TIM1 Trigger and Commutation         */
        TIM1_CC_IRQHandler, /*!< 27: TIM1 Capture Compare                 */
        TIM2_IRQHandler, /*!< 28: TIM2                                 */
        TIM3_IRQHandler, /*!< 29: TIM3                                 */
        TIM4_IRQHandler, /*!< 30: TIM4                                 */
        I2C1_EV_IRQHandler, /*!< 31: I2C1 Event                           */
        I2C1_ER_IRQHandler, /*!< 32: I2C1 Error                           */
        I2C2_EV_IRQHandler, /*!< 33: I2C2 Event                           */
        I2C2_ER_IRQHandler, /*!< 34: I2C2 Error                           */
        SPI1_IRQHandler, /*!< 35: SPI1                                 */
        SPI2_IRQHandler, /*!< 36: SPI2                                 */
        USART1_IRQHandler, /*!< 37: USART1                               */
        USART2_IRQHandler, /*!< 38: USART2                               */
        USART3_IRQHandler, /*!< 39: USART3                               */
        EXTI15_10_IRQHandler, /*!< 40: EXTI Line 15..10                     */
        RTCAlarm_IRQHandler, /*!< 41: RTC Alarm through EXTI Line          */
        USBWakeUp_IRQHandler, /*!< 42: USB Wakeup from suspend              */
        0,
        0,
        0,
        0,
        0,
        0,
        0, /*!< Reserved                                 */
        reinterpret_cast<void (*)()>(0xF108F85F),
        // (void*)0xF108F85F            /*!< Boot in RAM mode                         */
    };

    /**
     * @brief  This is the code that gets called when the processor first
     *         starts execution following a reset event. Only the absolutely
     *         necessary set is performed, after which the application
     *         supplied main() routine is called.
     * @param  None
     * @retval None
     */
    [[noreturn]] void Default_Reset_Handler(void)
    {
        std::fill(&__processStack_start[0], &__processStack_end[0], 0xEF'BE'AD'DE);

        asm volatile("ldr r0, =__processStack_end");
        asm volatile("msr psp, r0");
        asm volatile("movs r0, %[consrolSpselMask]" ::[consrolSpselMask] "i"(CONTROL_SPSEL_Msk));
        asm volatile("msr control, r0");
        asm volatile("isb");

        /* Copy the data segment initializers from flash to SRAM */
        std::copy(&_sidata,
                  &_sidata + (&_edata - &_sdata), // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)
                  &_sdata);

        /* Zero fill the bss segment. */
        std::fill(&_sbss, &_ebss, 0);

        __libc_init_array();

        /* Setup the microcontroller system. */
        SystemInit();

        SystemCoreClockUpdate();

        /* Call the application's entry point.*/
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
        main();
#pragma GCC diagnostic pop

        while (true)
        {
        }
    }

/**
 *@brief Provide weak aliases for each Exception handler to the Default_Handler.
 *       As they are weak aliases, any function with the same name will override
 *       this definition.
 */
#pragma weak Reset_Handler = Default_Reset_Handler
#pragma weak NMI_Handler = Default_Handler
#pragma weak HardFault_Handler = Default_Handler
#pragma weak MemManage_Handler = Default_Handler
#pragma weak BusFault_Handler = Default_Handler
#pragma weak UsageFault_Handler = Default_Handler
// #pragma weak SVC_Handler = Default_Handler
#pragma weak DebugMon_Handler = Default_Handler
// #pragma weak PendSV_Handler = Default_Handler
// #pragma weak SysTick_Handler = Default_Handler
#pragma weak WWDG_IRQHandler = Default_Handler
#pragma weak PVD_IRQHandler = Default_Handler
#pragma weak TAMPER_IRQHandler = Default_Handler
#pragma weak RTC_IRQHandler = Default_Handler
#pragma weak FLASH_IRQHandler = Default_Handler
#pragma weak RCC_IRQHandler = Default_Handler
#pragma weak EXTI0_IRQHandler = Default_Handler
#pragma weak EXTI1_IRQHandler = Default_Handler
#pragma weak EXTI2_IRQHandler = Default_Handler
#pragma weak EXTI3_IRQHandler = Default_Handler
#pragma weak EXTI4_IRQHandler = Default_Handler
#pragma weak DMA1_Channel1_IRQHandler = Default_Handler
#pragma weak DMA1_Channel2_IRQHandler = Default_Handler
#pragma weak DMA1_Channel3_IRQHandler = Default_Handler
#pragma weak DMA1_Channel4_IRQHandler = Default_Handler
#pragma weak DMA1_Channel5_IRQHandler = Default_Handler
#pragma weak DMA1_Channel6_IRQHandler = Default_Handler
#pragma weak DMA1_Channel7_IRQHandler = Default_Handler
#pragma weak ADC1_2_IRQHandler = Default_Handler
#pragma weak USB_HP_CAN1_TX_IRQHandler = Default_Handler
#pragma weak USB_LP_CAN1_RX0_IRQHandler = Default_Handler
#pragma weak CAN1_RX1_IRQHandler = Default_Handler
#pragma weak CAN1_SCE_IRQHandler = Default_Handler
#pragma weak EXTI9_5_IRQHandler = Default_Handler
#pragma weak TIM1_BRK_IRQHandler = Default_Handler
#pragma weak TIM1_UP_IRQHandler = Default_Handler
#pragma weak TIM1_TRG_COM_IRQHandler = Default_Handler
#pragma weak TIM1_CC_IRQHandler = Default_Handler
#pragma weak TIM2_IRQHandler = Default_Handler
#pragma weak TIM3_IRQHandler = Default_Handler
#pragma weak TIM4_IRQHandler = Default_Handler
#pragma weak I2C1_EV_IRQHandler = Default_Handler
#pragma weak I2C1_ER_IRQHandler = Default_Handler
#pragma weak I2C2_EV_IRQHandler = Default_Handler
#pragma weak I2C2_ER_IRQHandler = Default_Handler
#pragma weak SPI1_IRQHandler = Default_Handler
#pragma weak SPI2_IRQHandler = Default_Handler
#pragma weak USART1_IRQHandler = Default_Handler
#pragma weak USART2_IRQHandler = Default_Handler
#pragma weak USART3_IRQHandler = Default_Handler
#pragma weak EXTI15_10_IRQHandler = Default_Handler
#pragma weak RTCAlarm_IRQHandler = Default_Handler
#pragma weak USBWakeUp_IRQHandler = Default_Handler

    /**
     * @brief  This is the code that gets called when the processor receives an
     *         unexpected interrupt.  This simply enters an infinite loop,
     *         preserving the system state for examination by a debugger.
     * @param  None
     * @retval None
     */
    static void
        Default_Handler() // NOLINT: Clang does not see this function being used, probably due to the 'weak' attribute
    {
        /* Go into an infinite loop. */
        while (true)
        {
            __BKPT();
        }
    }
}
/*********************** (C) COPYRIGHT 2009 Coocox ************END OF FILE*****/

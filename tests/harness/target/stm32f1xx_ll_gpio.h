#ifndef HARNESS_TARGET_STM32F1XX_LL_GPIO_H
#define HARNESS_TARGET_STM32F1XX_LL_GPIO_H

#include <cstdint>

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct
{
    volatile uint32_t CRL;
    volatile uint32_t CRH;
    volatile uint32_t IDR;
    volatile uint32_t ODR;
    volatile uint32_t BSRR;
    volatile uint32_t BRR;
    volatile uint32_t LCKR;
} GPIO_TypeDef;

#define LL_GPIO_MODE_INPUT 5

void LL_GPIO_SetPinMode(GPIO_TypeDef* GPIOx, uint32_t Pin, uint32_t Mode) noexcept(false);
void LL_GPIO_ResetOutputPin(GPIO_TypeDef* GPIOx, uint32_t PinMask) noexcept(false);
void LL_GPIO_SetOutputPin(GPIO_TypeDef* GPIOx, uint32_t PinMask) noexcept(false);


#ifdef __cplusplus
}
#endif

#endif

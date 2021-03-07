

#cache string "Chip configuration as defined in stm32f1xx.h (default value for the bluepill)"
set(STM32_DEVICE_GROUP STM32F103xB)

add_source(
    PUBLIC
        FW
            src/stm32f1xx_ll_utils.c
            src/stm32f1xx_ll_dac.c
            src/stm32f1xx_ll_dma.c
            src/stm32f1xx_ll_exti.c
            src/stm32f1xx_ll_fsmc.c
            src/stm32f1xx_ll_gpio.c
            src/stm32f1xx_ll_i2c.c
            src/stm32f1xx_ll_pwr.c
            src/stm32f1xx_ll_rcc.c
            src/stm32f1xx_ll_rtc.c
            src/stm32f1xx_ll_sdmmc.c
            src/stm32f1xx_ll_spi.c
            src/stm32f1xx_ll_tim.c
            src/stm32f1xx_ll_usart.c
            src/stm32f1xx_ll_usb.c
            src/stm32f1xx_ll_adc.c
            src/stm32f1xx_ll_crc.c
)

add_source(
    PRIVATE
        SRC
            dummy.cpp
)


target_compile_definitions(stm32f103_arm PUBLIC ${STM32_DEVICE_GROUP})
target_compile_definitions(stm32f103_arm PUBLIC USE_FULL_LL_DRIVER)
target_include_directories(stm32f103_arm
    SYSTEM PUBLIC
        inc
        CMSIS/Device/ST/STM32F1xx/Include
        CMSIS/Include)

add_subdirectory(test)
cmake_minimum_required(VERSION 3.0)

set(STM32_DEVICE_GROUP STM32F103xB
#cache string "Chip configuration as defined in stm32f1xx.h (default value for the bluepill)"
)

add_library(
    ll

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

target_compile_definitions(ll PUBLIC ${STM32_DEVICE_GROUP})
target_compile_definitions(ll PUBLIC USE_FULL_LL_DRIVER)
target_include_directories(ll
	SYSTEM PUBLIC
		inc
		CMSIS/Device/ST/STM32F1xx/Include
		CMSIS/Include)

target_compile_options(ll
	PUBLIC
		-mthumb
		-mtune=cortex-m3
		-mcpu=cortex-m3
		-mlittle-endian
		-specs=nano.specs
		-specs=nosys.specs)
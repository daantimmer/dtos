add_subdirectory(hal/port)

add_source(PUBLIC
    SRC
        "source.cpp"
    
    FW
        "../../arm/src/CMSIS/Device/ST/STM32F1xx/source/Templates/system_stm32f1xx.c"
        "../../arm/src/CMSIS/Device/ST/STM32F1xx/Include/system_stm32f1xx.h"
)

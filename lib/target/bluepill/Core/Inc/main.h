/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define LED_BLUE_Pin GPIO_PIN_13
#define LED_BLUE_GPIO_Port GPIOC
#define COL3_Pin GPIO_PIN_0
#define COL3_GPIO_Port GPIOA
#define COL4_Pin GPIO_PIN_1
#define COL4_GPIO_Port GPIOA
#define COL5_Pin GPIO_PIN_2
#define COL5_GPIO_Port GPIOA
#define COL6_Pin GPIO_PIN_3
#define COL6_GPIO_Port GPIOA
#define COL1_Pin GPIO_PIN_4
#define COL1_GPIO_Port GPIOA
#define COL2_Pin GPIO_PIN_5
#define COL2_GPIO_Port GPIOA
#define BOOT1_Pin GPIO_PIN_2
#define BOOT1_GPIO_Port GPIOB
#define SegA_Pin GPIO_PIN_11
#define SegA_GPIO_Port GPIOA
#define SegB_Pin GPIO_PIN_12
#define SegB_GPIO_Port GPIOA
#define SegC_Pin GPIO_PIN_15
#define SegC_GPIO_Port GPIOA
#define SegD_Pin GPIO_PIN_3
#define SegD_GPIO_Port GPIOB
#define SegE_Pin GPIO_PIN_4
#define SegE_GPIO_Port GPIOB
#define SegF_Pin GPIO_PIN_5
#define SegF_GPIO_Port GPIOB
#define SegG_Pin GPIO_PIN_6
#define SegG_GPIO_Port GPIOB
#define DOT_Pin GPIO_PIN_7
#define DOT_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

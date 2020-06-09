/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
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
#include "stm32f4xx_hal.h"

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
#define LED4_Pin GPIO_PIN_13
#define LED4_GPIO_Port GPIOC
#define LED5_Pin GPIO_PIN_14
#define LED5_GPIO_Port GPIOC
#define A16_Pin GPIO_PIN_0
#define A16_GPIO_Port GPIOC
#define CE__Pin GPIO_PIN_1
#define CE__GPIO_Port GPIOC
#define OE__Pin GPIO_PIN_2
#define OE__GPIO_Port GPIOC
#define WE__Pin GPIO_PIN_3
#define WE__GPIO_Port GPIOC
#define A0_Pin GPIO_PIN_0
#define A0_GPIO_Port GPIOA
#define A1_Pin GPIO_PIN_1
#define A1_GPIO_Port GPIOA
#define A2_Pin GPIO_PIN_2
#define A2_GPIO_Port GPIOA
#define A3_Pin GPIO_PIN_3
#define A3_GPIO_Port GPIOA
#define A4_Pin GPIO_PIN_4
#define A4_GPIO_Port GPIOA
#define A5_Pin GPIO_PIN_5
#define A5_GPIO_Port GPIOA
#define A6_Pin GPIO_PIN_6
#define A6_GPIO_Port GPIOA
#define A7_Pin GPIO_PIN_7
#define A7_GPIO_Port GPIOA
#define CE2_Pin GPIO_PIN_4
#define CE2_GPIO_Port GPIOC
#define SW1_Pin GPIO_PIN_5
#define SW1_GPIO_Port GPIOC
#define DQ0_Pin GPIO_PIN_0
#define DQ0_GPIO_Port GPIOB
#define DQ1_Pin GPIO_PIN_1
#define DQ1_GPIO_Port GPIOB
#define DQ2_Pin GPIO_PIN_2
#define DQ2_GPIO_Port GPIOB
#define SW4_Pin GPIO_PIN_8
#define SW4_GPIO_Port GPIOC
#define SW5_Pin GPIO_PIN_9
#define SW5_GPIO_Port GPIOC
#define A8_Pin GPIO_PIN_8
#define A8_GPIO_Port GPIOA
#define A9_Pin GPIO_PIN_9
#define A9_GPIO_Port GPIOA
#define A10_Pin GPIO_PIN_10
#define A10_GPIO_Port GPIOA
#define A11_Pin GPIO_PIN_11
#define A11_GPIO_Port GPIOA
#define A12_Pin GPIO_PIN_12
#define A12_GPIO_Port GPIOA
#define A15_Pin GPIO_PIN_15
#define A15_GPIO_Port GPIOA
#define LED1_Pin GPIO_PIN_10
#define LED1_GPIO_Port GPIOC
#define LED2_Pin GPIO_PIN_11
#define LED2_GPIO_Port GPIOC
#define LED3_Pin GPIO_PIN_12
#define LED3_GPIO_Port GPIOC
#define DQ3_Pin GPIO_PIN_3
#define DQ3_GPIO_Port GPIOB
#define DQ4_Pin GPIO_PIN_4
#define DQ4_GPIO_Port GPIOB
#define DQ5_Pin GPIO_PIN_5
#define DQ5_GPIO_Port GPIOB
#define DQ6_Pin GPIO_PIN_6
#define DQ6_GPIO_Port GPIOB
#define DQ7_Pin GPIO_PIN_7
#define DQ7_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
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
#include "stm32g4xx_hal.h"

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
#define SEG2_Pin GPIO_PIN_2
#define SEG2_GPIO_Port GPIOE
#define SEG3_Pin GPIO_PIN_3
#define SEG3_GPIO_Port GPIOE
#define SEG4_Pin GPIO_PIN_4
#define SEG4_GPIO_Port GPIOE
#define SW3_Pin GPIO_PIN_13
#define SW3_GPIO_Port GPIOC
#define SEG5_Pin GPIO_PIN_8
#define SEG5_GPIO_Port GPIOE
#define SEG6_Pin GPIO_PIN_9
#define SEG6_GPIO_Port GPIOE
#define SEG7_Pin GPIO_PIN_10
#define SEG7_GPIO_Port GPIOE
#define DIG0_Pin GPIO_PIN_14
#define DIG0_GPIO_Port GPIOD
#define DIG1_Pin GPIO_PIN_15
#define DIG1_GPIO_Port GPIOD
#define DIG2_Pin GPIO_PIN_0
#define DIG2_GPIO_Port GPIOD
#define DIG3_Pin GPIO_PIN_1
#define DIG3_GPIO_Port GPIOD
#define KC0_Pin GPIO_PIN_3
#define KC0_GPIO_Port GPIOD
#define KC1_Pin GPIO_PIN_6
#define KC1_GPIO_Port GPIOD
#define KC2_Pin GPIO_PIN_9
#define KC2_GPIO_Port GPIOB
#define SEG0_Pin GPIO_PIN_0
#define SEG0_GPIO_Port GPIOE
#define SEG1_Pin GPIO_PIN_1
#define SEG1_GPIO_Port GPIOE

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

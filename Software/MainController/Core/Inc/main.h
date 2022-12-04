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
#define PWM_PEROID (CKTIM/(2*PWM_FREQ))
#define CKTIM 168000000
#define PWM_FREQ 15000
#define OLED_CS_Pin GPIO_PIN_13
#define OLED_CS_GPIO_Port GPIOC
#define SPI_SDI_Pin GPIO_PIN_6
#define SPI_SDI_GPIO_Port GPIOA
#define SPI_SDO_Pin GPIO_PIN_7
#define SPI_SDO_GPIO_Port GPIOA
#define M1_CS_Pin GPIO_PIN_12
#define M1_CS_GPIO_Port GPIOB
#define M0_EN_Pin GPIO_PIN_10
#define M0_EN_GPIO_Port GPIOC
#define M1_EN_Pin GPIO_PIN_11
#define M1_EN_GPIO_Port GPIOC
#define M0_CS_Pin GPIO_PIN_12
#define M0_CS_GPIO_Port GPIOC
#define FnLED_Pin GPIO_PIN_2
#define FnLED_GPIO_Port GPIOD
#define SPI_SCK_Pin GPIO_PIN_3
#define SPI_SCK_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

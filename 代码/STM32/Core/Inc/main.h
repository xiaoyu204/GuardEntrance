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
#include "stm32h7xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>

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
#define SG90_PWM_Pin GPIO_PIN_5
#define SG90_PWM_GPIO_Port GPIOE
#define ASRPRO_TX_Pin GPIO_PIN_2
#define ASRPRO_TX_GPIO_Port GPIOA
#define ASRPRO_RX_Pin GPIO_PIN_3
#define ASRPRO_RX_GPIO_Port GPIOA
#define RC522_SCK_Pin GPIO_PIN_5
#define RC522_SCK_GPIO_Port GPIOA
#define RC522_MISO_Pin GPIO_PIN_6
#define RC522_MISO_GPIO_Port GPIOA
#define RC522_MOSI_Pin GPIO_PIN_7
#define RC522_MOSI_GPIO_Port GPIOA
#define RC522_CS_Pin GPIO_PIN_4
#define RC522_CS_GPIO_Port GPIOC
#define RC522_RST_Pin GPIO_PIN_5
#define RC522_RST_GPIO_Port GPIOC
#define LED0_Pin GPIO_PIN_0
#define LED0_GPIO_Port GPIOB
#define AS608_TX_Pin GPIO_PIN_10
#define AS608_TX_GPIO_Port GPIOB
#define AS608_RX_Pin GPIO_PIN_11
#define AS608_RX_GPIO_Port GPIOB
#define TP_RST_Pin GPIO_PIN_6
#define TP_RST_GPIO_Port GPIOH
#define TP_INT_Pin GPIO_PIN_7
#define TP_INT_GPIO_Port GPIOH
#define TRIG_Pin GPIO_PIN_14
#define TRIG_GPIO_Port GPIOB
#define ECHO_Pin GPIO_PIN_15
#define ECHO_GPIO_Port GPIOB
#define TP_SCL_Pin GPIO_PIN_12
#define TP_SCL_GPIO_Port GPIOD
#define TP_SDA_Pin GPIO_PIN_13
#define TP_SDA_GPIO_Port GPIOD
#define BEEP_Pin GPIO_PIN_8
#define BEEP_GPIO_Port GPIOC
#define AS608_TOUCH_Pin GPIO_PIN_9
#define AS608_TOUCH_GPIO_Port GPIOC
#define AS608_TOUCH_EXTI_IRQn EXTI9_5_IRQn
#define SMBUS_SDA_Pin GPIO_PIN_11
#define SMBUS_SDA_GPIO_Port GPIOA
#define SMBUS_SCL_Pin GPIO_PIN_12
#define SMBUS_SCL_GPIO_Port GPIOA
#define BL_EN_Pin GPIO_PIN_5
#define BL_EN_GPIO_Port GPIOB
#define ESP32_TX_Pin GPIO_PIN_6
#define ESP32_TX_GPIO_Port GPIOB
#define ESP32_RX_Pin GPIO_PIN_7
#define ESP32_RX_GPIO_Port GPIOB
#define MAX30102_SCL_Pin GPIO_PIN_8
#define MAX30102_SCL_GPIO_Port GPIOB
#define MAX30102_SDA_Pin GPIO_PIN_9
#define MAX30102_SDA_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

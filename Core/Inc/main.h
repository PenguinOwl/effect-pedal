/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
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

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */
#if defined( __ICCARM__ )
  #define BDMA_BUFFER \
      _Pragma("location=\".bdma_buffer\"")
#else
  #define BDMA_BUFFER \
      __attribute__((section(".bdma_buffer")))
#endif
/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */
void update_text(void);
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define DISPLAY_BL_Pin GPIO_PIN_8
#define DISPLAY_BL_GPIO_Port GPIOF
#define DISPLAY_DC_Pin GPIO_PIN_9
#define DISPLAY_DC_GPIO_Port GPIOF
#define DISPLAY_RST_Pin GPIO_PIN_10
#define DISPLAY_RST_GPIO_Port GPIOF
#define DISPLAY_CS_Pin GPIO_PIN_0
#define DISPLAY_CS_GPIO_Port GPIOC
#define DISPLAY_DATA_Pin GPIO_PIN_1
#define DISPLAY_DATA_GPIO_Port GPIOC
#define EDAC_LRCLK_Pin GPIO_PIN_0
#define EDAC_LRCLK_GPIO_Port GPIOA
#define EDAC_SCK_Pin GPIO_PIN_3
#define EDAC_SCK_GPIO_Port GPIOA
#define EDAC_BCLK_Pin GPIO_PIN_5
#define EDAC_BCLK_GPIO_Port GPIOA
#define EDAC_DOUT_Pin GPIO_PIN_7
#define EDAC_DOUT_GPIO_Port GPIOA
#define EDAC_FMT_Pin GPIO_PIN_2
#define EDAC_FMT_GPIO_Port GPIOB
#define EDAC_XSMT_Pin GPIO_PIN_11
#define EDAC_XSMT_GPIO_Port GPIOF
#define EDAC_FLT_Pin GPIO_PIN_12
#define EDAC_FLT_GPIO_Port GPIOF
#define EDAC_DEMP_Pin GPIO_PIN_13
#define EDAC_DEMP_GPIO_Port GPIOF
#define DISPLAY_SCK_Pin GPIO_PIN_10
#define DISPLAY_SCK_GPIO_Port GPIOB
#define ENC4_B_Pin GPIO_PIN_13
#define ENC4_B_GPIO_Port GPIOB
#define ENC4_R_Pin GPIO_PIN_14
#define ENC4_R_GPIO_Port GPIOB
#define ENC4_L_Pin GPIO_PIN_15
#define ENC4_L_GPIO_Port GPIOB
#define ENC3_B_Pin GPIO_PIN_8
#define ENC3_B_GPIO_Port GPIOD
#define ENC3_R_Pin GPIO_PIN_9
#define ENC3_R_GPIO_Port GPIOD
#define ENC3_L_Pin GPIO_PIN_10
#define ENC3_L_GPIO_Port GPIOD
#define ENC2_B_Pin GPIO_PIN_11
#define ENC2_B_GPIO_Port GPIOD
#define ENC2_R_Pin GPIO_PIN_12
#define ENC2_R_GPIO_Port GPIOD
#define ENC2_L_Pin GPIO_PIN_13
#define ENC2_L_GPIO_Port GPIOD
#define ENC1_B_Pin GPIO_PIN_2
#define ENC1_B_GPIO_Port GPIOG
#define ENC1_R_Pin GPIO_PIN_3
#define ENC1_R_GPIO_Port GPIOG
#define ENC1_L_Pin GPIO_PIN_4
#define ENC1_L_GPIO_Port GPIOG
#define LED_1_Pin GPIO_PIN_5
#define LED_1_GPIO_Port GPIOG
#define LED_2_Pin GPIO_PIN_6
#define LED_2_GPIO_Port GPIOG
#define ARM_MATH_CM4

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

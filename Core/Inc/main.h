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


/* Exported types ------------------------------------------------------------*/


/* Exported constants --------------------------------------------------------*/


/* Exported macro ------------------------------------------------------------*/


/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);


/* Private defines -----------------------------------------------------------*/
#define USER_Btn_Pin 		GPIO_PIN_13
#define USER_Btn_GPIO_Port 	GPIOC
#define MCO_Pin 			GPIO_PIN_0
#define MCO_GPIO_Port 		GPIOH

#define LD1_Pin 			GPIO_PIN_0
#define LD1_GPIO_Port 		GPIOB
#define LD2_Pin 			GPIO_PIN_7
#define LD2_GPIO_Port 		GPIOB
#define LD3_Pin 			GPIO_PIN_14
#define LD3_GPIO_Port 		GPIOB


#define STLK_RX_Pin 				GPIO_PIN_8
#define STLK_RX_GPIO_Port 			GPIOD
#define STLK_TX_Pin 				GPIO_PIN_9
#define STLK_TX_GPIO_Port 			GPIOD
#define USB_PowerSwitchOn_Pin 		GPIO_PIN_6
#define USB_PowerSwitchOn_GPIO_Port GPIOG
#define USB_OverCurrent_Pin 		GPIO_PIN_7
#define USB_OverCurrent_GPIO_Port 	GPIOG
#define TMS_Pin 					GPIO_PIN_13
#define TMS_GPIO_Port 				GPIOA
#define TCK_Pin 					GPIO_PIN_14
#define TCK_GPIO_Port 				GPIOA


/**
 * @brief   ADC_STATE structure.
 *
 * */
typedef enum
{
  ADC_IDLE = 0x00U,
  ADC_BUSY = 0x01U
} ADC_STATE_E;

/**
 * @brief   ADC_STATUS structure.
 *
 * */
typedef struct{
	ADC_STATE_E 	status;
	uint8_t 		active_channel;
}ADC_STATUS_S;

/**
 * @brief   ADC_DMA structure.
 *
 * */
extern const struct ADCdma_s
{
	HAL_StatusTypeDef (*init)(void);
	HAL_StatusTypeDef (*getValue)(uint8_t ch, uint32_t* value, uint32_t len);
	ADC_STATUS_S (*getStatus)(void);
}ADCdma;


#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

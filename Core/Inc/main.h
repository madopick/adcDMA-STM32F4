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
 * @brief   ADC_CH enum.
 *
 * */
typedef enum
{
	ADC_CH0 = ADC_CHANNEL_0,
	ADC_CH1 = ADC_CHANNEL_1,
	ADC_CH2 = ADC_CHANNEL_2,
	ADC_CH3 = ADC_CHANNEL_3,
	ADC_CH4 = ADC_CHANNEL_4,
	ADC_CH5 = ADC_CHANNEL_5,
	ADC_CH6 = ADC_CHANNEL_6,
	ADC_CH7 = ADC_CHANNEL_7,
	ADC_CH8 = ADC_CHANNEL_8,
	ADC_CH9 = ADC_CHANNEL_9,
	ADC_CH10 = ADC_CHANNEL_10,
	ADC_CH11 = ADC_CHANNEL_11,
	ADC_CH12 = ADC_CHANNEL_12,
	ADC_CH13 = ADC_CHANNEL_13,
	ADC_CH14 = ADC_CHANNEL_14,
	ADC_CH15 = ADC_CHANNEL_15,
	ADC_CH16 = ADC_CHANNEL_16,
	ADC_CH17 = ADC_CHANNEL_17,
	ADC_CH18 = ADC_CHANNEL_18,
}ADC_CH_E;


/**
 * @brief   ADC_STATE enum.
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
	uint32_t 		conv_len;
	void    		( *adcDone )( uint32_t size );
}ADC_STATUS_S;

/**
 * @brief   ADC_DMA structure.
 *
 * */
extern const struct ADCdma_s
{
	HAL_StatusTypeDef (*init)(void (*cb)(uint32_t len));
	HAL_StatusTypeDef (*getValue)(ADC_CH_E ch, uint32_t* value, uint32_t len);
	ADC_STATUS_S (*getStatus)(void);
}ADCdma;


#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

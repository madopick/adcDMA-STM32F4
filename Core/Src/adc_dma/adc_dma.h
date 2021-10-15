/*
 * adc_dma.h
 *
 *  Created on: Oct 15, 2021
 *      Author: madop
 */

#ifndef SRC_ADC_DMA_ADC_DMA_H_
#define SRC_ADC_DMA_ADC_DMA_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f4xx_hal.h"

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

#endif /* SRC_ADC_DMA_ADC_DMA_H_ */

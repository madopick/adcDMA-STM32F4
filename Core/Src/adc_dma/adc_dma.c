/*
 * adc_dma.c
 *
 *  Created on: Oct 15, 2021
 *      Author: madop
 */


#include "main.h"

static void STM32_ADC1_Init(void);
static void STM32_DMA_Init(void);


ADC_HandleTypeDef 	hadc1;
DMA_HandleTypeDef 	hdma_adc1;
ADC_STATUS_S 		adcStatus;


/************************************************
  * @name 	: _adc_dma_init
  * @brief	:Initialize ADC with DMA operation
  ***********************************************/
static HAL_StatusTypeDef _adc_dma_init(void (*cb)(uint32_t len))
{
	STM32_DMA_Init();
	STM32_ADC1_Init();
	adcStatus.status 	= ADC_IDLE;

	if(cb != NULL){
		adcStatus.adcDone 	= cb;
	}
	return HAL_OK;
}

/************************************************
  * @name 	: _adc_dma_getValue
  * @brief	:Get value from ADC
  ***********************************************/
static HAL_StatusTypeDef _adc_dma_getValue(ADC_CH_E ch, uint32_t* value, uint32_t len)
{
	adcStatus.active_channel 	= ch;
	adcStatus.status			= ADC_BUSY;
	adcStatus.conv_len			= len;

	ADC_ChannelConfTypeDef adcConf = {0};

	/* configure adc channel */
	adcConf.Channel 			= ch;
	adcConf.Rank				= 1;
	adcConf.SamplingTime 		= ADC_SAMPLETIME_3CYCLES;
	HAL_ADC_ConfigChannel(&hadc1, &adcConf);

	/** Start ADC the conversion process */
	if(HAL_ADC_Start_DMA(&hadc1, (uint32_t*)value, len) != HAL_OK)
	{
	  /* Start Conversion Error */
	  Error_Handler();
	}

	return HAL_OK;
}

/************************************************
  * @name 	: _adc_dma_getStatus
  * @brief	: Get ADC status
  ***********************************************/
static ADC_STATUS_S _adc_dma_getStatus(void)
{
	return adcStatus;
}



///ADC_DMA
const struct ADCdma_s ADCdma =
{
		.init 	 	= _adc_dma_init,
		.getValue	= _adc_dma_getValue,
		.getStatus	= _adc_dma_getStatus
};


/************************************************
  * @name 	: _adc_dma_getStatus
  * @brief	: STM32 ADC Conversion
  ***********************************************/
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
	if(hadc->Instance == ADC1){
		adcStatus.status			= ADC_IDLE;
		if(adcStatus.adcDone != NULL){
			adcStatus.adcDone(adcStatus.conv_len);
		}
	}
}



/************************
 * STM32 Driver
 ************************/

/**
  * @brief STM32 ADC1 Initialization Function
  */
static void STM32_ADC1_Init(void)
{
  ADC_ChannelConfTypeDef sConfig = {0};

  /** Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion)*/
  hadc1.Instance 					= ADC1;
  hadc1.Init.ClockPrescaler 		= ADC_CLOCK_SYNC_PCLK_DIV4;
  hadc1.Init.Resolution 			= ADC_RESOLUTION_12B;
  hadc1.Init.ScanConvMode 			= DISABLE;				/* Sequencer chain conv disabled */
  hadc1.Init.ContinuousConvMode 	= DISABLE;				/* Continuous mode disabled to have only 1 conversion at each conversion trig */
  hadc1.Init.DiscontinuousConvMode 	= DISABLE;				/* Parameter discarded because sequencer is disabled */
  hadc1.Init.ExternalTrigConvEdge 	= ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc1.Init.ExternalTrigConv 		= ADC_SOFTWARE_START;
  hadc1.Init.DataAlign 				= ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion 		= 1;
  hadc1.Init.DMAContinuousRequests 	= DISABLE;
  hadc1.Init.EOCSelection 			= ADC_EOC_SINGLE_CONV;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.*/
  sConfig.Channel 					= ADC_CHANNEL_10;
  sConfig.Rank 						= 1;
  sConfig.SamplingTime 				= ADC_SAMPLETIME_3CYCLES;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

}


/**
  * Enable STm32 DMA controller clock
  */
static void STM32_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA2_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA2_Stream0_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA2_Stream0_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA2_Stream0_IRQn);

}




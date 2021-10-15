/*
 * afe_spi.c
 *
 *  Created on: Oct 15, 2021
 *      Author: madop
 */

#include "main.h"

/* Private functions ---------------------------------------------------------*/
static void STM32_SPI1_Init(void);


/* Private variables ---------------------------------------------------------*/
SPI_HandleTypeDef hspi1;
DMA_HandleTypeDef hdma_spi1_rx;
DMA_HandleTypeDef hdma_spi1_tx;

/************************
 * STM32 Driver
 ************************/

/**
  * @brief STM32 SPI1 Initialization Function
  */
static void STM32_SPI1_Init(void)
{
  /* SPI1 parameter configuration*/
  hspi1.Instance 				= SPI1;
  hspi1.Init.Mode 				= SPI_MODE_MASTER;
  hspi1.Init.Direction 			= SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize 			= SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity 		= SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase 			= SPI_PHASE_1EDGE;
  hspi1.Init.NSS 				= SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler 	= SPI_BAUDRATEPRESCALER_2;
  hspi1.Init.FirstBit 			= SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode 			= SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation 	= SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial 		= 10;

  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }

}

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

static SPI_STATUS_S 		spiStatus;

/************************************************
  * @name 	: _spi_dma_init
  * @brief	:Initialize ADC with DMA operation
  ***********************************************/
static HAL_StatusTypeDef _spi_dma_init(void (*cb)(uint16_t size))
{
	STM32_SPI1_Init();
	spiStatus.status 		= SPI_IDLE;

	if(cb != NULL){
		spiStatus.spiDone 	= cb;
	}
	return HAL_OK;
}


/************************************************
  * @name 	: _afe_spi_TransmitReceive
  * @brief	:TransmitReceive to slave SPI
  ***********************************************/
static HAL_StatusTypeDef _afe_spi_TransmitReceive(uint8_t *pTxData, uint8_t *pRxData, uint16_t Size)
{
	spiStatus.status 		= SPI_BUSY;
	spiStatus.len			= Size;

	if(HAL_SPI_TransmitReceive_DMA(&hspi1, (uint8_t*)pTxData, (uint8_t *)pRxData, Size) != HAL_OK)
	{
		 /* Transfer error in transmission process */
		 Error_Handler();
	}

	return HAL_OK;
}


/************************************************
  * @name 	: _afe_spi_Transmit
  * @brief	:TransmitReceive to slave SPI
  ***********************************************/
static HAL_StatusTypeDef _afe_spi_Transmit(uint8_t *pTxData, uint16_t Size)
{
	spiStatus.status 		= SPI_BUSY;
	spiStatus.len			= Size;

	if(HAL_SPI_Transmit_DMA(&hspi1, (uint8_t*)pTxData, Size) != HAL_OK)
	{
		 /* Transfer error in transmission process */
		 Error_Handler();
	}

	return HAL_OK;
}

/************************************************
  * @name 	: _afe_spi_Receive
  * @brief	:TransmitReceive to slave SPI
  ***********************************************/
static HAL_StatusTypeDef _afe_spi_Receive(uint8_t *pRxData, uint16_t Size)
{
	spiStatus.status 		= SPI_BUSY;
	spiStatus.len			= Size;

	if(HAL_SPI_Receive_DMA(&hspi1, (uint8_t *)pRxData, Size) != HAL_OK)
	{
		 /* Transfer error in transmission process */
		 Error_Handler();
	}

	return HAL_OK;
}


/************************************************
  * @name 	: _afe_spi_getStatus
  * @brief	: Get SPI status
  ***********************************************/
static SPI_STATUS_S _afe_spi_getStatus(void)
{
	return spiStatus;
}


///SPI_DMA
const struct SPIdma_s SPIdma =
{
		.init 	 		= _spi_dma_init,
		.tranRecv		= _afe_spi_TransmitReceive,
		.tranmit		= _afe_spi_Transmit,
		.receive		= _afe_spi_Receive,
		.getStatus		= _afe_spi_getStatus
};


/*************************************************************************************
 * 									STM32 Driver									 *
 *************************************************************************************/

/**
  * @brief  Tx Transfer completed callback.
  * @param  hspi pointer to a SPI_HandleTypeDef structure.
  * @retval None
  */
void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi)
{
	spiStatus.status 		= SPI_IDLE;
}

/**
  * @brief  Rx Transfer completed callback.
  * @param  hspi pointer to a SPI_HandleTypeDef structure.
  * @retval None
  */
void HAL_SPI_RxCpltCallback(SPI_HandleTypeDef *hspi)
{
	spiStatus.status 		= SPI_IDLE;
}


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

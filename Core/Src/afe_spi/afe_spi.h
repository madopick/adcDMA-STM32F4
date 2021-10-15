/*
 * afe_spi.h
 *
 *  Created on: Oct 15, 2021
 *      Author: madop
 */

#ifndef SRC_AFE_SPI_AFE_SPI_H_
#define SRC_AFE_SPI_AFE_SPI_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f4xx_hal.h"

/**
 * @brief   SPI_STATE enum.
 *
 * */
typedef enum
{
  SPI_IDLE = 0x00U,
  SPI_BUSY = 0x01U
} SPI_STATE_E;

/**
 * @brief   SPI_STATUS structure.
 *
 * */
typedef struct{
	SPI_STATE_E 	status;
	uint16_t 		len;
	void    		( *spiDone )( uint16_t size );
}SPI_STATUS_S;

/**
 * @brief   SPI_DMA structure.
 *
 * */
extern const struct SPIdma_s
{
	HAL_StatusTypeDef (*init)(void (*cb)(uint16_t size));
	HAL_StatusTypeDef (*tranRecv)(uint8_t *pTxData, uint8_t *pRxData, uint16_t Size);
	HAL_StatusTypeDef (*tranmit)(uint8_t *pData, uint16_t Size);
	HAL_StatusTypeDef (*receive)(uint8_t *pData, uint16_t Size);
	SPI_STATUS_S (*getStatus)(void);
}SPIdma;


#ifdef __cplusplus
}
#endif

#endif /* SRC_AFE_SPI_AFE_SPI_H_ */

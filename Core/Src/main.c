/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/


/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/


/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef 	hadc1;
DMA_HandleTypeDef 	hdma_adc1;
UART_HandleTypeDef 	huart3;
ADC_STATUS_S 		adcStatus;

/* Variable used to get converted value */
__IO uint16_t adcConvertedValue = 0;


/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_ADC1_Init(void);
static void MX_USART3_UART_Init(void);



/* Private user code ---------------------------------------------------------*/
/************************************************
  * @name 	: _adc_dma_init
  * @brief	:Initialize ADC with DMA operation
  ***********************************************/
static HAL_StatusTypeDef _adc_dma_init(void)
{
	MX_DMA_Init();
	MX_ADC1_Init();
	adcStatus.status = ADC_IDLE;
	return HAL_OK;
}

/************************************************
  * @name 	: _adc_dma_getValue
  * @brief	:Get value from ADC
  ***********************************************/
static HAL_StatusTypeDef _adc_dma_getValue(ADC_CH_E ch, uint32_t* value, uint32_t len, void (*cb)(uint32_t len))
{
	adcStatus.active_channel 	= ch;
	adcStatus.status			= ADC_BUSY;

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

	adcStatus.status			= ADC_IDLE;

	if(cb != NULL){
		cb(len);
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


/**
  * @brief  ADC_DMA complete conversion CB.
  * @retval int
  */
void adcConvComplete(uint32_t len)
{
	HAL_GPIO_TogglePin(GPIOB, LD3_Pin);
}


/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* Configure the system clock */
  SystemClock_Config();

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART3_UART_Init();

  ADCdma.init();

  ADC_STATUS_S adcCurStatus;

  /* Infinite loop */
  while (1)
  {
	  HAL_GPIO_TogglePin(GPIOB, LD1_Pin);
	  HAL_Delay(1000);

	  adcCurStatus = ADCdma.getStatus();
	  if(adcCurStatus.status == ADC_IDLE){
		  if(ADCdma.getValue(ADC_CH10, (uint32_t*)&adcConvertedValue, 1, NULL) != HAL_OK){
			  /* Start Conversion Error */
			  Error_Handler();
		  }else{
			  /* Conversion OK */
			  HAL_GPIO_TogglePin(GPIOB, LD2_Pin);
		  }
	  }else{
		  HAL_GPIO_WritePin(GPIOB, LD3_Pin, GPIO_PIN_RESET);
	  }
  }
}


/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow :
  *            System Clock source            = PLL (HSE)
  *            SYSCLK(Hz)                     = 180000000
  *            HCLK(Hz)                       = 180000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 4
  *            APB2 Prescaler                 = 2
  *            HSE Frequency(Hz)              = 8000000
  *            PLL_M                          = 8
  *            PLL_N                          = 360
  *            PLL_P                          = 2
  *            PLL_Q                          = 7
  *            VDD(V)                         = 3.3
  *            Main regulator output voltage  = Scale1 mode
  *            Flash Latency(WS)              = 5
  * @param  None
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage*/
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters in the RCC_OscInitTypeDef structure.*/
  RCC_OscInitStruct.OscillatorType 	= RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState 		= RCC_HSE_BYPASS;
  RCC_OscInitStruct.PLL.PLLState 	= RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource 	= RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM 		= 8;
  RCC_OscInitStruct.PLL.PLLN 		= 360;
  RCC_OscInitStruct.PLL.PLLP 		= RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ 		= 7;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Activate the Over-Drive mode*/
  if (HAL_PWREx_EnableOverDrive() != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks*/
  RCC_ClkInitStruct.ClockType 		= RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK|RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource 	= RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider 	= RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider 	= RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider 	= RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief ADC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC1_Init(void)
{
  ADC_ChannelConfTypeDef sConfig = {0};

  /** Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion)*/
  hadc1.Instance 					= ADC1;
  hadc1.Init.ClockPrescaler 		= ADC_CLOCK_SYNC_PCLK_DIV4;
  hadc1.Init.Resolution 			= ADC_RESOLUTION_12B;
  hadc1.Init.ScanConvMode 			= DISABLE;				/* Sequencer disabled (ADC conversion on only 1 channel: channel set on rank 1) */
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
  * @brief USART3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART3_UART_Init(void)
{
  huart3.Instance 			= USART3;
  huart3.Init.BaudRate 		= 115200;
  huart3.Init.WordLength 	= UART_WORDLENGTH_8B;
  huart3.Init.StopBits 		= UART_STOPBITS_1;
  huart3.Init.Parity 		= UART_PARITY_NONE;
  huart3.Init.Mode 			= UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl 	= UART_HWCONTROL_NONE;
  huart3.Init.OverSampling 	= UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA2_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA2_Stream0_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA2_Stream0_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA2_Stream0_IRQn);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin : USER_Btn_Pin */
  GPIO_InitStruct.Pin 	= USER_Btn_Pin;
  GPIO_InitStruct.Mode 	= GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull 	= GPIO_NOPULL;
  HAL_GPIO_Init(USER_Btn_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : LD1_Pin LD3_Pin LD2_Pin */
  GPIO_InitStruct.Pin 	= LD1_Pin|LD3_Pin|LD2_Pin;
  GPIO_InitStruct.Mode 	= GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull 	= GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, LD1_Pin|LD3_Pin|LD2_Pin, GPIO_PIN_RESET);
}


/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  __disable_irq();
  while (1)
  {
  }
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

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
#include "stdio.h"
#include "string.h"

/* Private includes ----------------------------------------------------------*/


/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/


/* Private variables ---------------------------------------------------------*/
UART_HandleTypeDef 	huart3;

/* Variable used to get converted value */
__IO uint16_t adcConvertedValue = 0;


/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART3_UART_Init(void);



/* Private user code ---------------------------------------------------------*/

#ifdef __GNUC__
/* With GCC, small printf (option LD Linker->Libraries->Small printf set to 'Yes') calls __io_putchar() */
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */

/******************************************************************
  * @name   PUTCHAR_PROTOTYPE
  * @brief  Retargets the C library printf function to the USART.
  *****************************************************************/
PUTCHAR_PROTOTYPE
{
	HAL_UART_Transmit(&huart3, (uint8_t *)&ch, 1, 0xFFFF);				//Use USART3
	return ch;
}


const ADC_CH_E adc_ch_selection [] =
{
	ADC_CH9,
	ADC_CH10,
	ADC_CH11,
	ADC_CH12,
	ADC_CH13,
};


uint8_t spiTxBuffer[] = "****SPI - SEND SPI Message By Master ****";
uint8_t spiRxBuffer[sizeof(spiTxBuffer)/sizeof(spiTxBuffer[0])];

/**
  * @brief  ADC_DMA complete conversion CB.
  * @retval int
  */
void adcConvComplete(uint32_t len)
{
	HAL_GPIO_TogglePin(GPIOB, LD3_Pin);
}

/**
  * @brief  SPI_DMA complete conversion CB.
  * @retval int
  */
void spiTransferComplete(uint32_t len)
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

  ADCdma.init(adcConvComplete);		//can also be NULL
  SPIdma.init(NULL);

  //ADC_STATUS_S adcCurStatus;

  /* Infinite loop */
  while (1)
  {
	  HAL_GPIO_TogglePin(GPIOB, LD1_Pin);
	  puts("ADC / SPI Start\r\n");
	  HAL_Delay(1000);

//	  for(size_t i=0; i<sizeof(adc_ch_selection)/sizeof(*adc_ch_selection); i++)
//	  {
//		  HAL_Delay(200);
//
//		  adcCurStatus = ADCdma.getStatus();
//		  if(adcCurStatus.status == ADC_IDLE){
//			  if(ADCdma.getValue(ADC_CH10, (uint32_t*)&adcConvertedValue, 1) != HAL_OK){
//				  /* Start Conversion Error */
//				  Error_Handler();
//			  }else{
//				  /* Conversion OK */
//				  HAL_GPIO_TogglePin(GPIOB, LD2_Pin);
//			  }
//		  }
//	  }


	  if(SPIdma.tranRecv((uint8_t*)spiTxBuffer, (uint8_t *)spiRxBuffer, sizeof(spiTxBuffer)/sizeof(spiTxBuffer[0])) != HAL_OK){
		  /* Start Conversion Error */
		  Error_Handler();
	  }else{
		  /* Conversion OK */
		  HAL_GPIO_TogglePin(GPIOB, LD2_Pin);
	  }

	  puts((char*)spiRxBuffer);
	  memset(spiRxBuffer, 0, sizeof(spiTxBuffer)/sizeof(spiTxBuffer[0]));
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

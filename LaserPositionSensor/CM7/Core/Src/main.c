/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "dcmi.h"
#include "dma.h"
#include "i2c.h"
#include "usart.h"
#include "gpio.h"
#include "fmc.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdbool.h>

#include "printf.h"
#include "main.h"
#include "stm32h747i_discovery_sdram.h"

#include "LPS_ov5640.h"
#include "LPS_DCMI.h"
#include "LPS_communication.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

#ifndef HSEM_ID_0
#define HSEM_ID_0 (0U) /* HW semaphore 0*/
#endif

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
extern uint32_t active_buffer;

uint8_t cameraLineBuffer0[CAMERA_LINE_SIZE] __attribute__ ((aligned (32)));
uint8_t cameraLineBuffer1[CAMERA_LINE_SIZE] __attribute__ ((aligned (32)));

uint32_t line_weight_vertical[CAMERA_RES_X];
uint32_t line_weight_horizontal[CAMERA_RES_Y];

uint32_t line_number = 0;

bool process_line = false;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

void _putchar(char character);
void HAL_DCMI_VsyncEventCallback(DCMI_HandleTypeDef *hdcmi);
void HAL_DCMI_LineEventCallback(DCMI_HandleTypeDef *hdcmi);

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void _putchar(char character)
{
	HAL_UART_Transmit(&huart1, (uint8_t*)&character, 1, 10);
}

/* full frame was received */
void HAL_DCMI_VsyncEventCallback(DCMI_HandleTypeDef *hdcmi)
{
	line_number = 0;
}

/* single line was received */
void HAL_DCMI_LineEventCallback(DCMI_HandleTypeDef *hdcmi)
{

}

double calculateCOG(uint32_t *buffer, uint32_t buffer_size)
{
	double weight_sum = 0;
	double cog = 0;

	for (uint32_t i = 1; i < buffer_size + 1; i++)
	{
		cog += buffer[i-1] * i;
		weight_sum += buffer[i-1];
	}
	cog = cog / weight_sum;

	return cog;
}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */
/* USER CODE BEGIN Boot_Mode_Sequence_0 */
  int32_t timeout;
/* USER CODE END Boot_Mode_Sequence_0 */

  /* Enable I-Cache---------------------------------------------------------*/
  SCB_EnableICache();

/* USER CODE BEGIN Boot_Mode_Sequence_1 */
  /* Wait until CPU2 boots and enters in stop mode or timeout*/
  timeout = 0xFFFF;
  while((__HAL_RCC_GET_FLAG(RCC_FLAG_D2CKRDY) != RESET) && (timeout-- > 0));
  if ( timeout < 0 )
  {
  Error_Handler();
  }
/* USER CODE END Boot_Mode_Sequence_1 */
  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();
/* USER CODE BEGIN Boot_Mode_Sequence_2 */
	/* When system initialization is finished, Cortex-M7 will release Cortex-M4 by means of
	HSEM notification */

	__HAL_RCC_HSEM_CLK_ENABLE(); /*HW semaphore Clock enable*/
	HAL_HSEM_FastTake(HSEM_ID_0);	/*Take HSEM */
	HAL_HSEM_Release(HSEM_ID_0,0);/*Release HSEM in order to notify the CPU2(CM4)*/
	/* wait until CPU2 wakes up from stop mode */
	timeout = 0xFFFF;
	while((__HAL_RCC_GET_FLAG(RCC_FLAG_D2CKRDY) == RESET) && (timeout-- > 0)){}
	if ( timeout < 0 )
	{
		Error_Handler();
	}
/* USER CODE END Boot_Mode_Sequence_2 */

  /* USER CODE BEGIN SysInit */
  MX_DMA_Init();
  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART1_UART_Init();
  MX_DMA_Init();
  MX_I2C4_Init();
  MX_DCMI_Init();
  MX_FMC_Init();
  /* USER CODE BEGIN 2 */

  /* init peripherals from BSP */
  BSP_SDRAM_Init(0);

  /* init ov5640 camera with I2C */
  ov5640_init();
  ov5640_SetPixelFormat(LPS_COLOR_FORMAT);
  ov5640_setResolution(LPS_RESOLUTION);

  /* set camera to test mode */
  //ov5640_enableTestMode();

  /* disable auto exposure and set it to lower value */
  ov5640_disableAutoExposure();
  ov5640_setManualExposure(8, 0);

  /* start initial frame capture */
  HAL_Delay(10);
  DCMI_Start_DMA_line(&hdcmi, DCMI_MODE_SNAPSHOT);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	while (1)
	{
		#if CAMERA_BITS_PER_PX==(24)
		/* wait until data in one of the buffers is ready to be processed */
		if (process_line == true)
		{
			process_line = false; //reset flag that was set by DMA TC interrupt

			uint32_t line_sum = 0;

			/* if buffer 0 is accessed by DMA, do calculations on buffer 1 */
			if (active_buffer == 0)
			{
				/* delete channels G and B */
				for (uint32_t i = 0; i < CAMERA_LINE_SIZE; i+=3)
				{
					cameraLineBuffer1[i + 1] = 0;
					cameraLineBuffer1[i + 2] = 0;
				}

				/* delete noise values */
				for (uint32_t i = 0; i < CAMERA_LINE_SIZE; i++)
				{
					if (cameraLineBuffer1[i] < RGB888_THRESHOLD)
						cameraLineBuffer1[i] = 0;
				}

				/* calculate sum of all pixels */
				for (uint32_t i = 0; i < CAMERA_LINE_SIZE; i++)
				{
					/* horizontal */
					line_sum += cameraLineBuffer1[i];

					/* vertical */
					line_weight_vertical[i / (CAMERA_BITS_PER_PX / 8)] += cameraLineBuffer1[i];
				}
			}

			/* if buffer 1 is accessed by DMA, do calculations on buffer 0 */
			if (active_buffer == 1)
			{
				/* delete channels G and B */
				for (uint32_t i = 0; i < CAMERA_LINE_SIZE; i+=3)
				{
					cameraLineBuffer0[i + 1] = 0;
					cameraLineBuffer0[i + 2] = 0;
				}

				/* delete noise values */
				for (uint32_t i = 0; i < CAMERA_LINE_SIZE; i++)
				{
					if (cameraLineBuffer0[i] < RGB888_THRESHOLD)
						cameraLineBuffer0[i] = 0;
				}

				/* calculate sum of all pixels */
				for (uint32_t i = 0; i < CAMERA_LINE_SIZE; i++)
				{
					/* horizontal */
					line_sum += cameraLineBuffer0[i];

					/* vertical */
					line_weight_vertical[i / (CAMERA_BITS_PER_PX / 8)] += cameraLineBuffer0[i];
				}
			}

			/* save sum of the currently processed line */
			line_weight_horizontal[line_number] = line_sum;
			line_number++;
			#endif

		#if CAMERA_BITS_PER_PX==(16)
		/* wait until data in one of the buffers is ready to be processed */
		if (process_line == true)
		{
			process_line = false; //reset flag that was set by DMA TC interrupt

			uint32_t line_sum = 0;

			/* if buffer 0 is accessed by DMA, do calculations on buffer 1 */
			if (active_buffer == 0)
			{
				/* delete channels G and B */
				for (uint32_t i = 0; i < CAMERA_LINE_SIZE; i+=2)
				{
					cameraLineBuffer1[i] = 0;
					cameraLineBuffer1[i + 1] &= 0b11111000;
				}

				/* delete noise values */
				for (uint32_t i = 0; i < CAMERA_LINE_SIZE; i++)
				{
					if (cameraLineBuffer1[i] < RGB565_THRESHOLD)
						cameraLineBuffer1[i] = 0;
				}

				/* calculate sum of all pixels */
				for (uint32_t i = 0; i < CAMERA_LINE_SIZE; i++)
				{
					/* horizontal */
					line_sum += cameraLineBuffer1[i];

					/* vertical */
					line_weight_vertical[i / (CAMERA_BITS_PER_PX / 8)] += cameraLineBuffer1[i];
				}
			}

			/* if buffer 1 is accessed by DMA, do calculations on buffer 0 */
			if (active_buffer == 1)
			{
				/* delete channels G and B */
				for (uint32_t i = 0; i < CAMERA_LINE_SIZE; i+=2)
				{
					cameraLineBuffer0[i] = 0;
					cameraLineBuffer0[i + 1] &= 0b11111000;
				}

				/* delete noise values */
				for (uint32_t i = 0; i < CAMERA_LINE_SIZE; i++)
				{
					if (cameraLineBuffer0[i] < RGB565_THRESHOLD)
						cameraLineBuffer0[i] = 0;
				}

				/* calculate sum of all pixels */
				for (uint32_t i = 0; i < CAMERA_LINE_SIZE; i++)
				{
					/* horizontal */
					line_sum += cameraLineBuffer0[i];

					/* vertical */
					line_weight_vertical[i / (CAMERA_BITS_PER_PX / 8)] += cameraLineBuffer0[i];
				}
			}

			/* save sum of the currently processed line */
			line_weight_horizontal[line_number] = line_sum;
			line_number++;
			#endif

			/* if last line of the frame was taken, print results */
			if (line_number >= CAMERA_RES_Y)
			{
				printDataToMatlab(line_weight_horizontal, CAMERA_RES_Y, line_weight_vertical, CAMERA_RES_X);

				double cog_h = calculateCOG(line_weight_vertical, CAMERA_RES_X);
				double cog_v = calculateCOG(line_weight_horizontal, CAMERA_RES_Y);

				printCOGToMatlab(cog_h, cog_v);

				/* reset values in vertical sums buffer */
				for (uint32_t i = 0; i < CAMERA_LINE_SIZE; i++)
				{
					line_weight_vertical[i / (CAMERA_BITS_PER_PX / 8)] = 0;
				}

				/* start capturing next frame */
				DCMI_Start_DMA_line(&hdcmi, DCMI_MODE_SNAPSHOT);

			}
		}

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
    }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Supply configuration update enable
  */
  HAL_PWREx_ConfigSupply(PWR_DIRECT_SMPS_SUPPLY);

  /** Configure the main internal regulator output voltage
  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 2;
  RCC_OscInitStruct.PLL.PLLN = 64;
  RCC_OscInitStruct.PLL.PLLP = 2;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;
  RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_3;
  RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE;
  RCC_OscInitStruct.PLL.PLLFRACN = 0;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2
                              |RCC_CLOCKTYPE_D3PCLK1|RCC_CLOCKTYPE_D1PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV2;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV2;
  RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
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


/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  ** This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * COPYRIGHT(c) 2019 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f1xx_hal.h"
#include "adc.h"
#include "dma.h"
#include "rtc.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* USER CODE BEGIN Includes */
/******************************************************************
 * �ļ����ƣ�main.c
 * ��    �ߣ�mcli
 * ��    �ڣ�2019/2/28
 * ��    ����v6.0.2
 * ��    �������������ѧϰ����ʹ�ã�������ҵʹ�ã�
 * ��    �ͣ�https://blog.csdn.net/mc_li
 ******************************************************************/
#include "key.h"
#include "fft.h"
#include "display.h"
#include "rtcpro.h"

/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/
uint16_t time_num = 0;

/* USER CODE END PFP */

/* USER CODE BEGIN 0 */
void time_start(void);
uint32_t time_stop(void);
void main_task(void);

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  *
  * @retval None
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration----------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_USART1_UART_Init();
  MX_TIM3_Init();
  MX_ADC1_Init();
  MX_TIM2_Init();
  MX_RTC_Init();
  /* USER CODE BEGIN 2 */
	printf("UART Init OK.\r\n");
	
	/*��ʾ��ʼ��*/
	Display_Init();
	printf("OLED Init Ok.\r\n");
	
	/*RTC��ʼ��*/
	RTC_Init();
	/*�������ò�����ʼ��*/
	Key_Init();
	
	/*����FFT�ɼ�*/
	FFT_Start();

	/*֡�ʼ����õ�1��ʱ��*/
//	HAL_TIM_Base_Start_IT(&htim2);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
  /* USER CODE END WHILE */

  /* USER CODE BEGIN 3 */
		main_task();

  }
  /* USER CODE END 3 */

}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_PeriphCLKInitTypeDef PeriphClkInit;

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE|RCC_OSCILLATORTYPE_LSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_RTC|RCC_PERIPHCLK_ADC;
  PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSE;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV6;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure the Systick interrupt time 
    */
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

    /**Configure the Systick 
    */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/* USER CODE BEGIN 4 */

/* �������ƣ�time_start
 * ������������ʼ��ȡʱ��
 * ��������
 * ����ֵ����
 */
void time_start(void)
{
	htim3.Instance->CNT = 0;
	time_num = 0;
	HAL_TIM_Base_Start_IT(&htim2);
}

/* �������ƣ�time_stop
 * ������������ȡʱ��
 * ��������
 * ����ֵ�����ز���ʱ��
 */
uint32_t time_stop(void)
{
	uint32_t time_val = 0;
	HAL_TIM_Base_Stop_IT(&htim2);
	time_val = htim3.Instance->CNT;
	time_val += (time_num*65535);	//us
	return time_val;
}

/* �������ƣ�HAL_TIM_PeriodElapsedCallback
 * ������������ʱ���ص�����
 * ��������
 * ����ֵ�����ز���ʱ��
 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance == htim2.Instance)
	{
		time_num++;
		if(time_num == 20)
		{
			HAL_TIM_Base_Stop_IT(&htim2);
		}
	}
}

/* �������ƣ�main_task
 * ������������������
 * ��������
 * ����ֵ����
 */
void main_task(void)
{
	static uint8_t ret = SINGLE_CHECK;	// �źż�ⷵ��ֵ
	static uint8_t num = 0;							
	
	/**********FFT�����Լ���ʾ��ʽ�ȴ���*************/
	if(adc_dma_ok == 1)	// ADC ���ݲɼ����
	{
		FFT_Pro();								//FFT����
		ret = Single_Check();			// �źż��
		if(Current_display_mode == DISPLAY_MODE_FFT)	// FFT��ʾģʽ
		{
			if( ret == SINGLE_NOT)	// û���ź�����
			{
				num++;
				if(num >= 5)					// ����һ��������
				{
					num = 0;
					Current_display_mode = DISPLAY_MODE_TIME;	// �л���TIME��ʾ
				}
			}
			else if(ret == SINGLE_OK)
					num = 0;
		}
		else if(Current_display_mode == DISPLAY_MODE_TIME && Set_info.page == DISPLAY)
		{
			if(ret == SINGLE_OK)	// ���ź�����
			{
				num++;
				if(num >= 5)				// ����һ��������
				{
					num = 0;
					Current_display_mode = DISPLAY_MODE_FFT;	// �л���FFT��ʾ
				}
				else if(ret == SINGLE_NOT)
					num = 0;
			}
		}
		FFT_Start();						//������һ�ε�ת��
		adc_dma_ok = 0;					//�����־λ
	}
	/******��ʾ����*********/
	if(Set_info.page == DISPLAY)
	{
		if(Current_display_mode == DISPLAY_MODE_FFT)
		{
				display_pro();	// ��ʾƵ��
		}
		else if(Current_display_mode == DISPLAY_MODE_TIME)
		{
				RTC_Get();			// ��ȡʱ��
				RTC_Display();	// ��ʾʱ��
		}
	}
	/******��������*********/
	Key_Pro();						// ����ɨ��
}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  file: The file name as string.
  * @param  line: The line in file as a number.
  * @retval None
  */
void _Error_Handler(char *file, int line)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  while(1)
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
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

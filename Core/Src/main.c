/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
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
#include "i2c.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdint-gcc.h"
#include "oled.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
int Counter_Flag_TIM3=0,Frequency_Light=20,Frequency_Sound=20,Value_Light=5,Value_Sound=5,Counter_Flag_TIM4=0,Signal_Set_Light=0,Signal_Set_Sound=0;
int Time_Left=3600,State=0,Menu_State=0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

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
  MX_I2C1_Init();
  MX_TIM3_Init();
  MX_TIM4_Init();
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */
  OLED_Init();
  OLED_DisPlay_On();
  HAL_TIM_Base_Start_IT(&htim2);
  HAL_TIM_Base_Start_IT(&htim3);
  HAL_TIM_Base_Start_IT(&htim4);
  
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    Display();
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

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void Display(){
  OLED_NewFrame();
  if (!Menu_State)
  {
    OLED_PrintString(0,0,"光频率:%d",Frequency_Light,&font12x12,OLED_COLOR_NORMAL);
    OLED_PrintString(0,12,"声频率:%d",Frequency_Sound,&font12x12,OLED_COLOR_NORMAL);
    OLED_PrintString(0,24,"光强度:%d",Value_Light,&font12x12,OLED_COLOR_NORMAL);
    OLED_PrintString(0,36,"音量:%d",Value_Sound,&font12x12,OLED_COLOR_NORMAL);
    OLED_PrintString(0,48,"剩余工作时间:%d",Time_Left,&font12x12,OLED_COLOR_NORMAL);
  } else
  {
    OLED_PrintString(0,0,"设置模式",&font12x12,OLED_COLOR_NORMAL);
    switch (State)
    {
      case 0:
        OLED_PrintString(0,12,"光频率:%d",Frequency_Light,&font12x12,OLED_COLOR_NORMAL);
        break;
      case 1:
        OLED_PrintString(0,12,"声频率:%d",Frequency_Sound,&font12x12,OLED_COLOR_NORMAL);
        break;
      case 2:
        OLED_PrintString(0,12,"光强度:%d",Value_Light,&font12x12,OLED_COLOR_NORMAL);
        break;
      case 3:
        OLED_PrintString(0,12,"音量:%d",Value_Sound,&font12x12,OLED_COLOR_NORMAL);
        break;
      case 4:
        OLED_PrintString(0,12,"剩余工作时间:%d",Time_Left,&font12x12,OLED_COLOR_NORMAL);
        break;
      default:
        break;
    }
  }
  OLED_ShowFrame();
}
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
  if (htim==htim2)
  {
    Time_Left--;
    if (!Time_Left)
    {
     HAL_PWR_EnterSTANDBYMode(); 
    }
  }
  if (htim==htim3)
  {
    Counter_Flag_TIM3++;
    if (Counter_Flag_TIM3>=Frequency_Light)
    {
      Signal_Set_Light=0;
    } else
    {
      Signal_Set_Light=1;
    }
    if (Counter_Flag_TIM3>=Frequency_Sound)
    {
      Signal_Set_Sound=0;
    } else
    {
      Signal_Set_Sound=1;
    }
    if (Counter_Flag_TIM3==40)
    {
      Counter_Flag_TIM3=0;
    }
  }
  if (htim==htim4)
  {
    Counter_Flag_TIM4++;
    if (Counter_Flag_TIM4>=Value_Light)
    {
      HAL_GPIO_WritePin(GPIOA,GPIO_PIN_15,GPIO_PIN_RESET);
    } else
    {
      HAL_GPIO_WritePin(GPIOA,GPIO_PIN_15,Signal_Set_Light);
    }
    if (Counter_Flag_TIM4>=Value_Sound)
    {
      HAL_GPIO_WritePin(GPIOB,GPIO_PIN_3,GPIO_PIN_RESET);
    } else
    {
      HAL_GPIO_WritePin(GPIOB,GPIO_PIN_3,Signal_Set_Light);
    }
    if (Counter_Flag_TIM4==10)
    {
      Counter_Flag_TIM4=0;
    }
  }
}
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){
  if (GPIO_Pin==GPIO_PIN_4&&Menu_State)
  {
    switch (State)
    {
    case 0:
      Frequency_Light++;
      break;
    case 1:
      Frequency_Sound++;
      break;
    case 2:
      Value_Light++;
      break;
    case 3:
      Value_Sound++;
      break;
    case 3:
      Time_Left++;
      break;
    default:
      break;
    }
  }
  if (GPIO_Pin==GPIO_PIN_5&&Menu_State)
  {
    switch (State)
    {
    case 0:
      Frequency_Light--;
      break;
    case 1:
      Frequency_Sound--;
      break;
    case 2:
      Value_Light--;
      break;
    case 3:
      Value_Sound--;
      break;
    case 3:
      Time_Left--;
      break;
    default:
      break;
    }
  }
  if (GPIO_Pin==GPIO_PIN_6&&Menu_State)
  {
    if (State+1==5)
    {
      State=0;
    } else
    {
      State++;
    }
  }
  if (GPIO_Pin==GPIO_PIN_7)
  {
    Menu_State=!Menu_State;
  }
}
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

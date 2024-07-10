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
#include "stdio.h"
#include <string.h>
#include <stdlib.h>
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
int Counter_Flag_Light=0,Counter_Flag_Sound=0,Frequency_Light,
Frequency_Sound,Value_Light=10,Value_Sound=10,Counter_Flag_TIM4=0;
double Set_LightFrequency=40,Set_SoundFrequency=40;
int Signal_Set_Light=0,Signal_Set_Sound=0;
int Time_Left=3600,State=0,Menu_State=0;
uint_fast32_t Last_Tick=0;
int Func=18260;
char UART_str[80]="\0";
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void Bluetooth_Init();
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
  Frequency_Light=(int)(Func/Set_LightFrequency);
  Frequency_Sound=(int)(Func/Set_SoundFrequency);
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
  //OLED_Init();
  //OLED_DisPlay_On();
  //Bluetooth_Init();
  HAL_TIM_Base_Start_IT(&htim2);
  HAL_TIM_Base_Start_IT(&htim3);
  HAL_TIM_Base_Start_IT(&htim4);
  HAL_UART_Receive_IT(&huart1,UART_str,30);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    //Display();
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
void BTDecode(){
  char *tmp=strstr(UART_str,"Display");
  if (tmp!=NULL)
  {
    char str[180],at_str[30];
    sprintf(str,"LightFrequency:%lf\r\nSoundFrequency:%lf\r\nLightValue:%d\r\nSoundValue:%d\r\nTimeLeft:%d\r\n",Set_LightFrequency,Set_SoundFrequency,Value_Light,Value_Sound,Time_Left);
    sprintf(at_str,"AT+SEND=0,%d",sizeof(str));
    HAL_UART_Transmit(&huart1,at_str,sizeof(at_str),50);
    HAL_UART_Transmit(&huart1,str,sizeof(str),50);
    HAL_UART_Transmit(&huart1,"+++",sizeof("+++"),50);
    return;
  }
  tmp=strstr(UART_str,"LightFrequency");
  if (tmp!=NULL)
  {
    Set_LightFrequency=atof(tmp+14*sizeof(char));
    Frequency_Light=(int)(Func/Set_LightFrequency);
    return;
  }
  tmp=strstr(UART_str,"SoundFrequency");
  if (tmp!=NULL)
  {
    Set_SoundFrequency=atof(tmp+14*sizeof(char));
    Frequency_Sound=(int)(Func/Set_SoundFrequency);
    return;
  }
  tmp=strstr(UART_str,"LightValue");
  if (tmp!=NULL)
  {
    Value_Light=atoi(tmp+10*sizeof(char));
    return;
  }
  tmp=strstr(UART_str,"SoundValue");
  if (tmp!=NULL)
  {
    Value_Sound=atoi(tmp+10*sizeof(char));
    return;
  }
  tmp=strstr(UART_str,"WorkTime");
  if (tmp!=NULL)
  {
    Time_Left=atoi(tmp+8*sizeof(char));
    return;
  }
}
void Decode(){
  char *tmp=strstr(UART_str,"BTDATA");
  if (tmp!=NULL)
  {
    BTDecode();
  }
  
}
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
  if (huart==&huart1)
  {
    Decode();
  }
  memset(UART_str,0,sizeof(UART_str));
  HAL_UART_Receive_IT(&huart1,UART_str,30);
}
/*暂时弃用
void Bluetooth_Init(){
  HAL_Delay(10000);
  HAL_UART_Transmit(&huart1,"ATE0\r\n",sizeof("ATE0\r\n"),50);
  HAL_Delay(2000);
  HAL_UART_Transmit(&huart1,"AT+BTINIT=1\r\n",sizeof("AT+BTINIT=1\r\n"),50);
  HAL_UART_Receive(&huart1,UART_str,80,2000);
  if (strcmp(UART_str,"\r\nOK\r\n")!=0)
  {
    HAL_UART_Transmit(&huart2,"BT_Init_Error\r\n",sizeof("BT_Init_Error\r\n"),50);
    return;
  }
  HAL_UART_Transmit(&huart1,"AT+BTNAME=\"SteinGate-AT\"\r\n",sizeof("AT+BTNAME=\"SteinGate-AT\"\r\n"),50);
  memset(UART_str,0,sizeof(UART_str));
  HAL_UART_Receive(&huart1,UART_str,80,2000);
  if (strcmp(UART_str,"\r\nOK\r\n")!=0)
  {
    HAL_UART_Transmit(&huart2,"BT_NameSet_Error\r\n",sizeof("BT_NameSet_Error\r\n"),50);
    return;
  }
  HAL_UART_Transmit(&huart1,"AT+BTSCANMODE=2\r\n",sizeof("AT+BTSCANMODE=2\r\n"),50);
  memset(UART_str,0,sizeof(UART_str));
  HAL_UART_Receive(&huart1,UART_str,80,2000);
  if (strcmp(UART_str,"\r\nOK\r\n")!=0)
  {
    HAL_UART_Transmit(&huart2,"BT_ScanModeSet_Error\r\n",sizeof("BT_ScanModeSet_Error\r\n"),50);
    return;
  }
  HAL_UART_Transmit(&huart1,"AT+BTSPPINIT=2\r\n",sizeof("AT+BTSPPINIT=2\r\n"),50);
  memset(UART_str,0,sizeof(UART_str));
  HAL_UART_Receive(&huart1,UART_str,80,2000);
  if (strcmp(UART_str,"\r\nOK\r\n")!=0)
  {
    HAL_UART_Transmit(&huart2,"BT_PPSInit_Error\r\n",sizeof("BT_PPSInit_Error\r\n"),50);
    return;
  }
  HAL_UART_Transmit(&huart1,"AT+BTSPPSTART\r\n",sizeof("AT+BTSPPSTART\r\n"),50);
  memset(UART_str,0,sizeof(UART_str));
  HAL_UART_Receive(&huart1,UART_str,80,2000);
  if (strcmp(UART_str,"\r\nOK\r\n")!=0)
  {
    HAL_UART_Transmit(&huart2,"BT_PPSStart_Error\r\n",sizeof("BT_PPSStart_Error\r\n"),50);
    return;
  }
  HAL_UART_Transmit(&huart1,"BT_WaitForConnect\r\n",sizeof("BT_WaitForConnect\r\n"),50);
  if (strcmp(UART_str,"+BTSPPCONN:0,\"38:c6:bd:9e:44:7c\"\r\n")!=0)
  {
    HAL_UART_Transmit(&huart1,"BT_Connect_Error\r\n",sizeof("BT_Connect_Error\r\n"),50);
    return;
  } else
  {
    HAL_UART_Transmit(&huart1,"BT_Connected\r\n",sizeof("BT_Connected\r\n"),50);
  }
}
*/
/*
void Display(){
  OLED_NewFrame();
  if (!Menu_State)
  {
    char str[80];
    sprintf(str,"光频:%f",Set_LightFrequency);
    OLED_PrintString(0,0,str,&font12x12,OLED_COLOR_NORMAL);
    sprintf(str,"声频:%f",Set_SoundFrequency);
    OLED_PrintString(0,12,str,&font12x12,OLED_COLOR_NORMAL);
    sprintf(str,"光强:%d",Value_Light);
    OLED_PrintString(0,24,str,&font12x12,OLED_COLOR_NORMAL);
    sprintf(str,"音量:%d",Value_Sound);
    OLED_PrintString(0,36,str,&font12x12,OLED_COLOR_NORMAL);
    sprintf(str,"剩余工作时间:%d",Time_Left);
    OLED_PrintString(0,48,str,&font12x12,OLED_COLOR_NORMAL);
  } else
  {
    OLED_PrintString(0,0,"设置模式",&font12x12,OLED_COLOR_NORMAL);
    char str[80];
    switch (State)
    {
      case 0:
        sprintf(str,"光频:%lf",Set_LightFrequency);
        OLED_PrintString(0,12,str,&font12x12,OLED_COLOR_NORMAL);
        break;
      case 1:
        sprintf(str,"声频:%lf",Set_SoundFrequency);
        OLED_PrintString(0,12,str,&font12x12,OLED_COLOR_NORMAL);
        //OLED_PrintString(0,12,,&font12x12,OLED_COLOR_NORMAL);
        break;
      case 2:
        sprintf(str,"光强:%d",Value_Light);
        OLED_PrintString(0,12,str,&font12x12,OLED_COLOR_NORMAL);
        //OLED_PrintString(0,12,,&font12x12,OLED_COLOR_NORMAL);
        break;
      case 3:
        sprintf(str,"音量:%d",Value_Sound);
        OLED_PrintString(0,12,str,&font12x12,OLED_COLOR_NORMAL);
       // OLED_PrintString(0,12,"音量:%d",Value_Sound,&font12x12,OLED_COLOR_NORMAL);
        break;
      case 4:
        sprintf(str,"剩余工作时间:%d",Time_Left);
        OLED_PrintString(0,12,str,&font12x12,OLED_COLOR_NORMAL);
        //OLED_PrintString(0,12,"剩余工作时间:%d",Time_Left,&font12x12,OLED_COLOR_NORMAL);
        break;
      default:
        break;
    }
  }
  OLED_ShowFrame();
}
*/
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
  if (htim->Instance==TIM2)
  {
    Time_Left--;
    if (!Time_Left)
    {
     HAL_PWR_EnterSTANDBYMode(); 
    }
  }
  if (htim->Instance==TIM3)//htim3的周期：7.28*1e-5s
  {
    Counter_Flag_Light++;
    Counter_Flag_Sound++;
    if (Counter_Flag_Light>(int)(0.5*Frequency_Light))
    {
      Signal_Set_Light=0;
    } else
    {
      Signal_Set_Light=1;
    }
    if (Counter_Flag_Sound>(int)(0.5*Frequency_Sound))
    {
      Signal_Set_Sound=0;
    } else
    {
      Signal_Set_Sound=1;
    }
    if (Counter_Flag_Light==Frequency_Light)
    {
      Counter_Flag_Light=0;
    }
    if (Counter_Flag_Sound==Frequency_Sound)
    {
      Counter_Flag_Sound=0;
    }
  }
  if (htim->Instance==TIM4)
  {
    Counter_Flag_TIM4++;
    if (Counter_Flag_TIM4>Value_Light)
    {
      HAL_GPIO_WritePin(GPIOB,GPIO_PIN_4,GPIO_PIN_RESET);
    } else
    {
      HAL_GPIO_WritePin(GPIOB,GPIO_PIN_4,Signal_Set_Light);
    }
    if (Counter_Flag_TIM4>Value_Sound)
    {
      HAL_GPIO_WritePin(GPIOB,GPIO_PIN_5,GPIO_PIN_RESET);
    } else
    {
      HAL_GPIO_WritePin(GPIOB,GPIO_PIN_5,Signal_Set_Light);
    }
    if (Counter_Flag_TIM4==10)
    {
      Counter_Flag_TIM4=0;
    }
  }
}
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){
  uint_fast32_t Now_Tick=HAL_GetTick();
  if(Now_Tick-Last_Tick<10){
    Last_Tick=Now_Tick;
    return;
  }
  Last_Tick=Now_Tick;
  if (GPIO_Pin==GPIO_PIN_4&&Menu_State)
  {
    switch (State)
    {
      case 0:
        Set_LightFrequency+=0.1;
        Set_LightFrequency=Set_LightFrequency>40?40:Set_LightFrequency;
        break;
      case 1:
        Set_SoundFrequency+=0.1;
        Set_SoundFrequency=Set_SoundFrequency>40?40:Set_SoundFrequency;
        break;
      case 2:
        Value_Light++;
        Value_Light=Value_Light>10?10:Value_Light;
        break;
      case 3:
        Value_Sound++;
        Value_Sound=Value_Sound>10?10:Value_Sound;
        break;
      case 4:
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
        Set_LightFrequency-=0.1;
        Set_LightFrequency=Set_LightFrequency<37?37:Set_LightFrequency;
        break;
      case 1:
        Set_SoundFrequency-=0.1;
        Set_SoundFrequency=Set_SoundFrequency<37?37:Set_SoundFrequency;
        break;
      case 2:
        Value_Light--;
        Value_Light=Value_Light<0?0:Value_Light;
        break;
      case 3:
        Value_Sound--;
        Value_Sound=Value_Sound<0?0:Value_Sound;
        break;
      case 4:
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
  Frequency_Light=(int)(Func/Set_LightFrequency);
  Frequency_Sound=(int)(Func/Set_SoundFrequency);
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

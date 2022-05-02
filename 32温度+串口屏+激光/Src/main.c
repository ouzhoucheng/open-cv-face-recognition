/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "i2c.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include "mlx90614.h"
#include "OLED.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
int fputc(int ch, FILE *f)
{
    uint8_t temp[1] = {ch};
    HAL_UART_Transmit(&huart1, temp, 1, 2);
    return ch;
}
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
uint8_t usart2_Re;  //BlueTeeth receive data to control direction
uint16_t Motor_Forward = 0, Motor_Retreat = 0;
uint16_t Steer_Left = 0, Steer_Right = 0, Direction_Angle = 6;
uint16_t FPV_Yaw_Left = 0, FPV_Yaw_Right = 0, FPV_Yaw_Angle = 8;
uint16_t FPV_Pitch_Left = 0, FPV_Pitch_Right = 0, FPV_Pitch_Angle = 8;

//GY-906
float Tem_Real, Q = 0.1, Tem_Avg;
float Tem_Real1 = 0;
float vauleXk1;
float Pkn1, Pk, Zkn1, Zk, Xkn1, Xk, XkA, K;
float T1, T2, T3, T4, T5;
float T11, T22, T33, T44, T55;
uint8_t Tem_Count = 0;
int Reflect_Rate = 62258;   //Object 65536   Human 62258
uint8_t String_Value[10];

uint8_t Usart2_Bt2pi; //Receive from pi
uint8_t Usart1_Bt2Screen;
uint8_t String_Rec[20] = "Rec from pi: ";
uint8_t Send2Rasberry[23] = "32 receive successfully";
uint8_t TDelayCount = 0;
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
/*      uart test
//    sprintf(String_Value," Rec from pi = %c ^", Usart2_Bt2pi);
    String_Rec[12] = (uint8_t)(Usart2_Bt2pi/10);
    String_Rec[13] = Usart2_Bt2pi;
    OLED_ShowStr(0,6,String_Rec,2);
    
    HAL_UART_Transmit(&huart2, Send2Rasberry, 23, 20);
    
    HAL_UART_Receive_IT(&huart2, &Usart2_Bt2pi, 4);
*/
	if(huart->Instance == USART1){
		if(Usart1_Bt2Screen == 0xd8){
//			HAL_GPIO_WritePin(LED0_GPIO_Port, LED0_Pin, GPIO_PIN_RESET);
//			T_Kalmen(1);
//			sprintf(String_Value," T = %.1f ^", Xk);
//			OLED_ShowStr(20,0,String_Value,2);
//			HAL_Delay(100);

//			T_Kalmen(0);
//			HAL_Delay(100);

//				    Tem_Real = Xk + (Xk - XkA)*0.26;
//					Tem_Real1 =Tem_Real*10;//注意定义
//			sprintf(String_Value," T = %.1f ^", Tem_Real);
//			OLED_ShowStr(20,2,String_Value,2);
//                    HAL_Delay(10);
//	                  sprintf(vaule, "%.1f",Tem_Real);
//					  HMI_SetWidgetVaule("t7.txt", vaule);
//                    HAL_Delay(10);
//					  sprintf(vaule1, "%d",(int)Tem_Real1);
//					  HMI_SetWidgetVaule("t6.txt", vaule1);
//                    HAL_Delay(10);
//					  strcpy(cmd,"click 15,1");
//					  HMI_SendCmd(cmd);
			TDelayCount = 1;
		}
        else if(Usart1_Bt2Screen == 0x45){
            TDelayCount = 2;
        }
	}
    HAL_UART_Receive_IT(&huart1, &Usart1_Bt2Screen, 1);
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) 
{
	
    if(GPIO_Pin == KEY0_Pin)
    {
        HAL_GPIO_WritePin(LED0_GPIO_Port, LED0_Pin, GPIO_PIN_RESET);
    }
    
//    T_Kalmen(1);
////    if((Xk > 50.0) || (Xk < 0.0)){goto RESTART1;}
//    sprintf(String_Value," T = %.1f ^", Xk);
//    OLED_ShowStr(20,0,String_Value,2);
//    HAL_Delay(100);

//    T_Kalmen(0);
////    if((XkA > 50.0) || (XkA < 0.0)){goto RESTART1;}
////    sprintf(String_Value," T = %.1f ^", XkA);
////    OLED_ShowStr(20,2,String_Value,2);
//    HAL_Delay(100);

////      Real_Kalmen();
//    Tem_Real = Xk + (Xk - XkA)*0.26;
//				Tem_Real1 =Tem_Real*10;//注意定义
//	
//    sprintf(String_Value," T = %.1f ^", Tem_Real);
//    OLED_ShowStr(20,2,String_Value,2);
//	//串口屏专用
//                    HAL_Delay(10);
//	                  sprintf(vaule, "%.1f",Tem_Real);
//					  HMI_SetWidgetVaule("t7.txt", vaule);
//					  HAL_Delay(10);
//					  sprintf(vaule1, "%d",(int)Tem_Real1);
//					  HMI_SetWidgetVaule("t6.txt", vaule1);
//					  HAL_Delay(10);
//					  strcpy(cmd,"click 15,1");
//					  HMI_SendCmd(cmd);
//                    HAL_Delay(10);
    TDelayCount = 1;
    
//    printf("%f\n",Tem_Real);
}

char vaule[40];
char vaule1[40];
char vauleXk[40];
char cmd[40];
char vaule2[40];
char cmd2[40];

//Templete about
float TemGet[20], TemSum;
//Templete about
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
  MX_USART1_UART_Init();
  MX_I2C1_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */
    HAL_UART_Receive_IT(&huart2, &Usart2_Bt2pi, 1);
    HAL_UART_Receive_IT(&huart1, &Usart1_Bt2Screen, 1);
    HAL_GPIO_WritePin(Xray_GPIO_Port, Xray_Pin, GPIO_PIN_RESET);
    
RESTART1:
    Parameter_Init(Reflect_Rate);   
    
    OLED_Init();    //初始化OLED
    OLED_CLS();
    OLED_Fill(0xff);
    OLED_Fill(0x00);//全屏灭

    HAL_Delay(50);
//Templete about
    T_Kalmen(1);
    if((Xk > 50.0) || (Xk < 0.0)){goto RESTART1;}
    HAL_Delay(100);
    T_Kalmen(0);
    if((XkA > 50.0) || (XkA < 0.0)){goto RESTART1;}
    HAL_Delay(100);
//    for(char i = 0; i<3; i++){
//        T1 = SMBus_ReadTemp();HAL_Delay(10);
//        if((T1 > 50.0) || (T1 < 0.0)){goto RESTART1;}
//        T1 = SMBus_ReadTempA();HAL_Delay(10);
//        if((T1 > 50.0) || (T1 < 0.0)){goto RESTART1;}
//    }
//Templete about
    OLED_ShowStr(0,6,"Tem Check Ready",2);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
      if((TDelayCount == 1)||(TDelayCount == 2)){
          /*
            Xk-Tobject  Tem_Real-Tman
          */
          HAL_GPIO_WritePin(LED0_GPIO_Port, LED0_Pin, GPIO_PIN_RESET);
          HAL_GPIO_TogglePin(Xray_GPIO_Port, Xray_Pin);//Xray 1
//Templete about
			T_Kalmen(1);
//          T_Advance();
//Templete about
			sprintf(String_Value," T = %.1f ^", Xk);
			OLED_ShowStr(20,0,String_Value,2);
			HAL_Delay(100);
          HAL_GPIO_TogglePin(Xray_GPIO_Port, Xray_Pin);//Xray 0
//Templete about
			T_Kalmen(0);
//          T_Advance();
//Templete about
			HAL_Delay(100);
          HAL_GPIO_TogglePin(Xray_GPIO_Port, Xray_Pin);//Xray 1
				    Tem_Real = Xk + (Xk - XkA)*0.26;
					Tem_Real1 =Tem_Real*10;//注意定义
                    vauleXk1 = Xk*10;
          
			sprintf(String_Value," T = %.1f ^", Tem_Real);
			OLED_ShowStr(20,2,String_Value,2);
                      HAL_Delay(10);//人体体温
                      sprintf(vaule, "%.1f",Tem_Real);
                      HMI_SetWidgetVaule("t7.txt", vaule);   
    //                HAL_Delay(10);//物体温度
                      sprintf(vauleXk, "%.1f",Xk);
                      HMI_SetWidgetVaule("t2.txt", vauleXk);

                          HAL_Delay(10);
                          sprintf(vaule1, "%d",(int)(Tem_Real1+0.5));
                          HMI_SetWidgetVaule("t6.txt", vaule1);
              
                          HAL_Delay(10);
                          sprintf(vaule2, "%d",(int)( vauleXk1+0.5));
                          HMI_SetWidgetVaule("t4.txt", vaule2);
                      
              if(TDelayCount == 1){
               
           
   
                    HAL_Delay(10);
					  strcpy(cmd,"click 8,1");//此处12需要根据控件id进行修改，按钮bo id
					  HMI_SendCmd(cmd);
                }
                else if(TDelayCount == 2){
                    HAL_Delay(10);
					  strcpy(cmd2,"click 16,1");//此处12需要根据控件id进行修改
					  HMI_SendCmd(cmd2);
                }
          HAL_Delay(3000);
          HAL_GPIO_WritePin(LED0_GPIO_Port, LED0_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(Xray_GPIO_Port, Xray_Pin, GPIO_PIN_RESET);//Xray 0
          OLED_CLS();
          OLED_ShowStr(0,6,"Tem Check Ready",2);
          TDelayCount = 0;
//          
//          printf("%.1f",Tem_Real);
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

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
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
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

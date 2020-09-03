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
#include "adc.h"
#include "dma.h"
#include "i2c.h"
#include "iwdg.h"
#include "rtc.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include	"string.h" //用于包含memset()函数

#include "lcd.h"
#include "lcd_ui.h"

#include "pca9685.h"

#include "temt6000.h"

#include "sht30_drv.h"

#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h" 
#include "mpu6050Driver.h"

#include "24l01.h"  

#include "control.h"
//#include "bmp.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

#ifdef __GNUC__
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */
 
PUTCHAR_PROTOTYPE
{
    HAL_UART_Transmit(&huart1 , (uint8_t *)&ch, 1, 0xFFFF);
    return ch;
}

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
uint8_t aRxBuffer;   //接收中断缓冲
uint8_t Uart1_RxBuff[256];  //接收缓冲
uint8_t Uart1_Rx_Cnt = 0;  //接收缓冲计数

uint8_t Reve_Data[256];
uint8_t Reve_Order;

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

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
	

	
//	uint8_t SHT30_reg_dat[6] = {0};
	
//	float temperature = 0.0;
//	
//	float humidity = 0.0;
//	
//	float pitch,roll,yaw; 		//欧拉角

//	uint8_t RED_PEO;

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
  MX_DMA_Init();
  MX_ADC1_Init();
  MX_I2C1_Init();
  MX_I2C2_Init();
  MX_SPI1_Init();
  MX_USART1_UART_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  MX_ADC2_Init();
  MX_SPI2_Init();
//  MX_IWDG_Init();
  MX_RTC_Init();
  /* USER CODE BEGIN 2 */
//	HAL_IWDG_Refresh(&hiwdg); //清除看门狗设定值拉姆，独立看门狗计时时间为0.5秒
	HAL_UART_Receive_IT(&huart1, (uint8_t *)&aRxBuffer, 1); //开启接收中断，用来接收串口发送过来的信息
	
	TFT_LCD_INIT();
	
	printf("TFT_LCD init successful!\r\n");
	
	PCA_MG9XX_Init(60,0);//初始化舵机驱动
	
	printf("PCA_MG985 init successful!\r\n");
	
	SHT30_Reset();
	
	while(SHT30_Init() != HAL_OK)
	{
		printf("sht30 init fail.\r\n");
	}		
	printf("sht30 init successful!	\r\n");

	MPU_6050_Init();
	
	while(mpu_dmp_init())
	{
		printf("MPU6050 is error !!!!!!\r\n");
	}
	printf("MPU6050 init successful! \r\n");
	
	while(NRF24L01_Check())	//检查NRF24L01是否在位.	
	{
		printf("NRF24L01 Error\r\n");
	}								   
	NRF24L01_RX_Mode();
	
	printf("NRF24L01	init successful!	\r\n");
	
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1); //开启定时器PWM通道 
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_3); //开启定时器PWM通道 
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1); //开启定时器PWM通道 
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2); //开启定时器PWM通道 
	
	__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, 0); //设置PWM的占空比，这里初始化占空比为0
	__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3, 0); //设置PWM的占空比，这里初始化占空比为0
	__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, 0); //设置PWM的占空比，这里初始化占空比为0
	__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, 0); //设置PWM的占空比，这里初始化占空比为0
	
	printf("PWM init successful! \r\n");
	
	MX_IWDG_Init();
	
	printf("IWDG init successful! \r\n");
	
	printf("ALL is well\r\n");
	
	__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, 130); //设置蜂鸣器的声音。设置PWM的占空比，这里初始化占空比为0
	HAL_Delay(200);
	__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, 0); //设置蜂鸣器的声音。设置PWM的占空比，这里初始化占空比为0

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */
		
		
		
		HAL_IWDG_Refresh(&hiwdg); //清除看门狗设定值，独立看门狗计时时间为0.5秒
		
		print_first_menu();
		
		Read_sensor();
		
		printf("电池电量: %.1f temperature = %.1f, humidity = %.1f		是否检测到了人体：%d		%.1f %.1f %.1f	光照强度值：	%.1f	\r\n"
						,Battery_voltage_history,temperature, humidity,	RED_PEO,	pitch,	roll,	yaw,	Obtain_temt6000_voltage());
		
		
		/*开始进入串口控制部分&
				串口可以控制驱动板的所有模块，串口发送过来的数据分为两部分，第一个字节是命令，第一个字节以后是控制数据
				第一个字节用来判断需要对哪一个模块进行操作；
				后面的字节根据不同的模块分配不同的参数
				 0 ： 控制LED的亮灭；
				 1 ： 控制显示屏幕显示特定数据；
				 2 ： 控制PCA9685模块，来控制16路PWM的输出；
				 3 ： 控制底盘电机的运动，可以控制两个电机的正反转和转速；
				
				注意： 
					Reve_Order 的值直接代表了串口接收到的第一个字节的数据
					Reve_Data  是接收到的从第二个字节开始的数据，一共有255个字节
		*/
		usart_control();
			
		motor_24G_control();
		
		HAL_UART_Receive_IT(&huart1, (uint8_t *)&aRxBuffer, 1); //开启接收中断，用来接收串口发送过来的信息	，为了防止出现串口ORE寄存器出现错误，需要重复进行开启
				
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
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI|RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
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
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_RTC|RCC_PERIPHCLK_ADC;
  PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSI;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV6;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
/* USER CODE BEGIN 4 */
/*串口通讯部分*/
/*串口接收到的程序分为两部分，第一个字节是命令，后面的是数据
*/
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	uint8_t i=0;

	UNUSED(huart);
	
	Uart1_RxBuff[Uart1_Rx_Cnt++] = aRxBuffer;   //接收数据转存
 
	if((Uart1_RxBuff[Uart1_Rx_Cnt-1] == 0x0A)&&(Uart1_RxBuff[Uart1_Rx_Cnt-2] == 0x0D)) //判断结束位，判断数据的\r\n
		{
			/*接收的第一个字节是命令，用来控制不同的模块*/
			Reve_Order = Uart1_RxBuff[0];
			/*如果使用value无法向外传递接收到字符串*/
			/*用Uart1_RxBuff的前两个字节存放命令，后面的字节存放数据
				数据包括文字数据和图片数据一起其他上位机发送给下位机的控制数据，数据内容不一定，
				*/
			for (i=0; i<sizeof(Uart1_RxBuff)-1; i++)
			{	
				Reve_Data[i] = Uart1_RxBuff[i]; 
			}
			
			//printf("PA6 Voltage : %.4f \r\n",2*ADC1_Value*3.3f/4096);
			printf("进入了串口中断 \r\n");
			
			Uart1_Rx_Cnt = 0;
			
			memset(Uart1_RxBuff,0x00,sizeof(Uart1_RxBuff)); //清空数组
		}
 if(HAL_UART_Receive_IT(&huart1, (uint8_t *)&aRxBuffer, 1)	!= HAL_OK) Error_Handler();   //再开启接收中断
	__HAL_UART_ENABLE_IT(&huart1, UART_IT_ERR);
		
	 Connection_status = on_line;
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
	uint8_t i = 0;

	if(__HAL_UART_GET_FLAG(&huart1,UART_FLAG_ORE) != RESET) 
	{
			__HAL_UART_CLEAR_OREFLAG(&huart1);
			HAL_UART_Receive_IT(&huart1,(u8 *)&i,1);
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
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

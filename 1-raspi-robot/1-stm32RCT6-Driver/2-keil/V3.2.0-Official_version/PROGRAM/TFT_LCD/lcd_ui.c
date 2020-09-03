

#include "lcd_ui.h"

#include "spi.h"
#include "usart.h"
#include "lcd.h"
#include "spi.h"
#include "usart.h"
#include "adc.h"
#include "bmp.h"

#include "control.h"

int ADC_Value_BATTERY;
int Battery_voltage = 0; //电池电压的百分比 
float Battery_voltage_[10]; //电池历史电压，用来进行滑动平均滤波 
float Battery_voltage_history; //电池历史电压，用来进行滑动平均滤波 
unsigned char weather = 0;

/*******************************************
* @function: 	使用STM32板载ADC获取电池电压值，将获取到的电压值储存在全局变量中
* @input:			无
* @return:		输出成为全局变量直接使用
* @author:		田小花
* @time:			2020.9.1
*******************************************
*/
void fun(void)
{
}


/*******************************************
* @function: 	对显示屏进行初始化
* @input:			无
* @return:		无
* @author:		田小花
* @time:			2020.9.1
*******************************************
*/
void TFT_LCD_INIT(void)
{
	
	Lcd_Init();			//初始化OLED  

	LCD_Clear(BLACK);
	
	BACK_COLOR=BLACK;
	
	LCD_ShowNum(5,	175,	sTime.Hours,	2,	YELLOW);
	LCD_ShowString( 21,	175, ":",		YELLOW); 
	LCD_ShowNum(29,	175,	sTime.Minutes,	2,	YELLOW);
	LCD_ShowString( 45,	175, ":",		YELLOW); 
	LCD_ShowNum(53,	175,	sTime.Seconds,	2,	YELLOW);
	
	LCD_DrawRectangle		(  5,	195, 106,	281,	YELLOW); //绘制表格
	LCD_DrawLine				(	 5,	225, 106,	225,	YELLOW); //绘制表格里面的框线
	LCD_DrawLine				(	 5,	255, 106,	255,	YELLOW); //绘制表格里面的框线
		
	LCD_ShowNum1				(	10,	200,	21.5,3,	YELLOW); //显示温度 宽度32
	LCD_ShowString			( 42,	200, "/",			YELLOW); // 宽度8
	LCD_ShowNum1				(	50,	200,	21.5123,3,	YELLOW); //显示温度 宽度32
	LCD_ShowString			( 84,	200, "'C",		YELLOW); //把数据显示到屏幕上，显示完成自动换行 宽度16

	LCD_ShowChinese32x32(	10,	230, 0, 	16, YELLOW);   //显示文字温度 宽度16
	LCD_ShowChinese32x32(	26, 230, 1, 	16, YELLOW);   //显示文字湿度 宽度16
	LCD_ShowString			( 42,	230, ":",			YELLOW); //显示冒号 宽度8
	LCD_ShowNum1				(	50,	230,	21.5,3,	YELLOW); //显示温度 宽度32
	LCD_ShowString			( 85,	230, "'C",		YELLOW); //把数据显示到屏幕上，显示完成自动换行 宽度16

	LCD_ShowChinese32x32(	10,	260, 2, 	16,	YELLOW);   //显示文字温度 宽度16
	LCD_ShowChinese32x32(	26, 260, 3, 	16, YELLOW);   //显示文字湿度 宽度16
	LCD_ShowString			( 42,	260, ":",			YELLOW); //显示冒号 宽度8
	LCD_ShowNum1				(	50,	260,	56.3,	 3,	YELLOW); //显示湿度 宽度16
	LCD_ShowString			( 84,	260, "%",			YELLOW); //把数据显示到屏幕上，显示完成自动换行 宽度16
	
	LCD_ShowPicture(0,	 70,		100,	 100,	gImage_sunny);
}

/*******************************************
* @function: 	使用STM32板载ADC获取电池电压值，将获取到的电压值储存在全局变量中，打开ADC1通道10，对应引脚PA4
* @input:			无
* @return:		输出成为全局变量直接使用
* @author:		田小花
* @time:			2020.9.1
*******************************************
*/
void Obtain_battery_voltage(void)
{
	u8 i	=	0;
	
	HAL_ADC_Start(&hadc1);  //打开ADC1
		
	HAL_ADC_PollForConversion(&hadc1, 10); //

	if(HAL_IS_BIT_SET(HAL_ADC_GetState(&hadc1), HAL_ADC_STATE_REG_EOC))
	{
		ADC_Value_BATTERY = HAL_ADC_GetValue(&hadc1); //读取ADC值存放在ADC1_Value中
	}	
	
	Battery_voltage_history = 0 ; //由于获取的电压值会有波动，使用滑动平均滤波，至少需要测量十次才是有效测量
	for(i=0;i<9;i++)
	{
		Battery_voltage_[i] = Battery_voltage_[i+1];
	}
	Battery_voltage_[9] = 2*ADC_Value_BATTERY*3.3f/4096+0.23;	//电池的电压值
	
	for(i=0;i<10;i++) //把储存起来的电池电压的历史值进行求平均值运算
	{
		Battery_voltage_history = Battery_voltage_[i] +	Battery_voltage_history;
	}
	Battery_voltage_history = Battery_voltage_history/10; //最终得到的电池电压的平均值
	
	Battery_voltage = Battery_voltage_history*200 - 741; //最终得到的电池电量的百分比
}



/*******************************************
* @function: 	使用的屏幕为2.4寸TFT_oled,在屏幕上显示主界面
* @input:			电池电量，天气情况，温湿度
* @return:		无
* @author:		田小花
* @time:			2020.9.1
*******************************************
*/

unsigned char flag_body = 0;
unsigned char flag_in_num = 0;
unsigned char	flag_in_num_history = 0;
unsigned char flag_in = 0;

void print_first_menu(void)
{
	//u8 i	=	0;
	
	Obtain_battery_voltage();
	
	
	
	//printf("%.2f  %.2f  %d\r\n", 2*ADC1_Value*3.3f/4096+0.23,Battery_voltage_history,	Battery_voltage);
	
	/****
				处理显示的部分
				在屏幕上显示正常待机的图案
				最上面是显示的命令
				左面是天气情况
						**gImage_cloudy
						**gImage_night_rain
						**gImage_night_sunny
						**gImage_rain
						**gImage_snow
						**gImage_sunny
				右面是大电池的电量显示
					 _____
					|			|
					 _____
				|					|
				|					|
				|					|
				|					|
				|					|
				|					|
				|					|
		  	 __________
			*****/
			/*
			 *显示电池外壳的图标，当电池电量大于百分之10的时候显示的黄色图标
			 *	当电池带能量连百分之10都不到的时候显示的是红色图标
			 *	当电量显示百分之10都不到的时候在电池里面显示惊叹号
				*/
			if(Battery_voltage>10 && flag_body == 1)
			{
				LCD_Fill(150,		70,		210,		90,	YELLOW);//绘制电池上面的扣
				LCD_Fill(130,		90,		230,	 100,	YELLOW);//绘制电池外壳上面部分		
				LCD_Fill(130,	 100,		140,	 270,	YELLOW);//绘制电池外壳中间左面部分
				LCD_Fill(220,	 100,		230,	 270,	YELLOW);//绘制电池外壳中间右面部分		
				LCD_Fill(130,	 270,		230,	 280,	YELLOW);//绘制电池外壳下面部分
				flag_body = 0;
			}
			else if(Battery_voltage<10  &&	flag_body == 0)
			{
				LCD_Fill(150,		70,		210,		90,	RED);//绘制电池上面的扣
				LCD_Fill(130,		90,		230,	 100,	RED);//绘制电池外壳上面部分		
				LCD_Fill(130,	 100,		140,	 270,	RED);//绘制电池外壳中间左面部分
				LCD_Fill(220,	 100,		230,	 270,	RED);//绘制电池外壳中间右面部分		
				LCD_Fill(130,	 270,		230,	 280,	RED);//绘制电池外壳下面部分
				flag_body = 1;
			}
			
			
			
			if(Battery_voltage>	80)
			{
				flag_in_num = 8;
				if(flag_in)
				{
					flag_in = 0;
					LCD_Fill(144,	 104,		216,	 132,	YELLOW);//绘制电池内部的方块
					LCD_Fill(144,	 132,		216,	 136,	BLACK);//绘制电池内部的方块
					LCD_Fill(144,	 136,		216,	 166,	YELLOW);//绘制电池内部的方块
					LCD_Fill(144,	 166,		216,	 170,	BLACK);//绘制电池内部的方块
					LCD_Fill(144,	 170,		216,	 200,	YELLOW);//绘制电池内部的方块
					LCD_Fill(144,	 200,		216,	 204,	BLACK);//绘制电池内部的方块
					LCD_Fill(144,	 204,		216,	 234,	YELLOW);//绘制电池内部的方块
					LCD_Fill(144,	 234,		216,	 238,	BLACK);//绘制电池内部的方块
					LCD_Fill(144,	 238,		216,	 266,	YELLOW);//绘制电池内部的方块
				}
			}
			else if(Battery_voltage>60)
			{
				flag_in_num = 6;
				if(flag_in)
				{
					flag_in = 0;
					LCD_Fill(144,	 104,		216,	 136,	BLACK);//绘制电池内部的方块
					LCD_Fill(144,	 136,		216,	 166,	YELLOW);//绘制电池内部的方块
					LCD_Fill(144,	 166,		216,	 170,	BLACK);//绘制电池内部的方块
					LCD_Fill(144,	 170,		216,	 200,	YELLOW);//绘制电池内部的方块
					LCD_Fill(144,	 200,		216,	 204,	BLACK);//绘制电池内部的方块
					LCD_Fill(144,	 204,		216,	 234,	YELLOW);//绘制电池内部的方块
					LCD_Fill(144,	 234,		216,	 238,	BLACK);//绘制电池内部的方块
					LCD_Fill(144,	 238,		216,	 266,	YELLOW);//绘制电池内部的方块
				}
			}
			else if(Battery_voltage>40)
			{
				flag_in_num = 4;
				if(flag_in)
				{
					flag_in = 0;
					LCD_Fill(144,	 104,		216,	 170,	BLACK);//绘制电池内部的方块
					LCD_Fill(144,	 170,		216,	 200,	YELLOW);//绘制电池内部的方块
					LCD_Fill(144,	 200,		216,	 204,	BLACK);//绘制电池内部的方块
					LCD_Fill(144,	 204,		216,	 234,	YELLOW);//绘制电池内部的方块
					LCD_Fill(144,	 234,		216,	 238,	BLACK);//绘制电池内部的方块
					LCD_Fill(144,	 238,		216,	 266,	YELLOW);//绘制电池内部的方块
				}
			}
			else if(Battery_voltage>20)
			{
				flag_in_num = 2;
				if(flag_in)
				{
					flag_in = 0;
					LCD_Fill(144,	 104,		216,	 204,	BLACK);//绘制电池内部的方块
					LCD_Fill(144,	 204,		216,	 234,	YELLOW);//绘制电池内部的方块
					LCD_Fill(144,	 234,		216,	 238,	BLACK);//绘制电池内部的方块
					LCD_Fill(144,	 238,		216,	 266,	YELLOW);//绘制电池内部的方块
				}
			}
			else if(Battery_voltage>10)
			{
				flag_in_num = 1;
				if(flag_in)
				{
					flag_in = 0;
					LCD_Fill(144,	 104,		216,	 238,	BLACK);//绘制电池内部的方块
					LCD_Fill(144,	 238,		216,	 266,	YELLOW);//绘制电池内部的方块
				}
			}
			else
			{	
				flag_in_num = 0;
				if(flag_in)
				{	
					flag_in = 0;
					LCD_ShowPicture(141,	101,		79,	 170,	gImage_low_power);	
				}
			}
			
			if(flag_in_num_history != flag_in_num)
			{
				flag_in = 1;
			}
			flag_in_num_history = flag_in_num;
			
			LCD_ShowNum(5,	175,	sTime.Hours,	2,	YELLOW);
//			LCD_ShowString( 21,	175, ":",		YELLOW); 
			LCD_ShowNum(29,	175,	sTime.Minutes,	2,	YELLOW);
//			LCD_ShowString( 45,	175, ":",		YELLOW); 
			LCD_ShowNum(53,	175,	sTime.Seconds,	2,	YELLOW);
//			LCD_DrawRectangle		(  5,	195, 106,	281,	YELLOW); //绘制表格
//			LCD_DrawLine				(	 5,	225, 106,	225,	YELLOW); //绘制表格里面的框线
//			LCD_DrawLine				(	 5,	255, 106,	255,	YELLOW); //绘制表格里面的框线
			/*显示天气预报的温湿度*/
			LCD_ShowNum1				(	10,	200,	21.5,3,	YELLOW); //显示温度 宽度32
//			LCD_ShowString			( 42,	200, "/",			YELLOW); // 宽度8
			LCD_ShowNum1				(	50,	200,	21.5123,3,	YELLOW); //显示温度 宽度32
//			LCD_ShowString			( 84,	200, "'C",		YELLOW); //把数据显示到屏幕上，显示完成自动换行 宽度16
			
			/*显示当前环境的温湿度*/
//			LCD_ShowChinese32x32(	10,	230, 0, 	16, YELLOW);   //显示文字温度 宽度16
//			LCD_ShowChinese32x32(	26, 230, 1, 	16, YELLOW);   //显示文字湿度 宽度16
//			LCD_ShowString			( 42,	230, ":",			YELLOW); //显示冒号 宽度8
			LCD_ShowNum1				(	50,	230,	temperature,3,	YELLOW); //显示温度 宽度32
//			LCD_ShowString			( 85,	230, "'C",		YELLOW); //把数据显示到屏幕上，显示完成自动换行 宽度16
			
//			LCD_ShowChinese32x32(	10,	260, 2, 	16,	YELLOW);   //显示文字温度 宽度16
//			LCD_ShowChinese32x32(	26, 260, 3, 	16, YELLOW);   //显示文字湿度 宽度16
//			LCD_ShowString			( 42,	260, ":",			YELLOW); //显示冒号 宽度8
			LCD_ShowNum1				(	50,	260,	humidity,	 3,	YELLOW); //显示湿度 宽度16
//			LCD_ShowString			( 84,	260, "%",			YELLOW); //把数据显示到屏幕上，显示完成自动换行 宽度16
			
			LCD_ShowNum1				(	0,	300,	pitch,	 4,	YELLOW); //显示湿度 宽度16
			LCD_ShowNum1				(	50,	300,	roll,	 4,	YELLOW); //显示湿度 宽度16
			LCD_ShowNum1				(	100,	300,	yaw,	 4,	YELLOW); //显示湿度 宽度16
			
			//在屏幕上的左边显示
			switch(weather)
			{
				case 0:	LCD_ShowPicture(0,	 70,		100,	 100,	gImage_sunny); 	break;
				case 1:	LCD_ShowPicture(0,	 70,		100,	 100,	gImage_snow);		break;
				case 2:	LCD_ShowPicture(0,	 70,		100,	 100,	gImage_rain);		break;
				case 3:	LCD_ShowPicture(0,	 70,		100,	 100,	gImage_cloudy);			break;
				case 4:	LCD_ShowPicture(0,	 70,		100,	 100,	gImage_night_rain);	break;
				case 5:	LCD_ShowPicture(0,	 70,		100,	 100,	gImage_night_sunny);break;
				default:LCD_ShowPicture(0,	 70,		100,	 100,	gImage_sunny);
			}
}







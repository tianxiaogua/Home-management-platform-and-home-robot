#ifndef __LCD_UI_H
#define __LCD_UI_H	

#define weather_sunny 				((uint8_t)0x00)
#define weather_snow	 				((uint8_t)0x01)
#define weather_rain	 				((uint8_t)0x02)
#define weather_cloudy 				((uint8_t)0x03)
#define weather_night_rain	 	((uint8_t)0x04)
#define weather_night_sunny 	((uint8_t)0x05)

extern	int ADC_Value_BATTERY;
extern	int Battery_voltage; //电池电压的百分比 
extern	float Battery_voltage_[10]; //电池历史电压，用来进行滑动平均滤波 
extern	float Battery_voltage_history; //电池历史电压，用来进行滑动平均滤波 
extern	unsigned char weather;


/*******************************************
* @function: 	对显示屏进行初始化
* @input:			无
* @return:		无
* @author:		田小花
* @time:			2020.9.1
*******************************************
*/
void TFT_LCD_INIT(void);
	

	
/*******************************************
* @function: 	使用STM32板载ADC获取电池电压值，将获取到的电压值储存在全局变量中，打开ADC1通道10，对应引脚PA4
* @input:			无
* @return:		输出成为全局变量直接使用
* @author:		田小花
* @time:			2020.9.1
*******************************************
*/
void Obtain_battery_voltage(void);


/*******************************************
* @function: 	使用的屏幕为2.4寸TFT_oled,在屏幕上显示主界面
* @input:			电池电量，天气情况，温湿度
* @return:		无
* @author:		田小花
* @time:			2020.9.1
*******************************************
*/
void print_first_menu(void);
	

#endif



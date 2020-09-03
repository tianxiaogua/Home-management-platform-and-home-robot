

#ifndef __TRANSCEIVER_CONTROL_H
#define __TRANSCEIVER_CONTROL_H

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


#include "lcd.h"
#include "lcd_ui.h"

#include "pca9685.h"

#include "temt6000.h"

#include "sht30_drv.h"

#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h" 
#include "mpu6050Driver.h"

#include "24l01.h" 


#define on_line 0x00
#define off_line 0x01

#define Power_in_serial_port 0x00
#define Power_in_24G_port 0x01

extern uint8_t Connection_status;

extern	float temperature ;
	
extern	float humidity ;

extern	float pitch,roll,yaw; 		//欧拉角

extern	uint8_t RED_PEO;

extern	float Light_intensity; //光照强度

extern	RTC_TimeTypeDef sTime;

extern	RTC_DateTypeDef sDate;


/*******************************************
* @function: 	使用STM32板载ADC获取电池电压值，将获取到的电压值储存在全局变量中
* @input:			无
* @return:		输出成为全局变量直接使用
* @author:		田小花
* @time:			2020.9.1
*******************************************
*/
void Read_sensor(void);

/*******************************************
* @function: 	使用STM32板载ADC获取电池电压值，将获取到的电压值储存在全局变量中
* @input:			无
* @return:		输出成为全局变量直接使用
* @author:		田小花
* @time:			2020.9.1
*******************************************
*/
void motor_24G_control(void);


/*******************************************
* @function: 	使用STM32板载ADC获取电池电压值，将获取到的电压值储存在全局变量中
* @input:			无
* @return:		输出成为全局变量直接使用
* @author:		田小花
* @time:			2020.9.1
*******************************************
*/
void usart_control(void);
	
#endif


































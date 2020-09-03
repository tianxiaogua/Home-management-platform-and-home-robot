#ifndef __TEMT6000_H
#define __TEMT6000_H	


/*******************************************
* @function: 	使用STM32板载ADC获取temt6000引脚的电压值，将获取到的电压值储存在全局变量中
* @input:			无
* @return:		输出读取到的光照值
* @author:		田小花
* @time:			2020.9.1
*******************************************
*/
float Obtain_temt6000_voltage(void);
	


#endif


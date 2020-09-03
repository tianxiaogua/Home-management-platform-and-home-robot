#include "temt6000.h"

#include "adc.h"


/*******************************************
* @function: 	使用STM32板载ADC获取temt6000引脚的电压值，将获取到的电压值储存在全局变量中
* @input:			无
* @return:		输出读取到的光照值
* @author:		田小花
* @time:			2020.9.1
*******************************************
*/
float Obtain_temt6000_voltage(void)
{
	HAL_ADC_Start(&hadc2);
	
	HAL_ADC_PollForConversion(&hadc2, 10);
	
	if(HAL_IS_BIT_SET(HAL_ADC_GetState(&hadc2), HAL_ADC_STATE_REG_EOC))
	{
		return (2*HAL_ADC_GetValue(&hadc2)*3.3f/40);
	}
	else
		return NULL;
}



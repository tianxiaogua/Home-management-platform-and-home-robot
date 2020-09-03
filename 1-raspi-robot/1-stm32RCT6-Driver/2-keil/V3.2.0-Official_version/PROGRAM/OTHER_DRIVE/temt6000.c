#include "temt6000.h"

#include "adc.h"


/*******************************************
* @function: 	ʹ��STM32����ADC��ȡtemt6000���ŵĵ�ѹֵ������ȡ���ĵ�ѹֵ������ȫ�ֱ�����
* @input:			��
* @return:		�����ȡ���Ĺ���ֵ
* @author:		��С��
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



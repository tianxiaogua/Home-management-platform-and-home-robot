#ifndef __LCD_UI_H
#define __LCD_UI_H	

#define weather_sunny 				((uint8_t)0x00)
#define weather_snow	 				((uint8_t)0x01)
#define weather_rain	 				((uint8_t)0x02)
#define weather_cloudy 				((uint8_t)0x03)
#define weather_night_rain	 	((uint8_t)0x04)
#define weather_night_sunny 	((uint8_t)0x05)

extern	int ADC_Value_BATTERY;
extern	int Battery_voltage; //��ص�ѹ�İٷֱ� 
extern	float Battery_voltage_[10]; //�����ʷ��ѹ���������л���ƽ���˲� 
extern	float Battery_voltage_history; //�����ʷ��ѹ���������л���ƽ���˲� 
extern	unsigned char weather;


/*******************************************
* @function: 	����ʾ�����г�ʼ��
* @input:			��
* @return:		��
* @author:		��С��
* @time:			2020.9.1
*******************************************
*/
void TFT_LCD_INIT(void);
	

	
/*******************************************
* @function: 	ʹ��STM32����ADC��ȡ��ص�ѹֵ������ȡ���ĵ�ѹֵ������ȫ�ֱ����У���ADC1ͨ��10����Ӧ����PA4
* @input:			��
* @return:		�����Ϊȫ�ֱ���ֱ��ʹ��
* @author:		��С��
* @time:			2020.9.1
*******************************************
*/
void Obtain_battery_voltage(void);


/*******************************************
* @function: 	ʹ�õ���ĻΪ2.4��TFT_oled,����Ļ����ʾ������
* @input:			��ص����������������ʪ��
* @return:		��
* @author:		��С��
* @time:			2020.9.1
*******************************************
*/
void print_first_menu(void);
	

#endif



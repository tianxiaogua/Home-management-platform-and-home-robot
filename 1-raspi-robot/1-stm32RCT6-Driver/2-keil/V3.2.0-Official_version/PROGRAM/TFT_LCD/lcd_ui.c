

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
int Battery_voltage = 0; //��ص�ѹ�İٷֱ� 
float Battery_voltage_[10]; //�����ʷ��ѹ���������л���ƽ���˲� 
float Battery_voltage_history; //�����ʷ��ѹ���������л���ƽ���˲� 
unsigned char weather = 0;

/*******************************************
* @function: 	ʹ��STM32����ADC��ȡ��ص�ѹֵ������ȡ���ĵ�ѹֵ������ȫ�ֱ�����
* @input:			��
* @return:		�����Ϊȫ�ֱ���ֱ��ʹ��
* @author:		��С��
* @time:			2020.9.1
*******************************************
*/
void fun(void)
{
}


/*******************************************
* @function: 	����ʾ�����г�ʼ��
* @input:			��
* @return:		��
* @author:		��С��
* @time:			2020.9.1
*******************************************
*/
void TFT_LCD_INIT(void)
{
	
	Lcd_Init();			//��ʼ��OLED  

	LCD_Clear(BLACK);
	
	BACK_COLOR=BLACK;
	
	LCD_ShowNum(5,	175,	sTime.Hours,	2,	YELLOW);
	LCD_ShowString( 21,	175, ":",		YELLOW); 
	LCD_ShowNum(29,	175,	sTime.Minutes,	2,	YELLOW);
	LCD_ShowString( 45,	175, ":",		YELLOW); 
	LCD_ShowNum(53,	175,	sTime.Seconds,	2,	YELLOW);
	
	LCD_DrawRectangle		(  5,	195, 106,	281,	YELLOW); //���Ʊ��
	LCD_DrawLine				(	 5,	225, 106,	225,	YELLOW); //���Ʊ������Ŀ���
	LCD_DrawLine				(	 5,	255, 106,	255,	YELLOW); //���Ʊ������Ŀ���
		
	LCD_ShowNum1				(	10,	200,	21.5,3,	YELLOW); //��ʾ�¶� ���32
	LCD_ShowString			( 42,	200, "/",			YELLOW); // ���8
	LCD_ShowNum1				(	50,	200,	21.5123,3,	YELLOW); //��ʾ�¶� ���32
	LCD_ShowString			( 84,	200, "'C",		YELLOW); //��������ʾ����Ļ�ϣ���ʾ����Զ����� ���16

	LCD_ShowChinese32x32(	10,	230, 0, 	16, YELLOW);   //��ʾ�����¶� ���16
	LCD_ShowChinese32x32(	26, 230, 1, 	16, YELLOW);   //��ʾ����ʪ�� ���16
	LCD_ShowString			( 42,	230, ":",			YELLOW); //��ʾð�� ���8
	LCD_ShowNum1				(	50,	230,	21.5,3,	YELLOW); //��ʾ�¶� ���32
	LCD_ShowString			( 85,	230, "'C",		YELLOW); //��������ʾ����Ļ�ϣ���ʾ����Զ����� ���16

	LCD_ShowChinese32x32(	10,	260, 2, 	16,	YELLOW);   //��ʾ�����¶� ���16
	LCD_ShowChinese32x32(	26, 260, 3, 	16, YELLOW);   //��ʾ����ʪ�� ���16
	LCD_ShowString			( 42,	260, ":",			YELLOW); //��ʾð�� ���8
	LCD_ShowNum1				(	50,	260,	56.3,	 3,	YELLOW); //��ʾʪ�� ���16
	LCD_ShowString			( 84,	260, "%",			YELLOW); //��������ʾ����Ļ�ϣ���ʾ����Զ����� ���16
	
	LCD_ShowPicture(0,	 70,		100,	 100,	gImage_sunny);
}

/*******************************************
* @function: 	ʹ��STM32����ADC��ȡ��ص�ѹֵ������ȡ���ĵ�ѹֵ������ȫ�ֱ����У���ADC1ͨ��10����Ӧ����PA4
* @input:			��
* @return:		�����Ϊȫ�ֱ���ֱ��ʹ��
* @author:		��С��
* @time:			2020.9.1
*******************************************
*/
void Obtain_battery_voltage(void)
{
	u8 i	=	0;
	
	HAL_ADC_Start(&hadc1);  //��ADC1
		
	HAL_ADC_PollForConversion(&hadc1, 10); //

	if(HAL_IS_BIT_SET(HAL_ADC_GetState(&hadc1), HAL_ADC_STATE_REG_EOC))
	{
		ADC_Value_BATTERY = HAL_ADC_GetValue(&hadc1); //��ȡADCֵ�����ADC1_Value��
	}	
	
	Battery_voltage_history = 0 ; //���ڻ�ȡ�ĵ�ѹֵ���в�����ʹ�û���ƽ���˲���������Ҫ����ʮ�β�����Ч����
	for(i=0;i<9;i++)
	{
		Battery_voltage_[i] = Battery_voltage_[i+1];
	}
	Battery_voltage_[9] = 2*ADC_Value_BATTERY*3.3f/4096+0.23;	//��صĵ�ѹֵ
	
	for(i=0;i<10;i++) //�Ѵ��������ĵ�ص�ѹ����ʷֵ������ƽ��ֵ����
	{
		Battery_voltage_history = Battery_voltage_[i] +	Battery_voltage_history;
	}
	Battery_voltage_history = Battery_voltage_history/10; //���յõ��ĵ�ص�ѹ��ƽ��ֵ
	
	Battery_voltage = Battery_voltage_history*200 - 741; //���յõ��ĵ�ص����İٷֱ�
}



/*******************************************
* @function: 	ʹ�õ���ĻΪ2.4��TFT_oled,����Ļ����ʾ������
* @input:			��ص����������������ʪ��
* @return:		��
* @author:		��С��
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
				������ʾ�Ĳ���
				����Ļ����ʾ����������ͼ��
				����������ʾ������
				�������������
						**gImage_cloudy
						**gImage_night_rain
						**gImage_night_sunny
						**gImage_rain
						**gImage_snow
						**gImage_sunny
				�����Ǵ��صĵ�����ʾ
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
			 *��ʾ�����ǵ�ͼ�꣬����ص������ڰٷ�֮10��ʱ����ʾ�Ļ�ɫͼ��
			 *	����ش��������ٷ�֮10��������ʱ����ʾ���Ǻ�ɫͼ��
			 *	��������ʾ�ٷ�֮10��������ʱ���ڵ��������ʾ��̾��
				*/
			if(Battery_voltage>10 && flag_body == 1)
			{
				LCD_Fill(150,		70,		210,		90,	YELLOW);//���Ƶ������Ŀ�
				LCD_Fill(130,		90,		230,	 100,	YELLOW);//���Ƶ��������沿��		
				LCD_Fill(130,	 100,		140,	 270,	YELLOW);//���Ƶ������м����沿��
				LCD_Fill(220,	 100,		230,	 270,	YELLOW);//���Ƶ������м����沿��		
				LCD_Fill(130,	 270,		230,	 280,	YELLOW);//���Ƶ��������沿��
				flag_body = 0;
			}
			else if(Battery_voltage<10  &&	flag_body == 0)
			{
				LCD_Fill(150,		70,		210,		90,	RED);//���Ƶ������Ŀ�
				LCD_Fill(130,		90,		230,	 100,	RED);//���Ƶ��������沿��		
				LCD_Fill(130,	 100,		140,	 270,	RED);//���Ƶ������м����沿��
				LCD_Fill(220,	 100,		230,	 270,	RED);//���Ƶ������м����沿��		
				LCD_Fill(130,	 270,		230,	 280,	RED);//���Ƶ��������沿��
				flag_body = 1;
			}
			
			
			
			if(Battery_voltage>	80)
			{
				flag_in_num = 8;
				if(flag_in)
				{
					flag_in = 0;
					LCD_Fill(144,	 104,		216,	 132,	YELLOW);//���Ƶ���ڲ��ķ���
					LCD_Fill(144,	 132,		216,	 136,	BLACK);//���Ƶ���ڲ��ķ���
					LCD_Fill(144,	 136,		216,	 166,	YELLOW);//���Ƶ���ڲ��ķ���
					LCD_Fill(144,	 166,		216,	 170,	BLACK);//���Ƶ���ڲ��ķ���
					LCD_Fill(144,	 170,		216,	 200,	YELLOW);//���Ƶ���ڲ��ķ���
					LCD_Fill(144,	 200,		216,	 204,	BLACK);//���Ƶ���ڲ��ķ���
					LCD_Fill(144,	 204,		216,	 234,	YELLOW);//���Ƶ���ڲ��ķ���
					LCD_Fill(144,	 234,		216,	 238,	BLACK);//���Ƶ���ڲ��ķ���
					LCD_Fill(144,	 238,		216,	 266,	YELLOW);//���Ƶ���ڲ��ķ���
				}
			}
			else if(Battery_voltage>60)
			{
				flag_in_num = 6;
				if(flag_in)
				{
					flag_in = 0;
					LCD_Fill(144,	 104,		216,	 136,	BLACK);//���Ƶ���ڲ��ķ���
					LCD_Fill(144,	 136,		216,	 166,	YELLOW);//���Ƶ���ڲ��ķ���
					LCD_Fill(144,	 166,		216,	 170,	BLACK);//���Ƶ���ڲ��ķ���
					LCD_Fill(144,	 170,		216,	 200,	YELLOW);//���Ƶ���ڲ��ķ���
					LCD_Fill(144,	 200,		216,	 204,	BLACK);//���Ƶ���ڲ��ķ���
					LCD_Fill(144,	 204,		216,	 234,	YELLOW);//���Ƶ���ڲ��ķ���
					LCD_Fill(144,	 234,		216,	 238,	BLACK);//���Ƶ���ڲ��ķ���
					LCD_Fill(144,	 238,		216,	 266,	YELLOW);//���Ƶ���ڲ��ķ���
				}
			}
			else if(Battery_voltage>40)
			{
				flag_in_num = 4;
				if(flag_in)
				{
					flag_in = 0;
					LCD_Fill(144,	 104,		216,	 170,	BLACK);//���Ƶ���ڲ��ķ���
					LCD_Fill(144,	 170,		216,	 200,	YELLOW);//���Ƶ���ڲ��ķ���
					LCD_Fill(144,	 200,		216,	 204,	BLACK);//���Ƶ���ڲ��ķ���
					LCD_Fill(144,	 204,		216,	 234,	YELLOW);//���Ƶ���ڲ��ķ���
					LCD_Fill(144,	 234,		216,	 238,	BLACK);//���Ƶ���ڲ��ķ���
					LCD_Fill(144,	 238,		216,	 266,	YELLOW);//���Ƶ���ڲ��ķ���
				}
			}
			else if(Battery_voltage>20)
			{
				flag_in_num = 2;
				if(flag_in)
				{
					flag_in = 0;
					LCD_Fill(144,	 104,		216,	 204,	BLACK);//���Ƶ���ڲ��ķ���
					LCD_Fill(144,	 204,		216,	 234,	YELLOW);//���Ƶ���ڲ��ķ���
					LCD_Fill(144,	 234,		216,	 238,	BLACK);//���Ƶ���ڲ��ķ���
					LCD_Fill(144,	 238,		216,	 266,	YELLOW);//���Ƶ���ڲ��ķ���
				}
			}
			else if(Battery_voltage>10)
			{
				flag_in_num = 1;
				if(flag_in)
				{
					flag_in = 0;
					LCD_Fill(144,	 104,		216,	 238,	BLACK);//���Ƶ���ڲ��ķ���
					LCD_Fill(144,	 238,		216,	 266,	YELLOW);//���Ƶ���ڲ��ķ���
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
//			LCD_DrawRectangle		(  5,	195, 106,	281,	YELLOW); //���Ʊ��
//			LCD_DrawLine				(	 5,	225, 106,	225,	YELLOW); //���Ʊ������Ŀ���
//			LCD_DrawLine				(	 5,	255, 106,	255,	YELLOW); //���Ʊ������Ŀ���
			/*��ʾ����Ԥ������ʪ��*/
			LCD_ShowNum1				(	10,	200,	21.5,3,	YELLOW); //��ʾ�¶� ���32
//			LCD_ShowString			( 42,	200, "/",			YELLOW); // ���8
			LCD_ShowNum1				(	50,	200,	21.5123,3,	YELLOW); //��ʾ�¶� ���32
//			LCD_ShowString			( 84,	200, "'C",		YELLOW); //��������ʾ����Ļ�ϣ���ʾ����Զ����� ���16
			
			/*��ʾ��ǰ��������ʪ��*/
//			LCD_ShowChinese32x32(	10,	230, 0, 	16, YELLOW);   //��ʾ�����¶� ���16
//			LCD_ShowChinese32x32(	26, 230, 1, 	16, YELLOW);   //��ʾ����ʪ�� ���16
//			LCD_ShowString			( 42,	230, ":",			YELLOW); //��ʾð�� ���8
			LCD_ShowNum1				(	50,	230,	temperature,3,	YELLOW); //��ʾ�¶� ���32
//			LCD_ShowString			( 85,	230, "'C",		YELLOW); //��������ʾ����Ļ�ϣ���ʾ����Զ����� ���16
			
//			LCD_ShowChinese32x32(	10,	260, 2, 	16,	YELLOW);   //��ʾ�����¶� ���16
//			LCD_ShowChinese32x32(	26, 260, 3, 	16, YELLOW);   //��ʾ����ʪ�� ���16
//			LCD_ShowString			( 42,	260, ":",			YELLOW); //��ʾð�� ���8
			LCD_ShowNum1				(	50,	260,	humidity,	 3,	YELLOW); //��ʾʪ�� ���16
//			LCD_ShowString			( 84,	260, "%",			YELLOW); //��������ʾ����Ļ�ϣ���ʾ����Զ����� ���16
			
			LCD_ShowNum1				(	0,	300,	pitch,	 4,	YELLOW); //��ʾʪ�� ���16
			LCD_ShowNum1				(	50,	300,	roll,	 4,	YELLOW); //��ʾʪ�� ���16
			LCD_ShowNum1				(	100,	300,	yaw,	 4,	YELLOW); //��ʾʪ�� ���16
			
			//����Ļ�ϵ������ʾ
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







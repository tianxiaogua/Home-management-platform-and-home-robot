#include "control.h"

uint8_t Connection_status = off_line;

uint8_t Motor_control_power  = Power_in_serial_port; //��ʼ״̬�����Ŀ���Ȩ���ڴ�����

uint8_t connect_24G_flag = 0;

float temperature = 0.0;
	
float humidity = 0.0;

float pitch,roll,yaw; 		//ŷ����

uint8_t RED_PEO;

float Light_intensity = 0; //����ǿ��

RTC_TimeTypeDef sTime = {0};

RTC_DateTypeDef sDate = {0};
	
/*******************************************
* @function: 	ʹ��STM32����ADC��ȡ��ص�ѹֵ������ȡ���ĵ�ѹֵ������ȫ�ֱ�����
* @input:			��
* @return:		�����Ϊȫ�ֱ���ֱ��ʹ��
* @author:		��С��
* @time:			2020.9.1
*******************************************
*/
void Read_sensor(void)
{
	uint8_t SHT30_reg_dat[6] = {0};
	
	HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN); //��ȡʱ��
	//printf("%dʱ%d��%d��\r\n",sTime.Hours,	sTime.Minutes,	sTime.Seconds);
	
	while(mpu_dmp_get_data(&pitch,&roll,&yaw)!=0);

	if(SHT30_Read_Dat(SHT30_reg_dat) == HAL_OK)
	{
		SHT30_Dat_To_Float(SHT30_reg_dat, &temperature, &humidity);
	}
	
	RED_PEO = HAL_GPIO_ReadPin(RED_PEO_IOIN_GPIO_Port,RED_PEO_IOIN_Pin);
	
	Light_intensity = Obtain_temt6000_voltage();
}


/*******************************************
* @function: 	ʹ��STM32����ADC��ȡ��ص�ѹֵ������ȡ���ĵ�ѹֵ������ȫ�ֱ�����
* @input:			��
* @return:		�����Ϊȫ�ֱ���ֱ��ʹ��
* @author:		��С��
* @time:			2020.9.1
*******************************************
*/
void motor_24G_control(void)
{
	unsigned char tmp_buf[33]; 
	int PWM_motor_1 = 0;
	int PWM_motor_2 = 0;
	
	if(NRF24L01_RxPacket(tmp_buf)==0)
	{
		tmp_buf[32]=0;//�����ַ���������
		
		connect_24G_flag = 0; 
		
		if(tmp_buf[0] == tmp_buf[1]	&& tmp_buf[2] == tmp_buf[1])
		{
			printf("connect error	connect error	connect error	connect error	connect error	connect error	\r\n");
		}
		else
		{
			if(tmp_buf[2]<=13) //ǰ��
			{
				Motor_control_power = Power_in_24G_port;	//�趨����Ŀ���Ȩ����2.4GͨѶ��
				
				if(tmp_buf[1]>=19) //��ת
				{
					PWM_motor_1 = (13 - tmp_buf[2]) + (tmp_buf[1] - 19);
					PWM_motor_2 = (13 - tmp_buf[2]) - (tmp_buf[1] - 19);
					
					if(PWM_motor_1>13)
					{
						PWM_motor_1 = 13;
					}
					if(PWM_motor_2 <= 0)
					{
						PWM_motor_2 = 0;
					}
				}
				if(tmp_buf[1]<=13) //��ת
				{
					PWM_motor_1 = (13 - tmp_buf[2]) - (13 - tmp_buf[1]);
					PWM_motor_2 = (13 - tmp_buf[2]) + (13 - tmp_buf[1]);
					
					if(PWM_motor_2 >= 13)
					{
						PWM_motor_2 = 13;
					}
					if(PWM_motor_1 <= 0)
					{
						PWM_motor_1 = 0;
					}
				}
				if(tmp_buf[1]>13 && tmp_buf[1]<19) //������ת
				{
						PWM_motor_1 = (13 - tmp_buf[2]);
						
						PWM_motor_2 = (13 - tmp_buf[2]);
				}
				__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1,	400 - PWM_motor_1 * 30); //���ݽ��յ�����ת�������Ϳ���PWM
				
					HAL_GPIO_WritePin(motor1_GPIO_Port,motor1_Pin,GPIO_PIN_SET); //�������һ����������Ϊ��ͨio��ֱ�ӿ��Ƶ�ƽ����������ת

				__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3,	400 - PWM_motor_2 *30); //���ݽ��յ�����ת�������Ϳ���PWM
				
				HAL_GPIO_WritePin(motor2_GPIO_Port,motor2_Pin,GPIO_PIN_SET);//�������һ����������Ϊ��ͨio��ֱ�ӿ��Ƶ�ƽ����������ת
			}
			if(tmp_buf[2]>=19) //����
			{
				if(tmp_buf[1]>=19) //��ת
				{
					PWM_motor_1 = (tmp_buf[2]-19) + (tmp_buf[1] - 19);
					PWM_motor_2 = (tmp_buf[2]-19) - (tmp_buf[1] - 19);
					
					if(PWM_motor_1>13)
					{
						PWM_motor_1 = 13;
					}
					if(PWM_motor_2 <= 0)
					{
						PWM_motor_2 = 0;
					}
				}
				if(tmp_buf[1]<=13) //��ת
				{
					PWM_motor_1 = (tmp_buf[2]-19) - (13 - tmp_buf[1]);
					PWM_motor_2 = (tmp_buf[2]-19) + (13 - tmp_buf[1]);
					
					if(PWM_motor_2 >= 13)
					{
						PWM_motor_2 = 13;
					}
					if(PWM_motor_1 <= 0)
					{
						PWM_motor_1 = 0;
					}
				}
				if(tmp_buf[1]>13 && tmp_buf[1]<19) //������ת
				{
						PWM_motor_1 = (tmp_buf[2]-19);
						
						PWM_motor_2 = (tmp_buf[2]-19);
				}
				__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1,	200+PWM_motor_1*38); //���ݽ��յ�����ת�������Ϳ���PWM
					
					HAL_GPIO_WritePin(motor1_GPIO_Port,motor1_Pin,GPIO_PIN_RESET); //�������һ����������Ϊ��ͨio��ֱ�ӿ��Ƶ�ƽ����������ת

				__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3,	200+PWM_motor_2*38); //���ݽ��յ�����ת�������Ϳ���PWM		
				
				HAL_GPIO_WritePin(motor2_GPIO_Port,motor2_Pin,GPIO_PIN_RESET);//�������һ����������Ϊ��ͨio��ֱ�ӿ��Ƶ�ƽ����������ת			
			}
		  if(tmp_buf[2]>13 && tmp_buf[2]<19)
			{
				
				if(tmp_buf[1] == 32)
				{
					__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1,	0); //���ݽ��յ�����ת�������Ϳ���PWM
			
					HAL_GPIO_WritePin(motor1_GPIO_Port,motor1_Pin,GPIO_PIN_SET); //�������һ����������Ϊ��ͨio��ֱ�ӿ��Ƶ�ƽ����������ת

					__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3,	1000); //���ݽ��յ�����ת�������Ϳ���PWM
					
					HAL_GPIO_WritePin(motor2_GPIO_Port,motor2_Pin,GPIO_PIN_RESET);//�������һ����������Ϊ��ͨio��ֱ�ӿ��Ƶ�ƽ����������ת
				}
				if(tmp_buf[1] == 0)
				{
					__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1,	1000); //���ݽ��յ�����ת�������Ϳ���PWM
			
					HAL_GPIO_WritePin(motor1_GPIO_Port,motor1_Pin,GPIO_PIN_RESET); //�������һ����������Ϊ��ͨio��ֱ�ӿ��Ƶ�ƽ����������ת

					__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3,	0); //���ݽ��յ�����ת�������Ϳ���PWM
					
					HAL_GPIO_WritePin(motor2_GPIO_Port,motor2_Pin,GPIO_PIN_SET);//�������һ����������Ϊ��ͨio��ֱ�ӿ��Ƶ�ƽ����������ת
				} 
				if(tmp_buf[1]>13 && tmp_buf[1]<19 && Motor_control_power  == Power_in_24G_port)
				{
					error:
					
					__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1,	0); //���ݽ��յ�����ת�������Ϳ���PWM
			
					HAL_GPIO_WritePin(motor1_GPIO_Port,motor1_Pin,GPIO_PIN_RESET); //�������һ����������Ϊ��ͨio��ֱ�ӿ��Ƶ�ƽ����������ת

					__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3,	0); //���ݽ��յ�����ת�������Ϳ���PWM
					
					HAL_GPIO_WritePin(motor2_GPIO_Port,motor2_Pin,GPIO_PIN_RESET);//�������һ����������Ϊ��ͨio��ֱ�ӿ��Ƶ�ƽ����������ת
				}
			}
		}
	}
	//����2.4G����������Ĺ�����ͻȻ�Ͽ���û�и��õĴ����Ӧ�԰취��2.4G�����ӹ�����Ҳ�����û���ӵ����
	else
	{
		if(Motor_control_power == Power_in_24G_port && connect_24G_flag	>11)
		{
			connect_24G_flag = 10;
			
			Motor_control_power = Power_in_serial_port;
			
			printf("�Ѿ��Ͽ�����\r\n");
			
			goto error;
		}
		connect_24G_flag++;
	}
}


/*******************************************
* @function: 	ʹ��STM32����ADC��ȡ��ص�ѹֵ������ȡ���ĵ�ѹֵ������ȫ�ֱ�����
* @input:			��
* @return:		�����Ϊȫ�ֱ���ֱ��ʹ��
* @author:		��С��
* @time:			2020.9.1
*******************************************
*/
void usart_control(void)
{
	static	uint8_t	LCD_Number_of_rows = 0 ;
	int i = 0;
	int value_PWM;
	static uint8_t Beep_flag = 0;
	char assic_transformation = 0;
	/*��ʼ���봮�ڿ��Ʋ���&
		���ڿ��Կ��������������ģ�飬���ڷ��͹��������ݷ�Ϊ�����֣���һ���ֽ��������һ���ֽ��Ժ��ǿ�������
		��һ���ֽ������ж���Ҫ����һ��ģ����в�����
		������ֽڸ��ݲ�ͬ��ģ����䲻ͬ�Ĳ���
		 0 �� ����LED������
		 1 �� ������ʾ��Ļ��ʾ�ض����ݣ�
		 2 �� ����PCA9685ģ�飬������16·PWM�������
		 3 �� ���Ƶ��̵�����˶������Կ����������������ת��ת�٣�
		
		ע�⣺ 
			Reve_Order ��ֱֵ�Ӵ����˴��ڽ��յ��ĵ�һ���ֽڵ�����
			Reve_Data  �ǽ��յ��Ĵӵڶ����ֽڿ�ʼ�����ݣ�һ����255���ֽ�
	*/
	if(Reve_Order =='0') //�����ڿ���LED����
	{
		/*���ݲ���һ��һ���ֽڣ�
			0����ر�LED�ƣ�
			1������LED�ƣ�
		*/
		if(Reve_Data[1] == '0'){ HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET);}
		
		else if(Reve_Data[1] =='1') {	HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET);}
		
		Reve_Order = ' ';
		
		printf("set led ok  \r\n");
	}
		
	else if(Reve_Order =='1')  //�����ڿ�����ʾ������
	{  			
		/*���ݲ�����0��255���ֽڷ�Χ�ڣ�
			�����͹���������ֱ����ʾ����Ļ�ϡ�
		*/
		for(i=0;i<sizeof(Reve_Data);i++) //����Ҫ��ʾ������
		{
			if(Reve_Data[i]== '\0') //�����յ������ݵĽ�β
			{
				Reve_Data[i-2] = Reve_Data[i]; //�ѽ�β��'\0'��ǰ��'\r'ǰ�棬��ֹ��ʾ�����ݳ�������
			}
		}
		Reve_Data[0] = '>'; //����ʾ��������ǰ�����'>'�ַ�
			
			
		if(LCD_Number_of_rows>48)
		{
			LCD_Number_of_rows = 0;
		}	
		LCD_Fill(0,	 LCD_Number_of_rows,		LCD_W,	 LCD_Number_of_rows+16,	BLACK);//���Ƶ���ڲ��ķ���	
		
		LCD_ShowString(0, LCD_Number_of_rows, Reve_Data, YELLOW); //��������ʾ����Ļ��
		
		LCD_Number_of_rows = LCD_Number_of_rows+16;
		
		Reve_Order = ' '; //����������������
		
		printf("set OLED ok   \r\n");
	}
		
	else if(Reve_Order =='2')  //�����ڿ��ƶ������
	{		
		/*���յ�����Ϊa��p�Ŷ������Ƕȣ�ÿ�ο��Կ��ƶ�����
			�ѽ��յ�������ȫ��ѭ��������⵽��ĸ��ʱ���Զ�������ĸ�����ŵ���λ���ݣ�
			����λ���ݾ��Ƕ���ĽǶȣ����ƶ���ĽǶȵ�������ҪΪ001��090��180���ָ�ʽ�����ȱ�������λ�����Ȳ�����Ҫ��0���棻
		*/
		for(i=1;i<sizeof(Reve_Data);i++)
		{
			assic_transformation = (int)Reve_Data[i]; //�ѽ��յ����ַ�������ת��Ϊ��������
			
			if( 96<assic_transformation && assic_transformation<113 ) //�ж���������ǲ�����a��p��Χ�ڵ���ĸ������ǣ���ʼ��ת������ĽǶ�����
			{
				value_PWM = ((int)Reve_Data[i+1]-48)*100+ ((int)Reve_Data[i+2]-48)*10+ (int)Reve_Data[i+3]-48;//���ַ��ͽǶ�����ת��������
					
				if(value_PWM>= 180)value_PWM = 179; //�ԽǶȴ�С���п��ƣ����ó�����Χ
				
				else if (value_PWM <= 1) value_PWM = 1; 
				
				printf("set PWM ok %c  value_PWM: %d  \r\n",assic_transformation ,value_PWM);
				
				switch (assic_transformation) //������ĸ����Ĳ�ͬ�Ķ�������ò�ͬ�ĽǶ�
				{
					case 'a':PCA_MG9XX(0, value_PWM); break;//ͨ��0 ת��value_PWM��С�ĽǶ�
					case 'b':PCA_MG9XX(1, value_PWM); break;//ͨ��1 ת��value_PWM��С�ĽǶ�
					case 'c':PCA_MG9XX(2, value_PWM); break;//ͨ��2 ת��value_PWM��С�ĽǶ�
					case 'd':PCA_MG9XX(3, value_PWM); break;//ͨ��3 ת��value_PWM��С�ĽǶ�
					case 'e':PCA_MG9XX(4, value_PWM); break;//ͨ��4 ת��value_PWM��С�ĽǶ�
					case 'f':PCA_MG9XX(5, value_PWM); break;//ͨ��5 ת��value_PWM��С�ĽǶ�
					case 'g':PCA_MG9XX(6, value_PWM); break;//ͨ��6 ת��value_PWM��С�ĽǶ�
					case 'h':PCA_MG9XX(7, value_PWM); break;//ͨ��7 ת��value_PWM��С�ĽǶ�
					case 'i':PCA_MG9XX(8, value_PWM); break;//ͨ��8 ת��value_PWM��С�ĽǶ�
					case 'j':PCA_MG9XX(9, value_PWM); break;//ͨ��9 ת��value_PWM��С�ĽǶ�
					case 'k':PCA_MG9XX(10,value_PWM); break;//ͨ��10 ת��value_PWM��С�ĽǶ�
					case 'l':PCA_MG9XX(11,value_PWM); break;//ͨ��11 ת��value_PWM��С�ĽǶ�
					case 'm':PCA_MG9XX(12,value_PWM); break;//ͨ��12 ת��value_PWM��С�ĽǶ�
					case 'n':PCA_MG9XX(13,value_PWM); break;//ͨ��13 ת��value_PWM��С�ĽǶ�
					case 'o':PCA_MG9XX(14,value_PWM); break;//ͨ��14 ת��value_PWM��С�ĽǶ�
					case 'p':PCA_MG9XX(15,value_PWM); break;//ͨ��15 ת��value_PWM��С�ĽǶ�
				}
//				assic_transformation = 0; //�ѵõ�����ĸ���
			}
			if(Reve_Data[i] == '\n')break; //������ݽ������Զ�����forѭ��
		}
		Reve_Order = ' ';
	}
	else if(Reve_Order =='3')  //�����ڿ��Ƶ��̵��������
	{
		/*���ݲ������4���ֽ�
			���յ������ݷ�Ϊ��������Ŀ��ƣ�ÿ������Ŀ��Ʒ�Ϊǰ��ͽǶ�
			��������Ϊg0a9 Ĭ��ǰ�����ֽڿ��Ƶ�һ��������������ֽڿ�����һ��
			g��goҲ����ǰ�����ٶ�Ϊ0����ֹͣת����a��after�Ǻ��ˣ��ٶ�Ϊ9���������ת��
			�����������ݲ����ϸ�ʽ�������������ת��
			__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1,	PWM_Motor1); //���ݽ��յ�����ת�������Ϳ���PWM
			HAL_GPIO_WritePin(motor1_GPIO_Port,motor1_Pin,GPIO_PIN_SET); //�������һ����������Ϊ��ͨio��ֱ�ӿ��Ƶ�ƽ����������ת

			__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3,	PWM_Motor2); //���ݽ��յ�����ת�������Ϳ���PWM
			HAL_GPIO_WritePin(motor2_GPIO_Port,motor2_Pin,GPIO_PIN_SET);//�������һ����������Ϊ��ͨio��ֱ�ӿ��Ƶ�ƽ����������ת
		*/
		if( Reve_Data[1] == 'g')
		{	
			__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1,	1000-((int)Reve_Data[2]-48)*100); //���ݽ��յ�����ת�������Ϳ���PWM
			
			HAL_GPIO_WritePin(motor1_GPIO_Port,motor1_Pin,GPIO_PIN_SET); //�������һ����������Ϊ��ͨio��ֱ�ӿ��Ƶ�ƽ����������ת
		}
		if( Reve_Data[1] == 'a')
		{
			__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1,	((int)Reve_Data[2]-48)*100); //���ݽ��յ�����ת�������Ϳ���PWM
			
			HAL_GPIO_WritePin(motor1_GPIO_Port,motor1_Pin,GPIO_PIN_RESET); //�������һ����������Ϊ��ͨio��ֱ�ӿ��Ƶ�ƽ����������ת
		}
		if( Reve_Data[3] == 'g')
		{
			__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3,	1000-((int)Reve_Data[4]-48)*100); //���ݽ��յ�����ת�������Ϳ���PWM
			
			HAL_GPIO_WritePin(motor2_GPIO_Port,motor2_Pin,GPIO_PIN_SET);//�������һ����������Ϊ��ͨio��ֱ�ӿ��Ƶ�ƽ����������ת
		}
		if( Reve_Data[3] == 'a')
		{
			__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3,	((int)Reve_Data[4]-48)*100); //���ݽ��յ�����ת�������Ϳ���PWM
			
			HAL_GPIO_WritePin(motor2_GPIO_Port,motor2_Pin,GPIO_PIN_SET);//�������һ����������Ϊ��ͨio��ֱ�ӿ��Ƶ�ƽ����������ת
		}
	
		if(Reve_Data[1] != 'g'  &&  Reve_Data[1] != 'a')
		{ 
			//���������ֹͣת��
			__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3, 0); 
			
			HAL_GPIO_WritePin(motor1_GPIO_Port,motor1_Pin,GPIO_PIN_RESET);
			
			__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, 0);
			
			HAL_GPIO_WritePin(motor2_GPIO_Port,motor2_Pin,GPIO_PIN_RESET);
		}
		Reve_Order = ' ';
		
		Reve_Data[1] = ' ';
		
		Motor_control_power = Power_in_serial_port; //�ѵ�����Ƶ�Ȩ�����ӵ����ڿ�����
		
		printf("set MOTOR ok  \r\n");
	}
	
	else if(Reve_Order =='4')  //�����ڿ��Ʒ�����������
	{
		Beep_flag =! Beep_flag;
		
		if(Beep_flag)
		{
			__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, 120); //���÷�����������������PWM��ռ�ձȣ������ʼ��ռ�ձ�Ϊ0
		}
		else
		{
			__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, 0); //���÷�����������������PWM��ռ�ձȣ������ʼ��ռ�ձ�Ϊ0
		}
		
		Reve_Order = ' ';
		
		printf("set BEEP ok  \r\n");
	}
	
	else if(Reve_Order =='5')  //�����ڿ��������豸����
	{
		
		__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, ((int)Reve_Data[1]-48)*100); //���������ƹ�����ȡ������ʼ��ռ�ձ�Ϊ0
		
		Reve_Order = ' ';
		
		printf("set IO ok : %d	\r\n",((int)Reve_Data[1]-48)*100);
	}
	else if(Reve_Order =='6')  //�����ڿ��������豸����
	{
		sTime.Hours 	= ((int)Reve_Data[1]-48)*10+ ((int)Reve_Data[2]-48);
		
		sTime.Minutes =	((int)Reve_Data[3]-48)*10+ ((int)Reve_Data[4]-48);
		
		sTime.Seconds = ((int)Reve_Data[5]-48)*10+ ((int)Reve_Data[6]-48);
		
		if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN) != HAL_OK)
		{
			Error_Handler();
		}
		Reve_Order = ' ';
		
		printf("set time ok : %d : %d : %d \r\n",	sTime.Hours,	sTime.Minutes,	sTime.Seconds);
	}
	else if(Reve_Order =='7')  //�����ڿ��������豸����
	{
		switch(Reve_Data[1])
		{
			case('1'):	weather = weather_sunny;				printf("set time weather : sunny		\r\n");				break;
			case('2'):	weather = weather_snow;					printf("set time weather : snow			\r\n");				break;
			case('3'):	weather = weather_rain;					printf("set time weather : rain			\r\n");				break;
			case('4'):	weather = weather_cloudy;				printf("set time weather : cloudy		\r\n");				break;
			case('5'):	weather = weather_night_rain;		printf("set time weather : night_rain\r\n");			break;
			case('6'):	weather = weather_night_sunny;	printf("set time weather : night_sunny\r\n");			break;
			default:	{	weather = weather_sunny;printf("not this wwather!!		\r\n");	}
		}
		Reve_Order = ' ';
	}
}














#include "control.h"

uint8_t Connection_status = off_line;

uint8_t Motor_control_power  = Power_in_serial_port; //初始状态令电机的控制权力在串口上

uint8_t connect_24G_flag = 0;

float temperature = 0.0;
	
float humidity = 0.0;

float pitch,roll,yaw; 		//欧拉角

uint8_t RED_PEO;

float Light_intensity = 0; //光照强度

RTC_TimeTypeDef sTime = {0};

RTC_DateTypeDef sDate = {0};
	
/*******************************************
* @function: 	使用STM32板载ADC获取电池电压值，将获取到的电压值储存在全局变量中
* @input:			无
* @return:		输出成为全局变量直接使用
* @author:		田小花
* @time:			2020.9.1
*******************************************
*/
void Read_sensor(void)
{
	uint8_t SHT30_reg_dat[6] = {0};
	
	HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN); //获取时间
	//printf("%d时%d分%d秒\r\n",sTime.Hours,	sTime.Minutes,	sTime.Seconds);
	
	while(mpu_dmp_get_data(&pitch,&roll,&yaw)!=0);

	if(SHT30_Read_Dat(SHT30_reg_dat) == HAL_OK)
	{
		SHT30_Dat_To_Float(SHT30_reg_dat, &temperature, &humidity);
	}
	
	RED_PEO = HAL_GPIO_ReadPin(RED_PEO_IOIN_GPIO_Port,RED_PEO_IOIN_Pin);
	
	Light_intensity = Obtain_temt6000_voltage();
}


/*******************************************
* @function: 	使用STM32板载ADC获取电池电压值，将获取到的电压值储存在全局变量中
* @input:			无
* @return:		输出成为全局变量直接使用
* @author:		田小花
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
		tmp_buf[32]=0;//加入字符串结束符
		
		connect_24G_flag = 0; 
		
		if(tmp_buf[0] == tmp_buf[1]	&& tmp_buf[2] == tmp_buf[1])
		{
			printf("connect error	connect error	connect error	connect error	connect error	connect error	\r\n");
		}
		else
		{
			if(tmp_buf[2]<=13) //前进
			{
				Motor_control_power = Power_in_24G_port;	//设定电机的控制权力在2.4G通讯上
				
				if(tmp_buf[1]>=19) //左转
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
				if(tmp_buf[1]<=13) //右转
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
				if(tmp_buf[1]>13 && tmp_buf[1]<19) //无左右转
				{
						PWM_motor_1 = (13 - tmp_buf[2]);
						
						PWM_motor_2 = (13 - tmp_buf[2]);
				}
				__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1,	400 - PWM_motor_1 * 30); //根据接收的数据转换成整型控制PWM
				
					HAL_GPIO_WritePin(motor1_GPIO_Port,motor1_Pin,GPIO_PIN_SET); //电机的另一个引脚设置为普通io，直接控制电平来控制正反转

				__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3,	400 - PWM_motor_2 *30); //根据接收的数据转换成整型控制PWM
				
				HAL_GPIO_WritePin(motor2_GPIO_Port,motor2_Pin,GPIO_PIN_SET);//电机的另一个引脚设置为普通io，直接控制电平来控制正反转
			}
			if(tmp_buf[2]>=19) //后退
			{
				if(tmp_buf[1]>=19) //左转
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
				if(tmp_buf[1]<=13) //右转
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
				if(tmp_buf[1]>13 && tmp_buf[1]<19) //无左右转
				{
						PWM_motor_1 = (tmp_buf[2]-19);
						
						PWM_motor_2 = (tmp_buf[2]-19);
				}
				__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1,	200+PWM_motor_1*38); //根据接收的数据转换成整型控制PWM
					
					HAL_GPIO_WritePin(motor1_GPIO_Port,motor1_Pin,GPIO_PIN_RESET); //电机的另一个引脚设置为普通io，直接控制电平来控制正反转

				__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3,	200+PWM_motor_2*38); //根据接收的数据转换成整型控制PWM		
				
				HAL_GPIO_WritePin(motor2_GPIO_Port,motor2_Pin,GPIO_PIN_RESET);//电机的另一个引脚设置为普通io，直接控制电平来控制正反转			
			}
		  if(tmp_buf[2]>13 && tmp_buf[2]<19)
			{
				
				if(tmp_buf[1] == 32)
				{
					__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1,	0); //根据接收的数据转换成整型控制PWM
			
					HAL_GPIO_WritePin(motor1_GPIO_Port,motor1_Pin,GPIO_PIN_SET); //电机的另一个引脚设置为普通io，直接控制电平来控制正反转

					__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3,	1000); //根据接收的数据转换成整型控制PWM
					
					HAL_GPIO_WritePin(motor2_GPIO_Port,motor2_Pin,GPIO_PIN_RESET);//电机的另一个引脚设置为普通io，直接控制电平来控制正反转
				}
				if(tmp_buf[1] == 0)
				{
					__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1,	1000); //根据接收的数据转换成整型控制PWM
			
					HAL_GPIO_WritePin(motor1_GPIO_Port,motor1_Pin,GPIO_PIN_RESET); //电机的另一个引脚设置为普通io，直接控制电平来控制正反转

					__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3,	0); //根据接收的数据转换成整型控制PWM
					
					HAL_GPIO_WritePin(motor2_GPIO_Port,motor2_Pin,GPIO_PIN_SET);//电机的另一个引脚设置为普通io，直接控制电平来控制正反转
				} 
				if(tmp_buf[1]>13 && tmp_buf[1]<19 && Motor_control_power  == Power_in_24G_port)
				{
					error:
					
					__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1,	0); //根据接收的数据转换成整型控制PWM
			
					HAL_GPIO_WritePin(motor1_GPIO_Port,motor1_Pin,GPIO_PIN_RESET); //电机的另一个引脚设置为普通io，直接控制电平来控制正反转

					__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3,	0); //根据接收的数据转换成整型控制PWM
					
					HAL_GPIO_WritePin(motor2_GPIO_Port,motor2_Pin,GPIO_PIN_RESET);//电机的另一个引脚设置为普通io，直接控制电平来控制正反转
				}
			}
		}
	}
	//对于2.4G在驱动电机的过程中突然断开并没有更好的处理和应对办法，2.4G在连接过程中也会出现没链接的情况
	else
	{
		if(Motor_control_power == Power_in_24G_port && connect_24G_flag	>11)
		{
			connect_24G_flag = 10;
			
			Motor_control_power = Power_in_serial_port;
			
			printf("已经断开！！\r\n");
			
			goto error;
		}
		connect_24G_flag++;
	}
}


/*******************************************
* @function: 	使用STM32板载ADC获取电池电压值，将获取到的电压值储存在全局变量中
* @input:			无
* @return:		输出成为全局变量直接使用
* @author:		田小花
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
	if(Reve_Order =='0') //处理串口控制LED部分
	{
		/*数据部分一共一个字节，
			0代表关闭LED灯；
			1代表开启LED灯；
		*/
		if(Reve_Data[1] == '0'){ HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET);}
		
		else if(Reve_Data[1] =='1') {	HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET);}
		
		Reve_Order = ' ';
		
		printf("set led ok  \r\n");
	}
		
	else if(Reve_Order =='1')  //处理串口控制显示屏部分
	{  			
		/*数据部分在0至255个字节范围内；
			将发送过来的数据直接显示在屏幕上。
		*/
		for(i=0;i<sizeof(Reve_Data);i++) //处理要显示的数据
		{
			if(Reve_Data[i]== '\0') //检测接收到的数据的结尾
			{
				Reve_Data[i-2] = Reve_Data[i]; //把结尾的'\0'提前到'\r'前面，防止显示的数据出现乱码
			}
		}
		Reve_Data[0] = '>'; //在显示的数据最前面加上'>'字符
			
			
		if(LCD_Number_of_rows>48)
		{
			LCD_Number_of_rows = 0;
		}	
		LCD_Fill(0,	 LCD_Number_of_rows,		LCD_W,	 LCD_Number_of_rows+16,	BLACK);//绘制电池内部的方块	
		
		LCD_ShowString(0, LCD_Number_of_rows, Reve_Data, YELLOW); //把数据显示到屏幕上
		
		LCD_Number_of_rows = LCD_Number_of_rows+16;
		
		Reve_Order = ' '; //把命令控制数据清除
		
		printf("set OLED ok   \r\n");
	}
		
	else if(Reve_Order =='2')  //处理串口控制舵机部分
	{		
		/*接收的数据为a至p号舵机及其角度，每次可以控制多个舵机
			把接收到的数据全部循环，当检测到字母的时候自动整理字母紧跟着的三位数据；
			这三位数据就是舵机的角度，控制舵机的角度的数据需要为001、090、180这种格式，长度必须是三位，长度不够需要用0代替；
		*/
		for(i=1;i<sizeof(Reve_Data);i++)
		{
			assic_transformation = (int)Reve_Data[i]; //把接收到的字符型数据转换为整型数据
			
			if( 96<assic_transformation && assic_transformation<113 ) //判断这个数据是不是在a至p范围内的字母，如果是，开始则转换后面的角度数据
			{
				value_PWM = ((int)Reve_Data[i+1]-48)*100+ ((int)Reve_Data[i+2]-48)*10+ (int)Reve_Data[i+3]-48;//把字符型角度数据转换成整型
					
				if(value_PWM>= 180)value_PWM = 179; //对角度大小进行控制，放置超出范围
				
				else if (value_PWM <= 1) value_PWM = 1; 
				
				printf("set PWM ok %c  value_PWM: %d  \r\n",assic_transformation ,value_PWM);
				
				switch (assic_transformation) //根据字母代表的不同的舵机来设置不同的角度
				{
					case 'a':PCA_MG9XX(0, value_PWM); break;//通道0 转动value_PWM大小的角度
					case 'b':PCA_MG9XX(1, value_PWM); break;//通道1 转动value_PWM大小的角度
					case 'c':PCA_MG9XX(2, value_PWM); break;//通道2 转动value_PWM大小的角度
					case 'd':PCA_MG9XX(3, value_PWM); break;//通道3 转动value_PWM大小的角度
					case 'e':PCA_MG9XX(4, value_PWM); break;//通道4 转动value_PWM大小的角度
					case 'f':PCA_MG9XX(5, value_PWM); break;//通道5 转动value_PWM大小的角度
					case 'g':PCA_MG9XX(6, value_PWM); break;//通道6 转动value_PWM大小的角度
					case 'h':PCA_MG9XX(7, value_PWM); break;//通道7 转动value_PWM大小的角度
					case 'i':PCA_MG9XX(8, value_PWM); break;//通道8 转动value_PWM大小的角度
					case 'j':PCA_MG9XX(9, value_PWM); break;//通道9 转动value_PWM大小的角度
					case 'k':PCA_MG9XX(10,value_PWM); break;//通道10 转动value_PWM大小的角度
					case 'l':PCA_MG9XX(11,value_PWM); break;//通道11 转动value_PWM大小的角度
					case 'm':PCA_MG9XX(12,value_PWM); break;//通道12 转动value_PWM大小的角度
					case 'n':PCA_MG9XX(13,value_PWM); break;//通道13 转动value_PWM大小的角度
					case 'o':PCA_MG9XX(14,value_PWM); break;//通道14 转动value_PWM大小的角度
					case 'p':PCA_MG9XX(15,value_PWM); break;//通道15 转动value_PWM大小的角度
				}
//				assic_transformation = 0; //把得到的字母清除
			}
			if(Reve_Data[i] == '\n')break; //如果数据结束，自动跳出for循环
		}
		Reve_Order = ' ';
	}
	else if(Reve_Order =='3')  //处理串口控制底盘的两个电机
	{
		/*数据部分最多4个字节
			接收到的数据分为两个电机的控制，每个电机的控制分为前后和角度
			比如数据为g0a9 默认前两个字节控制第一个电机，后两个字节控制另一个
			g是go也就是前进，速度为0，则停止转动，a是after是后退，速度为9，以最高速转动
			如果输入的数据不符合格式，两个电机都不转动
			__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1,	PWM_Motor1); //根据接收的数据转换成整型控制PWM
			HAL_GPIO_WritePin(motor1_GPIO_Port,motor1_Pin,GPIO_PIN_SET); //电机的另一个引脚设置为普通io，直接控制电平来控制正反转

			__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3,	PWM_Motor2); //根据接收的数据转换成整型控制PWM
			HAL_GPIO_WritePin(motor2_GPIO_Port,motor2_Pin,GPIO_PIN_SET);//电机的另一个引脚设置为普通io，直接控制电平来控制正反转
		*/
		if( Reve_Data[1] == 'g')
		{	
			__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1,	1000-((int)Reve_Data[2]-48)*100); //根据接收的数据转换成整型控制PWM
			
			HAL_GPIO_WritePin(motor1_GPIO_Port,motor1_Pin,GPIO_PIN_SET); //电机的另一个引脚设置为普通io，直接控制电平来控制正反转
		}
		if( Reve_Data[1] == 'a')
		{
			__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1,	((int)Reve_Data[2]-48)*100); //根据接收的数据转换成整型控制PWM
			
			HAL_GPIO_WritePin(motor1_GPIO_Port,motor1_Pin,GPIO_PIN_RESET); //电机的另一个引脚设置为普通io，直接控制电平来控制正反转
		}
		if( Reve_Data[3] == 'g')
		{
			__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3,	1000-((int)Reve_Data[4]-48)*100); //根据接收的数据转换成整型控制PWM
			
			HAL_GPIO_WritePin(motor2_GPIO_Port,motor2_Pin,GPIO_PIN_SET);//电机的另一个引脚设置为普通io，直接控制电平来控制正反转
		}
		if( Reve_Data[3] == 'a')
		{
			__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3,	((int)Reve_Data[4]-48)*100); //根据接收的数据转换成整型控制PWM
			
			HAL_GPIO_WritePin(motor2_GPIO_Port,motor2_Pin,GPIO_PIN_SET);//电机的另一个引脚设置为普通io，直接控制电平来控制正反转
		}
	
		if(Reve_Data[1] != 'g'  &&  Reve_Data[1] != 'a')
		{ 
			//两个电机都停止转动
			__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3, 0); 
			
			HAL_GPIO_WritePin(motor1_GPIO_Port,motor1_Pin,GPIO_PIN_RESET);
			
			__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, 0);
			
			HAL_GPIO_WritePin(motor2_GPIO_Port,motor2_Pin,GPIO_PIN_RESET);
		}
		Reve_Order = ' ';
		
		Reve_Data[1] = ' ';
		
		Motor_control_power = Power_in_serial_port; //把电机控制的权力交接到串口控制上
		
		printf("set MOTOR ok  \r\n");
	}
	
	else if(Reve_Order =='4')  //处理串口控制蜂鸣器的鸣叫
	{
		Beep_flag =! Beep_flag;
		
		if(Beep_flag)
		{
			__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, 120); //设置蜂鸣器的声音。设置PWM的占空比，这里初始化占空比为0
		}
		else
		{
			__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, 0); //设置蜂鸣器的声音。设置PWM的占空比，这里初始化占空比为0
		}
		
		Reve_Order = ' ';
		
		printf("set BEEP ok  \r\n");
	}
	
	else if(Reve_Order =='5')  //处理串口控制外设设备开关
	{
		
		__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, ((int)Reve_Data[1]-48)*100); //设置照明灯光的亮度。这里初始化占空比为0
		
		Reve_Order = ' ';
		
		printf("set IO ok : %d	\r\n",((int)Reve_Data[1]-48)*100);
	}
	else if(Reve_Order =='6')  //处理串口控制外设设备开关
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
	else if(Reve_Order =='7')  //处理串口控制外设设备开关
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














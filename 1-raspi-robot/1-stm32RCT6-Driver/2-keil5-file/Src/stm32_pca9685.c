#include "stm32_pca9685.h"
#include "math.h"
#include "i2c.h"

uint8_t pca_read(uint8_t startAddress) {
    //Send address to start reading from.
    uint8_t tx[1];
    uint8_t buffer[1];
    tx[0]=startAddress;

    HAL_I2C_Master_Transmit(&hi2c1,pca_adrr, tx,1,10000);
    HAL_I2C_Master_Receive(&hi2c1,pca_adrr,buffer,1,10000);
    return buffer[0];
}

void pca_write(uint8_t startAddress, uint8_t buffer) {
    //Send address to start reading from.
    uint8_t tx[2];
    tx[0]=startAddress;
    tx[1]=buffer;
    HAL_I2C_Master_Transmit(&hi2c1,pca_adrr, tx,2,10000);

}

void pca_setfreq(float freq)//设置PWM频率
{
		uint8_t prescale,oldmode,newmode;
		double prescaleval;
		freq *= 0.92; 
		prescaleval = 25000000;
		prescaleval /= 4096;
		prescaleval /= freq;
		prescaleval -= 1;
		prescale =floor(prescaleval + 0.5f);

		oldmode = pca_read(pca_mode1);
	
		newmode = (oldmode&0x7F) | 0x10; // sleep
	
		pca_write(pca_mode1, newmode); // go to sleep
	
		pca_write(pca_pre, prescale); // set the prescaler
	
		pca_write(pca_mode1, oldmode);
		HAL_Delay(2);
	
		pca_write(pca_mode1, oldmode | 0xa1); 
}

void pca_setpwm(uint8_t num, uint32_t on, uint32_t off)
{
		pca_write(LED0_ON_L+4*num,on);
		pca_write(LED0_ON_H+4*num,on>>8);
		pca_write(LED0_OFF_L+4*num,off);
		pca_write(LED0_OFF_H+4*num,off>>8);
}
/*num:舵机PWM输出引脚0~15，on:PWM上升计数值0~4096,off:PWM下降计数值0~4096
一个PWM周期分成4096份，由0开始+1计数，计到on时跳变为高电平，继续计数到off时
跳变为低电平，直到计满4096重新开始。所以当on不等于0时可作延时,当on等于0时，
off/4096的值就是PWM的占空比。*/

/*
	函数作用：初始化舵机驱动板
	参数：1.PWM频率
		  2.初始化舵机角度
*/
void PCA_MG9XX_Init(float hz,uint8_t angle)
{
	uint32_t off=0;
//	IIC_Init();
	pca_write(pca_mode1,0x0);
	pca_setfreq(hz);//设置PWM频率
	off=(uint32_t)(145+angle*2.4);
	pca_setpwm(0,0,off);pca_setpwm(1,0,off);pca_setpwm(2,0,off);pca_setpwm(3,0,off);
	pca_setpwm(4,0,off);pca_setpwm(5,0,off);pca_setpwm(6,0,off);pca_setpwm(7,0,off);
	pca_setpwm(8,0,off);pca_setpwm(9,0,off);pca_setpwm(10,0,off);pca_setpwm(11,0,off);
	pca_setpwm(12,0,off);pca_setpwm(13,0,off);pca_setpwm(14,0,off);pca_setpwm(15,0,off);
	HAL_Delay(500);
}

/*
	函数作用：控制舵机转动；
	参数：1.输出端口，可选0~15；
		  2.起始角度，可选0~180；
		  3.结束角度，可选0~180；
		  4.模式选择，0 表示函数内无延时，调用时需要在函数后另外加延时函数，且不可调速，第五个参数可填任意值；
					  1 表示函数内有延时，调用时不需要在函数后另外加延时函数，且不可调速，第五个参数可填任意值；
					  2 表示速度可调，第五个参数表示速度值；
		  5.速度，可填大于 0 的任意值，填 1 时速度最快，数值越大，速度越小；
	注意事项：模式 0和1 的速度比模式 2 的最大速度大；
*/
void PCA_MG9XX(uint8_t num,uint8_t end_angle)
{
	uint32_t off=0;
	
	off=(uint32_t)(158+end_angle*2.2);
	pca_setpwm(num,0,off);
}










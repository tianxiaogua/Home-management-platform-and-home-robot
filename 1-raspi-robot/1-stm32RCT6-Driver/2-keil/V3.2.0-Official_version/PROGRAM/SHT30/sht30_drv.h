#ifndef __SHT30_DRV_H
#define __SHT30_DRV_H

#include "main.h"

/* ADDR Pin Conect to VSS */

#define	SHT30_ADDR_WRITE	0x44<<1         //10001000
#define	SHT30_ADDR_READ		(0x44<<1)+1	    //10001011

typedef enum
{
    /* �����λ���� */

    SOFT_RESET_CMD = 0x30A2,	
    /*
    ���β���ģʽ
    ������ʽ��Repeatability_CS_CMD
    CS�� Clock stretching
    */
    HIGH_ENABLED_CMD    = 0x2C06,
    MEDIUM_ENABLED_CMD  = 0x2C0D,
    LOW_ENABLED_CMD     = 0x2C10,
    HIGH_DISABLED_CMD   = 0x2400,
    MEDIUM_DISABLED_CMD = 0x240B,
    LOW_DISABLED_CMD    = 0x2416,

    /*
    ���ڲ���ģʽ
    ������ʽ��Repeatability_MPS_CMD
    MPS��measurement per second
    */
    HIGH_0_5_CMD   = 0x2032,
    MEDIUM_0_5_CMD = 0x2024,
    LOW_0_5_CMD    = 0x202F,
    HIGH_1_CMD     = 0x2130,
    MEDIUM_1_CMD   = 0x2126,
    LOW_1_CMD      = 0x212D,
    HIGH_2_CMD     = 0x2236,
    MEDIUM_2_CMD   = 0x2220,
    LOW_2_CMD      = 0x222B,
    HIGH_4_CMD     = 0x2334,
    MEDIUM_4_CMD   = 0x2322,
    LOW_4_CMD      = 0x2329,
    HIGH_10_CMD    = 0x2737,
    MEDIUM_10_CMD  = 0x2721,
    LOW_10_CMD     = 0x272A,
	/* ���ڲ���ģʽ��ȡ�������� */
	READOUT_FOR_PERIODIC_MODE = 0xE000,
} SHT30_CMD;



#define CRC8_POLYNOMIAL 0x31



/**
 * @brief	��SHT30���յ�6���ֽ����ݽ���CRCУ�飬��ת��Ϊ�¶�ֵ��ʪ��ֵ
 * @param	dat  ���� �洢�������ݵĵ�ַ��6���ֽ����飩
 * @retval	У��ɹ�  ���� ����0
 * 			У��ʧ��  ���� ����1���������¶�ֵ��ʪ��ֵΪ0
*/
uint8_t SHT30_Dat_To_Float(uint8_t* const dat, float* temperature, float* humidity);
	


/**
�������ֲ��п�֪��SHT30�ֱ����¶����ݺ�ʪ������֮������8-CRCУ���룬ȷ�������ݿɿ��ԡ�
CRC-8У���������
*/
uint8_t CheckCrc8(uint8_t* const message, uint8_t initial_value);




/**
 * @brief	��SHT30��ȡһ������
 * @param	dat ���� �洢��ȡ���ݵĵ�ַ��6���ֽ����飩
 * @retval	�ɹ� ���� ����HAL_OK
*/
uint8_t SHT30_Read_Dat(uint8_t* dat);



/**
 * @brief	��ʼ��SHT30
 * @param	none
 * @retval	�ɹ�����HAL_OK
 * @note	���ڲ���ģʽ
*/
uint8_t SHT30_Init(void);



/**
 * @brief	��λSHT30
 * @param	none
 * @retval	none
*/
void SHT30_Reset(void);



/**
 * @brief	��SHT30����һ��ָ��(16bit)
 * @param	cmd ���� SHT30ָ���SHT30_MODE��ö�ٶ��壩
 * @retval	�ɹ�����HAL_OK
*/
static uint8_t	SHT30_Send_Cmd(SHT30_CMD cmd);
	

#endif

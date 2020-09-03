#include "mpu6050Driver.h"

static signed char gyro_orientation[9] = { 1, 0, 0,
                                           0, 1, 0,
                                           0, 0, 1};

																					 void MPU_6050_Init(void)
{
	HAL_StatusTypeDef status;
	HAL_I2C_StateTypeDef flag;
	unsigned char pdata;
	//����豸�Ƿ�׼����  ��ַ   ��� ���� ��ʱʱ��  
	status=HAL_I2C_IsDeviceReady(&hi2c2, ADDRESS_W, 10, HAL_MAX_DELAY);
	printf("status is %d\n",status);
	//��������Ƿ�׼����
	flag=HAL_I2C_GetState(&hi2c2);
	printf("flag %X \n",flag);
	//����д�Ĵ�������
	pdata=0x80; //��λMPU
	HAL_I2C_Mem_Write(&hi2c2, ADDRESS_W, MPU_PWR_MGMT1_REG, 1, &pdata, 1, HAL_MAX_DELAY); //д0x80��λ
	status=HAL_I2C_IsDeviceReady(&hi2c2, ADDRESS_W, 10, HAL_MAX_DELAY);
	printf("status is %d\n",status);
	
	HAL_Delay(500);  //��λ����Ҫ�ȴ�һ��ʱ�� �ȴ�оƬ��λ��� 
	
	//��������Ƿ�׼����
	flag=HAL_I2C_GetState(&hi2c2);
	printf("flag %X \n",flag);
	//����MPU
	pdata=0x01; // 7λ 1 ����  6λ˯��ģʽ1 ˯�� 2 ����   3λ Ϊu�㶮������0 ����    0-2λ ʱ��ѡ��  01 PLLʹ��XZ��������Ϊ����
	HAL_I2C_Mem_Write(&hi2c2, ADDRESS_W, MPU_PWR_MGMT1_REG, 1, &pdata, 1, HAL_MAX_DELAY); //д0x80��λ
	//�������� �Ĵ���  2000  3
	pdata=3<<3; //����3 Ϊ����2000  ����3λ ����Ӧ�� 3 4 λ�Ĵ�����
	HAL_I2C_Mem_Write(&hi2c2, ADDRESS_W, MPU_GYRO_CFG_REG, 1, &pdata, 1, HAL_MAX_DELAY); 
	flag=HAL_I2C_GetState(&hi2c2);
	printf("flag %X \n",flag);
	//���ü��ٶȴ��������� 2g
	pdata=0; 
	HAL_I2C_Mem_Write(&hi2c2, ADDRESS_W, MPU_ACCEL_CFG_REG, 1, &pdata, 1, HAL_MAX_DELAY); 
	flag=HAL_I2C_GetState(&hi2c2);
	printf("flag %X \n",flag);
	//�����ǲ�����Ƶ����
	pdata=19; //1000/50-1  �������Ҫ�����ϲ鿴  ԭ�� �ͼ��㷽��
	HAL_I2C_Mem_Write(&hi2c2, ADDRESS_W, MPU_SAMPLE_RATE_REG, 1, &pdata, 1, HAL_MAX_DELAY); 
	flag=HAL_I2C_GetState(&hi2c2);
	printf("flag %X \n",flag);
	//�ر������ж�
	pdata=0;
	HAL_I2C_Mem_Write(&hi2c2, ADDRESS_W, MPU_INT_EN_REG, 1, &pdata, 1, HAL_MAX_DELAY); 
	//I2C ��ģʽ�� ��Ӵ����������ӿڹر�
	flag=HAL_I2C_GetState(&hi2c2);
	printf("flag %X \n",flag);
	pdata=0;
	HAL_I2C_Mem_Write(&hi2c2, ADDRESS_W, MPU_USER_CTRL_REG, 1, &pdata, 1, HAL_MAX_DELAY); 
	//�ر�FIFO 
	flag=HAL_I2C_GetState(&hi2c2);
	printf("flag %X \n",flag);
	pdata=0;
	HAL_I2C_Mem_Write(&hi2c2, ADDRESS_W, MPU_FIFO_EN_REG, 1, &pdata, 1, HAL_MAX_DELAY); 
	//�ж���·���� �͵�ƽ��Ч
	flag=HAL_I2C_GetState(&hi2c2);
	printf("flag %X \n",flag);
	pdata=0X80;
	HAL_I2C_Mem_Write(&hi2c2, ADDRESS_W, MPU_INTBP_CFG_REG, 1, &pdata, 1, HAL_MAX_DELAY); 
	//���õ�ͨ�˲���
	MPU_Set_LPF(50/2);
	//������ID  Ĭ���� 0x68 
	flag=HAL_I2C_GetState(&hi2c2);
	printf("flag %X \n",flag);
	pdata=MPU_DEVICE_ID_REG;
	HAL_I2C_Mem_Read(&hi2c2, ADDRESS_R, MPU_DEVICE_ID_REG, 1, &pdata, 1, HAL_MAX_DELAY); 
	printf("ID is %X \n",pdata);
	//ʹ�� ������ �ͼ��ٶ� ����
	flag=HAL_I2C_GetState(&hi2c2);
	printf("flag %X \n",flag);
	pdata=0;
	HAL_I2C_Mem_Write(&hi2c2, ADDRESS_W, MPU_PWR_MGMT2_REG, 1, &pdata, 1, HAL_MAX_DELAY);
//�Ĵ���������Թۿ�
/*
	//��ȡ�Ĵ���1 
	flag=HAL_I2C_GetState(&hi2c1);
	printf("flag %X \n",flag);
	pdata=0;
	HAL_I2C_Mem_Read(&hi2c1, ADDRESS_R, MPU_PWR_MGMT1_REG, 1, &pdata, 1, HAL_MAX_DELAY); 
	printf("0x6B REG1 %02X \n",pdata);
	//��ȡ�¶ȴ�����
	HAL_Delay(500);
	flag=HAL_I2C_GetState(&hi2c1);
	printf("flag %X \n",flag);
	pdata=0xff;
	HAL_I2C_Mem_Read(&hi2c1, ADDRESS_R, MPU_TEMP_OUTH_REG, 1, &pdata, 1, HAL_MAX_DELAY); 
	printf("hight bit %02X \n",pdata);
	HAL_Delay(500);
	flag=HAL_I2C_GetState(&hi2c1);
	printf("flag %X \n",flag);
	pdata=0xff;
	HAL_I2C_Mem_Read(&hi2c1, ADDRESS_R, MPU_TEMP_OUTH_REG+1, 1, &pdata, 1, HAL_MAX_DELAY); 
	printf("low bit %02X \n",pdata);
*/
}
//����MPU6050�����ֵ�ͨ�˲���
//lpf:���ֵ�ͨ�˲�Ƶ��(Hz)
//����ֵ:0,���óɹ�
//    ����,����ʧ�� 
unsigned char  MPU_Set_LPF(unsigned short lpf)
{
	unsigned char data=0;
	if(lpf>=188)data=1;
	else if(lpf>=98)data=2;
	else if(lpf>=42)data=3;
	else if(lpf>=20)data=4;
	else if(lpf>=10)data=5;
	else data=6; 
	
	return HAL_I2C_Mem_Write(&hi2c2, ADDRESS_W, MPU_CFG_REG, 1, &data, 1, HAL_MAX_DELAY); 
		//MPU_Write_Byte(MPU_CFG_REG,data);//�������ֵ�ͨ�˲���  
}

//��ȡ �¶ȴ�����   ������  ���ٶ�ԭʼ����
void read_all(void)
{
	HAL_StatusTypeDef status;
	HAL_I2C_StateTypeDef flag;
	unsigned char tem[2]={0};
	unsigned char  accbuff[6]={0};  //MPU_ACCEL_XOUTH_REG ���ٶ� 
	unsigned char tuoluo[6]={0};
	short aacx,aacy,aacz; //���ٶ�
	short tolx,toly,tolz;
	 short raw;
	float temp;
	short  lingmin=16384;//������ 16384 
	float tuol=16.4f;
	flag=HAL_I2C_GetState(&hi2c2);
	printf("flag %X \n",flag);
	status=HAL_I2C_Mem_Read(&hi2c2, ADDRESS_W, MPU_TEMP_OUTH_REG, 1, tem, 2, HAL_MAX_DELAY); 
	printf("status %02X \n",status);
	raw=((unsigned short)tem[0]<<8)|tem[1];  
	printf("%d %d \n",tem[0],tem[1]);
	temp=36.53+((double)raw)/340;  
	printf("%3.3f C\n",temp);
	//��ȡ���ٶ�3��ߵ�λ����
	status=HAL_I2C_Mem_Read(&hi2c2, ADDRESS_W, MPU_ACCEL_XOUTH_REG, 1, accbuff, 6, HAL_MAX_DELAY); 
	aacx=((short)(accbuff[0]<<8))|accbuff[1];
	aacy=((short)(accbuff[2]<<8))|accbuff[3];
	aacz=((short)(accbuff[4]<<8))|accbuff[5];
	
	printf("acc is  x %d y %d z %d   x%.1f g,y%.1f g,z%.1f g\n",aacx,aacy,aacz,
																																	(float)(aacx)/lingmin,
																																	(float)(aacy)/lingmin,
																																	(float)(aacz)/lingmin																																		
																																	);
	//��ȡ������3��  MPU_GYRO_XOUTH_REG
	status=HAL_I2C_Mem_Read(&hi2c2, ADDRESS_W, MPU_GYRO_XOUTH_REG, 1, tuoluo, 6, HAL_MAX_DELAY); 
	tolx=((unsigned short)(tuoluo[0]<<8))|tuoluo[1];
	toly=((unsigned short)(tuoluo[2]<<8))|tuoluo[3];
	tolz=((unsigned short)(tuoluo[4]<<8))|tuoluo[5];		
	printf("tuoluo is  x %d y %d z %d   x%.1f ,y%.1f ,z%.1f \n",tolx,toly,tolz,
																																	(float)(tolx)/tuol,
																																	(float)(toly)/tuol,
																																	(float)(tolz)/tuol																																		
																																	);																																
} 
//�����ں�DMP��ĺ���  
unsigned char HAL_i2c_write(unsigned char slave_addr, unsigned char reg_addr, unsigned char length, unsigned char const *data)
{
	//printf("flag %X \n",HAL_I2C_GetState(&hi2c1));
	HAL_I2C_GetState(&hi2c2);
	//DMP������  slave_addr ��ַ��0x68 ����ʱ��Ҫ����1λ���һλ���϶�дλ
	HAL_I2C_Mem_Write(&hi2c2, ((slave_addr<<1)|0), reg_addr, 1, (unsigned char *)data, length, HAL_MAX_DELAY);
	return 0;
}
unsigned char HAL_i2c_read(unsigned char slave_addr, unsigned char reg_addr, unsigned char length, unsigned char const *data)
{
	//printf("flag %X \n",HAL_I2C_GetState(&hi2c1));
	HAL_I2C_GetState(&hi2c2);
	//DMP������  slave_addr ��ַ��0x68 ����ʱ��Ҫ����1λ���һλ���϶�дλ
	HAL_I2C_Mem_Read(&hi2c2, ((slave_addr<<1)|1), reg_addr, 1, (unsigned char *)data, length, HAL_MAX_DELAY);
	return 0;
}
//mpu6050,dmp��ʼ��
//����ֵ:0,����
//    ����,ʧ��
unsigned char mpu_dmp_init(void)
{
	unsigned char res=0;
	//IIC_Init(); 		//��ʼ��IIC����
	if(mpu_init()==0)	//��ʼ��MPU6050
	{	 
		res=mpu_set_sensors(INV_XYZ_GYRO|INV_XYZ_ACCEL);//��������Ҫ�Ĵ�����
		if(res)return 1; 
		res=mpu_configure_fifo(INV_XYZ_GYRO | INV_XYZ_ACCEL);//����FIFO
		if(res)return 2; 
		res=mpu_set_sample_rate(DEFAULT_MPU_HZ);	//���ò�����
		if(res)return 3; 
		res=dmp_load_motion_driver_firmware();		//����dmp�̼�
		if(res)return 4; 
		res=dmp_set_orientation(inv_orientation_matrix_to_scalar(gyro_orientation));//���������Ƿ���
		if(res)return 5; 
		res=dmp_enable_feature(DMP_FEATURE_6X_LP_QUAT|DMP_FEATURE_TAP|	//����dmp����
		    DMP_FEATURE_ANDROID_ORIENT|DMP_FEATURE_SEND_RAW_ACCEL|DMP_FEATURE_SEND_CAL_GYRO|
		    DMP_FEATURE_GYRO_CAL);
		if(res)return 6; 
		res=dmp_set_fifo_rate(DEFAULT_MPU_HZ);	//����DMP�������(��󲻳���200Hz)
		if(res)return 7;   
		res=run_self_test();		//�Լ�
		if(res)return 8;    
		res=mpu_set_dmp_state(1);	//ʹ��DMP
		if(res)return 9;     
	}
	return 0;
}
//�����Ƿ������
unsigned short inv_orientation_matrix_to_scalar(
    const signed char *mtx)
{
    unsigned short scalar; 
    /*
       XYZ  010_001_000 Identity Matrix
       XZY  001_010_000
       YXZ  010_000_001
       YZX  000_010_001
       ZXY  001_000_010
       ZYX  000_001_010
     */

    scalar = inv_row_2_scale(mtx);
    scalar |= inv_row_2_scale(mtx + 3) << 3;
    scalar |= inv_row_2_scale(mtx + 6) << 6;


    return scalar;
}
//����ת��
unsigned short inv_row_2_scale(const signed char *row)
{
    unsigned short b;

    if (row[0] > 0)
        b = 0;
    else if (row[0] < 0)
        b = 4;
    else if (row[1] > 0)
        b = 1;
    else if (row[1] < 0)
        b = 5;
    else if (row[2] > 0)
        b = 2;
    else if (row[2] < 0)
        b = 6;
    else
        b = 7;      // error
    return b;
}
//MPU6050�Բ���
//����ֵ:0,����
//    ����,ʧ��
unsigned char run_self_test(void)
{
	int result;
	//char test_packet[4] = {0};
	long gyro[3], accel[3]; 
	result = mpu_run_self_test(gyro, accel);
	if (result == 0x3) 
	{
		/* Test passed. We can trust the gyro data here, so let's push it down
		* to the DMP.
		*/
		float sens;
		unsigned short accel_sens;
		mpu_get_gyro_sens(&sens);
		gyro[0] = (long)(gyro[0] * sens);
		gyro[1] = (long)(gyro[1] * sens);
		gyro[2] = (long)(gyro[2] * sens);
		dmp_set_gyro_bias(gyro);
		mpu_get_accel_sens(&accel_sens);
		accel[0] *= accel_sens;
		accel[1] *= accel_sens;
		accel[2] *= accel_sens;
		dmp_set_accel_bias(accel);
		return 0;
	}else return 1;
}
//�õ�dmp����������(ע��,��������Ҫ�Ƚ϶��ջ,�ֲ������е��)
//pitch:������ ����:0.1��   ��Χ:-90.0�� <---> +90.0��
//roll:�����  ����:0.1��   ��Χ:-180.0��<---> +180.0��
//yaw:�����   ����:0.1��   ��Χ:-180.0��<---> +180.0��
//����ֵ:0,����
//    ����,ʧ��
unsigned char mpu_dmp_get_data(float *pitch,float *roll,float *yaw)
{
	float q0=1.0f,q1=0.0f,q2=0.0f,q3=0.0f;
	unsigned long sensor_timestamp;
	short gyro[3], accel[3], sensors;
	unsigned char more;
	long quat[4]; 
	if(dmp_read_fifo(gyro, accel, quat, &sensor_timestamp, &sensors,&more))return 1;	 
	/* Gyro and accel data are written to the FIFO by the DMP in chip frame and hardware units.
	 * This behavior is convenient because it keeps the gyro and accel outputs of dmp_read_fifo and mpu_read_fifo consistent.
	**/
	/*if (sensors & INV_XYZ_GYRO )
	send_packet(PACKET_TYPE_GYRO, gyro);
	if (sensors & INV_XYZ_ACCEL)
	send_packet(PACKET_TYPE_ACCEL, accel); */
	/* Unlike gyro and accel, quaternions are written to the FIFO in the body frame, q30.
	 * The orientation is set by the scalar passed to dmp_set_orientation during initialization. 
	**/
	if(sensors&INV_WXYZ_QUAT) 
	{
		q0 = quat[0] / q30;	//q30��ʽת��Ϊ������
		q1 = quat[1] / q30;
		q2 = quat[2] / q30;
		q3 = quat[3] / q30; 
		//����õ�������/�����/�����
		*pitch = asin(-2 * q1 * q3 + 2 * q0* q2)* 57.3;	// pitch
		*roll  = atan2(2 * q2 * q3 + 2 * q0 * q1, -2 * q1 * q1 - 2 * q2* q2 + 1)* 57.3;	// roll
		*yaw   = atan2(2*(q1*q2 + q0*q3),q0*q0+q1*q1-q2*q2-q3*q3) * 57.3;	//yaw
	}else return 2;
	return 0;
}



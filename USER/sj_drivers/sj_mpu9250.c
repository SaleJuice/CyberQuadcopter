//
//  File:	sj_mpu9250.c
//

#include "sj_mpu9250.h"

#define ACC_SENSITIVITY (16.0f / 32767.0f)
#define GYRO_SENSITIVITY (1000.0f / 32767.0f)

#define _IMU_DT_ 0.005 //采样周期（单位：s）

// 陀螺仪总结构体
_MPU9250DATA_ mpudata;
//角度信息
ANGLE Angle;
//俯仰角度信息
PITCH Pitch;
//偏航角度信息
YAW Yaw;
//翻滚角度信息
ROLL Roll;
//温度信息
int temp;

//滤波变量
Butter_Parameter Accel_Parameter;
Butter_Parameter Gyro_Parameter;

Butter_BufferData gyro_filter_buf_1[3],gyro_filter_buf_2[3];
Butter_BufferData acc_filter_buf_1[3],acc_filter_buf_2[3];

/*
   @func 初始化MPU9250
   调用该函数前，请先调用模拟IIC的初始化

   ACCEL_CONFIG
       0x00: 2g
       0x08: 4g
       0x10: 8g
       0x18: 16g
   GYRO_CONFIG:
       0x00: 250deg/s
       0x08: 500deg/s
       0x10: 1000deg/s
       0x18: 2000deg/s
*/
void MPU9250_Config(void)
{
  unsigned char i2cread = iic_read_reg(MPU9250_DEV_ADDR, WHO_AM_I, IIC);
	
  iic_write_reg(MPU9250_DEV_ADDR, PWR_MGMT_1, 0x00); // 解除休眠状态
  iic_write_reg(MPU9250_DEV_ADDR, SMPLRT_DIV, 0x07); // 采样率, 8kHz / (1 + SMPLRT_DIV)
  iic_write_reg(MPU9250_DEV_ADDR, MPU_CONFIG, 0x02);
	
	iic_write_reg(MPU9250_DEV_ADDR, GYRO_CONFIG, 0x10);
  iic_write_reg(MPU9250_DEV_ADDR, ACCEL_CONFIG, 0x18);
  iic_write_reg(MPU9250_DEV_ADDR, ACCEL_CONFIG2, 0x06); //加速度采样频率460HZ
	
  iic_write_reg(MPU9250_DEV_ADDR, USER_CTRL, 0x00);
  iic_write_reg(MPU9250_DEV_ADDR, INT_PIN_CFG, 0x02);
	
	Set_Cutoff_Frequency(200, 50,&Gyro_Parameter);//姿态角速度反馈滤波参数  50
  Set_Cutoff_Frequency(200, 30,&Accel_Parameter);//姿态解算加计修正滤波值 30
}

/*
  @func 陀螺仪角速度采集，单位转换，计算acc角，pitch角
*/
void GetGyroData(void)
{
  int dat[6];
  int x, y, z;

  iic_read_regs(MPU9250_DEV_ADDR, GYRO_XOUT_H, dat, 6, IIC);

  x = ((short)((unsigned short)dat[0] << 8 | dat[1]));
  y = ((short)((unsigned short)dat[2] << 8 | dat[3]));
  z = ((short)((unsigned short)dat[4] << 8 | dat[5]));  
  
	Angle.GyroReal.X = x+23;//补偿值
  Angle.GyroReal.Y = y-53;//补偿值
  Angle.GyroReal.Z = z-43;//补偿值
	
	Angle.GyroFilter1.X=LPButterworth(Angle.GyroReal.X,&gyro_filter_buf_1[0],&Bandstop_Filter_Parameter_30_98);
	Angle.GyroFilter1.Y=LPButterworth(Angle.GyroReal.Y,&gyro_filter_buf_1[1],&Bandstop_Filter_Parameter_30_98);
	Angle.GyroFilter1.Z=LPButterworth(Angle.GyroReal.Z,&gyro_filter_buf_1[2],&Bandstop_Filter_Parameter_30_98);
	
	Angle.GyroFilter2.X=LPButterworth(Angle.GyroFilter1.X,&gyro_filter_buf_2[0],&Gyro_Parameter);
	Angle.GyroFilter2.Y=LPButterworth(Angle.GyroFilter1.Y,&gyro_filter_buf_2[1],&Gyro_Parameter);
	Angle.GyroFilter2.Z=LPButterworth(Angle.GyroFilter1.Z,&gyro_filter_buf_2[2],&Gyro_Parameter);
	

  Angle.Gyro.X = (float)(Angle.GyroFilter2.X * GYRO_SENSITIVITY);
  Angle.Gyro.Y = (float)(Angle.GyroFilter2.Y * GYRO_SENSITIVITY);
  Angle.Gyro.Z = (float)(Angle.GyroFilter2.Z * GYRO_SENSITIVITY);
}

/*
  @func 加速采集，单位转换，计算acc角，pitch角
*/
void GetAccData(void)
{
  int dat[6];

  iic_read_regs(MPU9250_DEV_ADDR, ACCEL_XOUT_H, dat, 6, IIC);
  Angle.AccReal.X = (short)((unsigned short)dat[0] << 8 | dat[1]);
  Angle.AccReal.Y = (short)((unsigned short)dat[2] << 8 | dat[3]);
  Angle.AccReal.Z = (short)((unsigned short)dat[4] << 8 | dat[5]);
	
	Angle.AccFilter1.X=LPButterworth(Angle.AccReal.X,&acc_filter_buf_1[0],&Bandstop_Filter_Parameter_30_94);
	Angle.AccFilter1.Y=LPButterworth(Angle.AccReal.Y,&acc_filter_buf_1[1],&Bandstop_Filter_Parameter_30_94);
	Angle.AccFilter1.Z=LPButterworth(Angle.AccReal.Z,&acc_filter_buf_1[2],&Bandstop_Filter_Parameter_30_94);
	
	Angle.AccFilter2.X=LPButterworth(Angle.AccFilter1.X,&acc_filter_buf_2[0],&Accel_Parameter);
	Angle.AccFilter2.Y=LPButterworth(Angle.AccFilter1.Y,&acc_filter_buf_2[1],&Accel_Parameter);
	Angle.AccFilter2.Z=LPButterworth(Angle.AccFilter1.Z,&acc_filter_buf_2[2],&Accel_Parameter);

  Angle.Acc.X = (float)(Angle.AccFilter2.X*ACC_SENSITIVITY);
  Angle.Acc.Y = (float)(Angle.AccFilter2.Y*ACC_SENSITIVITY);
  Angle.Acc.Z = (float)(Angle.AccFilter2.Z*ACC_SENSITIVITY);
}
/*
  @func 加速陀螺仪温度
*/
void GetTempData(void)
{
  int dat[2];

  iic_read_regs(MPU9250_DEV_ADDR, TEMP_OUT_H, dat, 2, IIC);
  temp = (short)((unsigned short)dat[0] << 8 | dat[1]);
}
void CountAngleGyro(void)
{
  GetGyroData();
  GetAccData();
  GetTempData();

  Pitch.GyroAngle = Pitch.Angle + Angle.Gyro.X * _IMU_DT_;
  Pitch.Gyro = Angle.Gyro.X;
  Roll.GyroAngle = Roll.Angle + Angle.Gyro.Y * _IMU_DT_;
  Roll.Gyro = Angle.Gyro.Y;
  //偏航角
  Yaw.Angle += Angle.Gyro.Z * _IMU_DT_;
  Yaw.Gyro = Angle.Gyro.Z;
  //俯仰角
  Pitch.AccAngle = 57.3f*atan(Angle.Acc.Y*invSqrt(Angle.Acc.X*Angle.Acc.X+Angle.Acc.Z*Angle.Acc.Z));
  Pitch.Angle = 0.985f * Pitch.GyroAngle + 0.015f * Pitch.AccAngle;
  //横滚角
  Roll.AccAngle = -57.3f*atan(Angle.Acc.X*invSqrt(Angle.Acc.Y*Angle.Acc.Y+Angle.Acc.Z*Angle.Acc.Z));
  Roll.Angle = 0.985f * Roll.GyroAngle + 0.015f * Roll.AccAngle;
}

void MPU9250_DataGet(void)
{
  CountAngleGyro();
	mpudata.temperature = temp;
	
  mpudata.gyro_pitch = Pitch.Gyro;
	mpudata.gyro_yaw = Yaw.Gyro;
  mpudata.gyro_roll = Roll.Gyro;
  mpudata.acc_x = Angle.Acc.X;//（roll）
  mpudata.acc_y = Angle.Acc.Y;//（pitch）
  mpudata.acc_z = Angle.Acc.Z;//（yaw）
	
	mpudata.angle_pitch = Pitch.Angle;
  mpudata.angle_yaw = Yaw.Angle;
	mpudata.angle_roll = Roll.Angle;
}

//
//  Author:	SaleJuice
//  Laboratory:	CyberSmartCar
//  School:	CJLU
//

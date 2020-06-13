//
//  File:	sj_mpu9250.h
//

#ifndef SJ_MPU9250_H
#define SJ_MPU9250_H

#include <math.h>
#include "sj_math.h"
#include "sj_filter.h"
#include "sj_iic.h"

// 定义MPU9250内部地址

#define MPU9250_DEV_ADDR        0x68    //IIC写入时的地址字节数据，+1为读取

#define SMPLRT_DIV              0x19    //陀螺仪采样率，典型值：0x07(125Hz)
#define MPU_CONFIG              0x1A    //低通滤波频率，典型值：0x06(5Hz)
#define GYRO_CONFIG             0x1B    //陀螺仪自检及测量范围，典型值：0x18(不自检，2000deg/s)
#define ACCEL_CONFIG            0x1C    //加速计自检、测量范围及高通滤波频率，典型值：0x01(不自检，2G，5Hz)
#define ACCEL_CONFIG2           0X1D    //加速度计低通滤波器 0x06 5hz
#define INT_PIN_CFG             0x37    //设置9250辅助I2C为直通模式寄存器
#define ACCEL_XOUT_H            0x3B
#define ACCEL_XOUT_L            0x3C
#define ACCEL_YOUT_H            0x3D
#define ACCEL_YOUT_L            0x3E
#define ACCEL_ZOUT_H            0x3F
#define ACCEL_ZOUT_L            0x40
#define TEMP_OUT_H              0X41    //温度计输出数据
#define TEMP_OUT_L              0X42
#define GYRO_XOUT_H             0x43
#define GYRO_XOUT_L             0x44
#define GYRO_YOUT_H             0x45
#define GYRO_YOUT_L             0x46
#define GYRO_ZOUT_H             0x47
#define GYRO_ZOUT_L             0x48
#define USER_CTRL               0x6A    //关闭9250对辅助I2C设备的控制,用户配置当为0X10时使用SPI模式
#define PWR_MGMT_1              0x6B    //电源管理，典型值：0x00(正常启用)
#define WHO_AM_I                0x75    //器件ID MPU9250默认ID为0X71

#define BARRICADE_LEFT          0
#define BARRICADE_RIGHT         1

// 俯仰角数据
typedef struct
{
  float       Angle;
  float       Gyro;
  float       AccAngle;
  float       GyroAngle;
} PITCH;

// 俯仰角数据
typedef struct
{
  float       Angle;
  float       Gyro;
  float       AccAngle;
  float       GyroAngle;
} ROLL;

// 偏航角数据
typedef struct
{
  float       Angle;
  float       Gyro;
} YAW;

typedef struct
{
  float     X;
  float     Y;
  float     Z;
}_COORDINATE_FLOAT_;

// 角度信息
typedef struct
{
  _COORDINATE_FLOAT_    Acc;
	_COORDINATE_FLOAT_    AccReal;
	_COORDINATE_FLOAT_    AccFilter1;
	_COORDINATE_FLOAT_    AccFilter2;
  _COORDINATE_FLOAT_    Gyro;
	_COORDINATE_FLOAT_    GyroReal;
  _COORDINATE_FLOAT_    GyroFilter1;
	_COORDINATE_FLOAT_    GyroFilter2;
} ANGLE;

//陀螺仪信息
typedef struct
{
  float angle_pitch;
  float angle_roll;
  float angle_yaw;

  float gyro_pitch;
  float gyro_roll;
  float gyro_yaw;

  float acc_x;
  float acc_y;
  float acc_z;

  float temperature;
} _MPU9250DATA_;

extern ANGLE Angle;
extern ROLL Roll;
extern PITCH Pitch;
extern YAW Yaw;

extern _MPU9250DATA_ mpudata;
void MPU9250_Config(void);//初始化MPU9250
void MPU9250_DataGet(void);//外部调用，通过_MPU9250DATA_结构体读取信息

#endif/*SJ_MPU9250_H*/

//
//  Author:	SaleJuice
//  Laboratory:	CyberSmartCar
//  School:	CJLU
//

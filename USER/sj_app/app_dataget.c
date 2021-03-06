//
//  app_dataget.c
//

#include "app_dataget.h"

//系统核心结构体
_CYBERCOREDATA_ Cyber;
//四轴姿态总结构体
_POSTUREDATA_ posturedata;
//四轴导航总结构体
_NAVIGATIONDATA_ navigationdata;

void CyberDataInit(void)
{
	//Cyber结构体初始化
  Cyber.SysTime = 0;
  Cyber.StopFlag = 1;
	
	//清空期望姿态数据
	posturedata.expectA.p = 0;
  posturedata.expectA.y = 0;
  posturedata.expectA.r = 0;

  posturedata.expectG.p = 0;
  posturedata.expectG.y = 0;
  posturedata.expectG.r = 0;

  //清空期望导航数据
  navigationdata.expectA.x = 0;
  navigationdata.expectA.y = 0;
  navigationdata.expectA.z = 0;

  navigationdata.expectG.x = 0;
  navigationdata.expectG.y = 0;
  navigationdata.expectG.z = 0;
	
	navigationdata.expectC.x = 0;
  navigationdata.expectC.y = 0;
  navigationdata.expectC.z = 0;
}

void OffsetDataSet(void)
{
  //姿态补偿数据
  posturedata.offsetA.p = 1.26;
  posturedata.offsetA.y = 0;
  posturedata.offsetA.r = -1.52;

  posturedata.offsetG.p = 0;
  posturedata.offsetG.y = 0;
  posturedata.offsetG.r = 0;

  //导航补偿数据
  navigationdata.offsetA.x = 0;
  navigationdata.offsetA.y = 0;
  navigationdata.offsetA.z = 0;

  navigationdata.offsetG.x = 0;
  navigationdata.offsetG.y = 0;
  navigationdata.offsetG.z = 0;
	
	navigationdata.offsetC.x = 0;
	navigationdata.offsetC.y = 0;
	navigationdata.offsetC.z = -9.646656f;//-10*sqrt(mpudata.acc_z*mpudata.acc_z+mpudata.acc_x*mpudata.acc_x+mpudata.acc_y*mpudata.acc_y);
}

void ModuleDataHandle(void)
{
	//
	//获取遥控器的各个通道的值
  REMOTEDATA_DataGet(RemoteBuff);
  //获得期望姿态数据
  posturedata.expectA.p = remotedata.channel[0];
  posturedata.expectA.y = remotedata.channel[1];
  posturedata.expectA.r = remotedata.channel[2];
  //获得期望导航数据
  navigationdata.expectA.x = 0;
  navigationdata.expectA.y = 0;
  navigationdata.expectA.z = remotedata.channel[3];
  navigationdata.expectG.x = 0;
  navigationdata.expectG.y = 0;
  navigationdata.expectG.z = 0;
	
	//
	//获取陀螺仪的角度值、角加速度值、角速度值、温度值
  MPU9250_DataGet();
	//计算出矫正后的真实角度数据
  posturedata.realA.p = mpudata.angle_pitch + posturedata.offsetA.p;
  posturedata.realA.y = mpudata.angle_yaw + posturedata.offsetA.y;
  posturedata.realA.r = mpudata.angle_roll + posturedata.offsetA.r;
	//计算出矫正后的真实角速度数据
  posturedata.realG.p = mpudata.gyro_pitch + posturedata.offsetG.p;
  posturedata.realG.y = mpudata.gyro_yaw + posturedata.offsetG.y;
  posturedata.realG.r = mpudata.gyro_roll + posturedata.offsetG.r;
	
	//
	//获取激光测距模块的距离值
	DISTANCEDATA_DataGet(DistanceBuff);
  //计算出矫正后的真实导航数据
	navigationdata.realC.z = 10*sqrt(mpudata.acc_z*mpudata.acc_z+mpudata.acc_x*mpudata.acc_x+mpudata.acc_y*mpudata.acc_y) + navigationdata.offsetC.z;
	navigationdata.realA.z = laserdata.distance + navigationdata.offsetA.z;
	//计算高度速度融合值
	highspeeddatafuse((float)navigationdata.realA.z,navigationdata.realC.z);
	navigationdata.realG.z = fusespeed + navigationdata.offsetG.z;
	
	//
	//获取光流的x轴y轴水平移动速度值、图片质量值
  OPTICALFLOW_DataGet(posturedata.realA.p,posturedata.realA.r,navigationdata.realA.z);
	navigationdata.realG.x = opticalflowdata.delta_x + navigationdata.offsetG.x;
  navigationdata.realG.y = opticalflowdata.delta_y + navigationdata.offsetG.y;
	
	
	//还未使用的
	navigationdata.realC.x = 0 + navigationdata.offsetC.x;
  navigationdata.realC.y = 0 + navigationdata.offsetC.y;
	navigationdata.realA.x = 0 + navigationdata.offsetA.x;
  navigationdata.realA.y = 0 + navigationdata.offsetA.y;
	//获取气压计的高度值、气压值、温度值
  //BMP280_DataGet();
	//获取摄像头的目标点的值
  //CAMERADATA_DataGet();
}

void ModuleDataGetAndHandle()
{
  if(Cyber.SysTime <= 1000)
    OffsetDataSet();
	ModuleDataHandle();
}

//
//  Author:	SaleJuice
//  Laboratory:	CyberSmartCar
//  School:	CJLU
//

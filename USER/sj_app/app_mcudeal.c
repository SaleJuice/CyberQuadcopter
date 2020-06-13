//
//  app_mcudeal.c
//

#include "app_mcudeal.h"

//PID有关变量
PosiPidNode PitchAPid, YawAPid, RollAPid;
PosiPidNode PitchGPid, YawGPid, RollGPid;
PosiPidNode HighAPid, HighGPid, HighCPid;

PosiPidNode NavixGPid, NaviyGPid, NavizGPid;


//电机有关变量
_PYR_Int_ PostureMot;
_XYZ_Int_ NavigationMot;

int MotorAll[4],MotorPosi[4],MotorNavi[4];

//PID参数初始化
void PidDataInit(void)
{
	PosiPidNode Pid_Init;
	
	Pid_Init.kp=1;
	Pid_Init.ki=0;
	Pid_Init.kd=0;
	Pid_Init.outd_filter_flag=0;
	Pid_Init.sum_err_limit=80;
  SetPosiPidParm(&PitchAPid, Pid_Init);
	SetPosiPidParm(&RollAPid, Pid_Init);
	
	Pid_Init.kp=0.85;
	Pid_Init.ki=5.5;
	Pid_Init.kd=0.035;
	Pid_Init.outd_filter_flag=1;
	Pid_Init.sum_err_limit=80;
  SetPosiPidParm(&PitchGPid, Pid_Init);
  SetPosiPidParm(&RollGPid, Pid_Init);
	
	Pid_Init.kp=0;
	Pid_Init.ki=0;
	Pid_Init.kd=0;
	Pid_Init.outd_filter_flag=0;
	Pid_Init.sum_err_limit=80;
	SetPosiPidParm(&YawAPid, Pid_Init);
	
	Pid_Init.kp=1.7;
	Pid_Init.ki=5.5;
	Pid_Init.kd=0;
	Pid_Init.outd_filter_flag=1;
	Pid_Init.sum_err_limit=80;
	SetPosiPidParm(&YawGPid, Pid_Init);
	
	Pid_Init.kp=1;
	Pid_Init.ki=0;
	Pid_Init.kd=0;
	Pid_Init.outd_filter_flag=0;
	Pid_Init.sum_err_limit=80;
	SetPosiPidParm(&HighAPid, Pid_Init);
	
	Pid_Init.kp=7;
	Pid_Init.ki=3;
	Pid_Init.kd=1.8;
	Pid_Init.outd_filter_flag=1;
	Pid_Init.sum_err_limit=80;
  SetPosiPidParm(&HighGPid, Pid_Init);
	
	Pid_Init.kp=1.5;
	Pid_Init.ki=0;
	Pid_Init.kd=0;
	Pid_Init.outd_filter_flag=0;
	Pid_Init.sum_err_limit=80;
	SetPosiPidParm(&NavixGPid, Pid_Init);
	SetPosiPidParm(&NaviyGPid, Pid_Init);
}


//导航PID数据处理
void NavigationDataCal(int *motz)
{
	/*垂直控制*/
	navigationdata.expectA.z = 1.0f;
	//外环垂直位移控制
	navigationdata.expectG.z = CalcPosiPidOut(&HighAPid, navigationdata.expectA.z, navigationdata.realA.z);
	//内环垂直速度控制
	*motz = 400 + (int)CalcPosiPidOut(&HighGPid, navigationdata.expectG.z, navigationdata.realG.z);
	
	/*水平控制*/
	//外环水平位移控制
	;
	//内环水平速度控制
	posturedata.expectA.r = - CalcPosiPidOut(&NavixGPid, navigationdata.expectG.x, navigationdata.realG.x);
	posturedata.expectA.p = CalcPosiPidOut(&NaviyGPid, navigationdata.expectG.y, navigationdata.realG.y);
}

//姿态PID数据处理
void PostureDataCal(int *motp, int *moty, int *motr)
{
	/*姿态控制*/
	//外环角度控制
  posturedata.expectG.p = CalcPosiPidOut(&PitchAPid, posturedata.expectA.p, posturedata.realA.p);
  //posturedata.expectG.y = CalcPosiPidOut(&YawAPid, posturedata.expectA.y, posturedata.realA.y);
  posturedata.expectG.r = CalcPosiPidOut(&RollAPid, posturedata.expectA.r, posturedata.realA.r);
	//内环角速度控制
  *motp = (int)CalcPosiPidOut(&PitchGPid, posturedata.expectG.p, posturedata.realG.p);
  *moty = (int)CalcPosiPidOut(&YawGPid, posturedata.expectG.y, posturedata.realG.y);
  *motr = (int)CalcPosiPidOut(&RollGPid, posturedata.expectG.r, posturedata.realG.r);
}


void ControlArithmeticDeal(void)
{
  if (Cyber.SysTime > 1500)
  {
    //计算导航输出值
    NavigationDataCal(&NavigationMot.z);
		//计算姿态输出值
    PostureDataCal(&PostureMot.p, &PostureMot.y, &PostureMot.r);
  }
  //调试时限制部分姿态不输出
	//PostureMot.p = 0;
  //PostureMot.y = 0;
	//PostureMot.r = 0;
  //NavigationMot.z = 400;
	
  //各个电机姿态输出的累加计算
  MotorPosi[0] = + PostureMot.p + PostureMot.r - PostureMot.y;
  MotorPosi[1] = + PostureMot.p - PostureMot.r + PostureMot.y; 
  MotorPosi[2] = - PostureMot.p + PostureMot.r + PostureMot.y;
  MotorPosi[3] = - PostureMot.p - PostureMot.r - PostureMot.y;
  //各个电机姿态输出的限幅
  rangelimitInt(&MotorPosi[0], -500, 500);
  rangelimitInt(&MotorPosi[1], -500, 500);
  rangelimitInt(&MotorPosi[2], -500, 500);
  rangelimitInt(&MotorPosi[3], -500, 500);
	
	//各个电机导航输出的累加计算
	MotorNavi[0] = NavigationMot.z;
	MotorNavi[1] = NavigationMot.z;
	MotorNavi[2] = NavigationMot.z;
	MotorNavi[3] = NavigationMot.z;
	//各个电机导航输出的限幅
	rangelimitInt(&MotorNavi[0], 0, 500);
  rangelimitInt(&MotorNavi[1], 0, 500);
  rangelimitInt(&MotorNavi[2], 0, 500);
  rangelimitInt(&MotorNavi[3], 0, 500);
	
	//各个电机总输出的累加计算
	MotorAll[0] = MotorPosi[0] + MotorNavi[0];
	MotorAll[1] = MotorPosi[1] + MotorNavi[1];
	MotorAll[2] = MotorPosi[2] + MotorNavi[2];
	MotorAll[3] = MotorPosi[3] + MotorNavi[3];
	//各个电机总输出的限幅
  rangelimitInt(&MotorAll[0], 0, 900);
  rangelimitInt(&MotorAll[1], 0, 900);
  rangelimitInt(&MotorAll[2], 0, 900);
  rangelimitInt(&MotorAll[3], 0, 900);
}

//
//  Author:	SaleJuice
//  Laboratory:	CyberSmartCar
//  School:	CJLU
//

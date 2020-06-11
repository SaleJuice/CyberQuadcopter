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

int Motor[4],MotorPosi[4],MotorNavi[4];

void PidDataInit(void)
{
  //设置角度pid参数：pid结构体/kp/ki/kd
  SetPosiPidParm(&PitchAPid, 1, 0, 0,0);//3, 0, 0
  SetPosiPidParm(&YawAPid, 0, 0, 0,0);//3, 0, 0
  SetPosiPidParm(&RollAPid, 1, 0, 0,0);//3, 0, 0
  //设置角速度pid参数：pid结构体/kp/ki/kd
  SetPosiPidParm(&PitchGPid, 0.85, 5.5, 0.035,1);//1.5, 6, 0.045(6/6)//1.15, 5.5, 0.045(6/7)//1.0, 5.5, 0.035(6/8/19:30)//0.85, 5.5, 0.035(6/8/19:50)
  SetPosiPidParm(&YawGPid, 1.5, 0, 0,1);//7.0, 0, 0
  SetPosiPidParm(&RollGPid, 0.85, 5.5, 0.035,1);//0.85, 5.5, 0.035
  //设置高度pid参数：pid结构体/kp/ki/kd
  SetPosiPidParm(&HighAPid, 1, 0, 0,0);
	//设置高速度pid参数：pid结构体/kp/ki/kd
  SetPosiPidParm(&HighGPid, 7, 3, 1.8,1);
  //设置高加速度pid参数：pid结构体/kp/ki/kd
  SetPosiPidParm(&HighCPid, 0, 0, 0,0);
	
	//设置光流pid参数：pid结构体/kp/ki/kd
	SetPosiPidParm(&NavixGPid, 1.5, 0, 0,0);
	SetPosiPidParm(&NaviyGPid, 1.5, 0, 0,0);
}

void MotOutputLimit(int *output, int limit_L, int limit_H)
{
  if (*output <  limit_L)
    *output =  limit_L;
  else if (*output > limit_H)
    *output = limit_H;
}

void ControlArithmeticInit(void)
{
	
}

void NavigationDataCal(int *motz)
{
	static float cc;
	navigationdata.expectA.z = 1.0f;
	//高度环
	navigationdata.expectG.z = CalcPosiPidOut(&HighAPid, navigationdata.expectA.z, navigationdata.realA.z);
	//速度环
	cc = CalcPosiPidOut(&HighGPid, navigationdata.expectG.z, navigationdata.realG.z);
	//加速度环
	//*motz = CalcPosiPidOut(&HighCPid, navigationdata.expectC.z, navigationdata.realC.z);
  //*motz = navigationdata.expectA.z;
	
	*motz = 400 + (int)cc;
	
	//光流控制
	posturedata.expectA.r = CalcPosiPidOut(&NavixGPid, navigationdata.expectG.x, navigationdata.realG.x);
	posturedata.expectA.p = CalcPosiPidOut(&NaviyGPid, navigationdata.expectG.y, navigationdata.realG.y);
}

void PostureDataCal(int *motp, int *moty, int *motr)
{
  posturedata.expectG.p = CalcPosiPidOut(&PitchAPid, posturedata.expectA.p, posturedata.realA.p);
  //posturedata.expectG.y = CalcPosiPidOut(&YawAPid, posturedata.expectA.y, posturedata.realA.y);
  posturedata.expectG.r = CalcPosiPidOut(&RollAPid, posturedata.expectA.r, posturedata.realA.r);
  *motp = (int)CalcPosiPidOut(&PitchGPid, posturedata.expectG.p, posturedata.realG.p);
  *moty = (int)CalcPosiPidOut(&YawGPid, posturedata.expectG.y, posturedata.realG.y);
  *motr = (int)CalcPosiPidOut(&RollGPid, posturedata.expectG.r, posturedata.realG.r);
}

void ControlArithmeticDeal(void)
{
  if (Cyber.SysTime > 1500)
  {
    //导航计算输出值
    NavigationDataCal(&NavigationMot.z);
		//姿态计算输出值
    PostureDataCal(&PostureMot.p, &PostureMot.y, &PostureMot.r);
  }
  //调试时限制部分姿态不输出
	//PostureMot.p = 0;
  //PostureMot.y = 0;
	//PostureMot.r = 0;
  //NavigationMot.z = 400;
  //各个电机值的分配与整合
  MotorPosi[0] = + PostureMot.p - PostureMot.r - PostureMot.y;
  MotorPosi[1] = + PostureMot.p + PostureMot.r + PostureMot.y; 
  MotorPosi[2] = - PostureMot.p - PostureMot.r + PostureMot.y;
  MotorPosi[3] = - PostureMot.p + PostureMot.r - PostureMot.y;
  //各个电机值的限幅
  MotOutputLimit(&MotorPosi[0], -500, 500);
  MotOutputLimit(&MotorPosi[1], -500, 500);
  MotOutputLimit(&MotorPosi[2], -500, 500);
  MotOutputLimit(&MotorPosi[3], -500, 500);
	//
	MotorNavi[0] = NavigationMot.z;
	MotorNavi[1] = NavigationMot.z;
	MotorNavi[2] = NavigationMot.z;
	MotorNavi[3] = NavigationMot.z;
	//
	MotOutputLimit(&MotorNavi[0], 0, 500);
  MotOutputLimit(&MotorNavi[1], 0, 500);
  MotOutputLimit(&MotorNavi[2], 0, 500);
  MotOutputLimit(&MotorNavi[3], 0, 500);
	
	//
	Motor[0]=MotorPosi[0]+MotorNavi[0];
	Motor[1]=MotorPosi[1]+MotorNavi[1];
	Motor[2]=MotorPosi[2]+MotorNavi[2];
	Motor[3]=MotorPosi[3]+MotorNavi[3];
	//各个电机值的限幅
  MotOutputLimit(&Motor[0], 0, 900);
  MotOutputLimit(&Motor[1], 0, 900);
  MotOutputLimit(&Motor[2], 0, 900);
  MotOutputLimit(&Motor[3], 0, 900);
}

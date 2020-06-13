//
//  app_motorctrl.c
//

#include "app_motorctrl.h"

void MotorPwmOutputControl(void)
{
  if (Cyber.SysTime <= -1 && 0) //调节油门进程的时候改为2500（注意安全）
  {
    PWM_Operate(1, 2000);
    PWM_Operate(2, 2000);
    PWM_Operate(3, 2000);
    PWM_Operate(4, 2000);
  }
  else
  {
    //设置飞机紧急停止的条件
    if (remotedata.channel[3] <= -1 || fabs(posturedata.realA.p) >= 45 || fabs(posturedata.realA.r) >= 45)
    {
      Cyber.StopFlag = 1;
    }
    //分情况执行任务
    if (Cyber.StopFlag == 1)
    {
      PWM_Operate(1, 1000);
      PWM_Operate(2, 1000);
      PWM_Operate(3, 1000);
      PWM_Operate(4, 1000);
    }
    else
    {
      PWM_Operate(1, (MotorAll[0]+1000));
      PWM_Operate(2, (MotorAll[1]+1000));
      PWM_Operate(3, (MotorAll[2]+1000));
      PWM_Operate(4, (MotorAll[3]+1000));
    }
  }
}

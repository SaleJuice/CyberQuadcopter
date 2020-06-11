//
//  sj_pid.c
//

//本文件包括位置式PID（Positional PID）算法和增量式（incremental PID）PID算法
//用户可可再外部函数声明其结构体和调用本头文件函数来使用两种PID
//位置式PID的参数调节方法：先调节KP，待出现抖动再调节KI（KI一般为0.0几），最后调节KD（KD一般很大）
//增量式PID的参数调节方法：先调节KI，再调节KP（步进电机用增量式PID较好）

#include "sj_pid.h"

#define CYCLE_DT 0.005 //秒

/*******************************/
/**********位置式PID部分**********/
/*******************************/
//u(k) = Kp*e(k) + Ki*∑e + Kd*Δe(k) , 式中Δe(k)=e(k)-e(k-1)
//设置位置式PID参数
void SetPosiPidParm(PosiPidNode *pid, float kp, float ki, float kd,int filter_flag)
{
  //限制范围
  if (kp < 0)
  {
    kp = 0;
  }
  if (ki < 0)
  {
    ki = 0;
  }
  if (kd < 0)
  {
    kd = 0;
  }
  pid->kp = kp;
  pid->ki = ki;
  pid->kd = kd;
  pid->limit_out_abs = _LIMIT_OUT_ABS_;
	pid->filter_flag = filter_flag;
	if(pid->filter_flag)
		Set_Cutoff_Frequency(200, 20,&pid->filter_parameter);
}

//计算位置式PID输出
//输入参数
//expc:期望值
//curr:当前值
float CalcPosiPidOut(PosiPidNode *pid, float expc, float curr)
{
  float ep = 0, ei = 0, ed = 0;
  float posi_out = 0;

  pid->error_curr = expc - curr;

  pid->sum_error += pid->error_curr * CYCLE_DT;
  if (pid->sum_error >= pid->limit_out_abs)
  {
    pid->sum_error = pid->limit_out_abs;
  }
  else if (pid->sum_error <= -pid->limit_out_abs)
  {
    pid->sum_error = -pid->limit_out_abs;
  }

  ep = pid->error_curr;
  ei = pid->sum_error;
  ed = (pid->error_curr - pid->error_last) / CYCLE_DT;
	if(pid->filter_flag)
		ed=LPButterworth(ed,&pid->filter_buff,&pid->filter_parameter);
	
	pid->cc = ed;
  posi_out = pid->kp * ep + pid->ki * ei + pid->kd * ed;

  pid->error_last = pid->error_curr;

  return posi_out;
}

//清空位置式PID累计值
void CleanPosiPidData(PosiPidNode *pid)
{
  pid->error_curr = 0;
  pid->error_last = 0;
  pid->sum_error = 0;
}

/**********增量式PID部分**********/
//Δu(k)=u(k)-u(k-1) = Kp*Δe(k)+ Ki*e(k)+ Kd*[Δe(k)-Δe(k-1)] ，式中Δe(k)=e(k)-e(k-1)

//设置增量式PID参数
void SetInrcPidParm(InrcPidNode *pid, float kp, float ki, float kd)
{
  //限制范围
  if (kp < 0)
  {
    kp = 0;
  }
  if (ki < 0)
  {
    ki = 0;
  }
  if (kd < 0)
  {
    kd = 0;
  }
  pid->kp = kp;
  pid->ki = ki;
  pid->kd = kd;
  pid->limit_out_abs = _LIMIT_OUT_ABS_;
}

//计算增量式PID目标增量
//输入参数
//expc:期望值
//curr:当前值
float CalcInrcPidOut(InrcPidNode *pid, float expc, float curr)
{
  float ep = 0, ei = 0, ed = 0;
  float inrc_out = 0;

  pid->error_curr = expc - curr;

  ep = pid->error_curr - pid->error_last;
  ei = pid->error_curr;
  ed = pid->error_curr - 2 * pid->error_last + pid->error_pre;

  inrc_out = pid->kp * ep + pid->ki * ei + pid->kd * ed;

  pid->error_pre = pid->error_last;
  pid->error_last = pid->error_curr;

  return inrc_out;
}

//清空增量式PID累计值
void CleanInrcPidData(InrcPidNode *pid)
{
  pid->error_curr = 0;
  pid->error_last = 0;
  pid->error_pre = 0;
}

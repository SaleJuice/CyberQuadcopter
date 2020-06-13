//
//  sj_pid.c
//

#include "sj_pid.h"

#define _PID_DT_ 0.005 //计算周期（单位：s）

//位置式PID部分
//u(k) = Kp*e(k) + Ki*∑e + Kd*Δe(k) , 式中Δe(k)=e(k)-e(k-1)

//清空位置式PID累计值
void CleanPosiPidData(PosiPidNode *pid)
{
	//清空pid历史数据
	pid->err_curr = 0;
  pid->err_last = 0;
  pid->sum_err = 0;
	
	pid->ep = 0;
	pid->ei = 0;
	pid->ed = 0;
	
	pid->outp = 0;
	pid->outi = 0;
	pid->outd = 0;
	pid->outall = 0;
}

//设置位置式PID参数
void SetPosiPidParm(PosiPidNode *pid, PosiPidNode sample)
{
  if (sample.kp < 0)
    sample.kp = 0;
  if (sample.ki < 0)
    sample.ki = 0;
  if (sample.kd < 0)
    sample.kd = 0;
  pid->kp = sample.kp;
  pid->ki = sample.ki;
  pid->kd = sample.kd;

	pid->outi_limit = sample.outi_limit;
  pid->outall_limit = sample.outall_limit;
	pid->sum_err_limit = sample.sum_err_limit;
	
	pid->outd_filter_flag = sample.outd_filter_flag;
	if(pid->outd_filter_flag)
		Set_Cutoff_Frequency(200, 20,&pid->outd_filter_para);
	
	//清空pid历史数据
	pid->err_curr = 0;
  pid->err_last = 0;
  pid->sum_err = 0;
	
	pid->ep = 0;
	pid->ei = 0;
	pid->ed = 0;
	
	pid->outp = 0;
	pid->outi = 0;
	pid->outd = 0;
	pid->outall = 0;
}

//计算位置式PID输出（输入参数: expc:期望值 curr:当前值）
float CalcPosiPidOut(PosiPidNode *pid, float expc, float curr)
{
  pid->err_curr = expc - curr;

  pid->sum_err += pid->err_curr * _PID_DT_;
	
  if (pid->sum_err >= pid->sum_err_limit)
    pid->sum_err = pid->sum_err_limit;
  else if (pid->sum_err <= -pid->sum_err_limit)
    pid->sum_err = -pid->sum_err_limit;

  pid->ep = pid->err_curr;
  pid->ei = pid->sum_err;
  pid->ed = (pid->err_curr - pid->err_last) / _PID_DT_;
	if(pid->outd_filter_flag)
		pid->ed=LPButterworth(pid->ed,&pid->outd_filter_buff,&pid->outd_filter_para);
	
	pid->outp = pid->kp * pid->ep;
	pid->outi = pid->ki * pid->ei;
	pid->outd = pid->kd * pid->ed;
	
  pid->outall = pid->outp + pid->outi + pid->outd;

  pid->err_last = pid->err_curr;

  return pid->outall;
}

//
//  Author:	SaleJuice
//  Laboratory:	CyberSmartCar
//  School:	CJLU
//

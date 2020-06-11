//
//  sj_pid.h
//

#ifndef SJ_PID_H
#define SJ_PID_H

#include "sj_filter.h"

#define _LIMIT_OUT_ABS_  80

//位置式PID参数结构体
//u(k) = Kp*e(k) + Ki*∑e + Kd*Δe(k) , 式中Δe(k)=e(k)-e(k-1)
typedef struct
{
  float kp;
  float ki;
  float kd;

	float cc;
	
  float error_curr;   //当前误差
  float error_last;   //上一次的误差
  float sum_error;    //误差和
	
	int filter_flag;
	Butter_BufferData filter_buff;
	Butter_Parameter filter_parameter;

  float limit_out_abs;    //输出限幅
} PosiPidNode;

//增量式PID参数结构体
//Δu(k)=u(k)-u(k-1) = Kp*Δe(k)+ Ki*e(k)+ Kd*[Δe(k)-Δe(k-1)] ，式中Δe(k)=e(k)-e(k-1)
typedef struct
{
  float kp;
  float ki;
  float kd;

  float error_curr;   //当前误差（e_0）
  float error_last;   //上一次误差(e_1)
  float error_pre;    //前一次的偏差(e_2)

  float limit_out_abs;    //输出限幅
} InrcPidNode;

//位置式PID
void SetPosiPidParm(PosiPidNode *pid, float kp, float ki, float kd,int filter_flag);//设置位置式PID参数
float CalcPosiPidOut(PosiPidNode *pid, float expc, float curr);//计算位置式PID输出
void CleanPosiPidData(PosiPidNode *pid);//清空位置式PID累计值

//增量式PID
void SetInrcPidParm(InrcPidNode *pid, float kp, float ki, float kd);//设置PID参数
float CalcInrcPidOut(InrcPidNode *pid, float expc, float curr);//计算目标增量
void CleanInrcPidData(InrcPidNode *pid);//清空增量式PID累计值

#endif/*SJ_PID_H*/

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
  float kp;								//比例系数
  float ki;								//积分系数
  float kd;								//微分系数
	
	float ep;  						//比例计算值
	float ei;  						//积分计算值
	float ed;  						//微分计算值
	
	float outp;  						//比例输出
	float outi;  						//积分输出
	float outd;  						//微分输出
	float outall; 					//总输出

	int outi_open_flag;			//积分开启标志
	float sum_err_limit;    //积分误差累计值限幅
	
	float outi_limit;   		//积分输出限幅
  float outall_limit;    	//总输出限幅
	
  float err_curr;  				//当前误差
  float err_last;  				//上一次的误差
  float sum_err;    			//误差累计值
	
	int outd_filter_flag;										//微分滤波标志
	Butter_BufferData outd_filter_buff;			//微分滤波缓存数组
	Butter_Parameter outd_filter_para;			//微分滤波参数
} PosiPidNode;

//位置式PID
void SetPosiPidParm(PosiPidNode *pid, PosiPidNode sample);//设置位置式PID参数
float CalcPosiPidOut(PosiPidNode *pid, float expc, float curr);//计算位置式PID输出
void CleanPosiPidData(PosiPidNode *pid);//清空位置式PID累计值

#endif/*SJ_PID_H*/

//
//  Author:	SaleJuice
//  Laboratory:	CyberSmartCar
//  School:	CJLU
//

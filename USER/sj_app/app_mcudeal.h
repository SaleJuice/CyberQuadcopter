//
//  app_mcudeal.h
//

#ifndef app_mcudeal_h
#define app_mcudeal_h

#include "sj_headfile.h"

//PID有关变量
extern PosiPidNode PitchAPid, YawAPid, RollAPid;
extern PosiPidNode PitchGPid, YawGPid, RollGPid;
extern PosiPidNode HighAPid, HighGPid, HighCPid;
//电机有关变量
extern _PYR_Int_ PostureMot;
extern _XYZ_Int_ NavigationMot;
extern int MotorAll[4];

extern Butter_Parameter Motor_Parameter;
extern void PidDataInit(void);
extern void ControlArithmeticDeal(void);

#endif/*app_mcudeal_h*/

//
//  Author:	SaleJuice
//  Laboratory:	CyberSmartCar
//  School:	CJLU
//

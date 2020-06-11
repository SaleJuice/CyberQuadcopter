//
// sj_pwm.h
//

#ifndef SJ_PWM_H
#define	SJ_PWM_H


#include "stm32f10x.h"

void PWM_Config(void);
void PWM_Operate(int port,int duty);

#endif /*SJ_PWM_H*/

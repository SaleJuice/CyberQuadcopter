//
// sj_ppm.h
//

#ifndef SJ_PPM_H
#define SJ_PPM_H

#include "stm32f10x.h"

#define            PPM_TIM                   TIM4
#define            PPM_TIM_APBxClock_FUN     RCC_APB1PeriphClockCmd
#define            PPM_TIM_CLK               RCC_APB1Periph_TIM4
#define            PPM_TIM_Period            (1000-1)
#define            PPM_TIM_Prescaler         71
#define            PPM_TIM_IRQ               TIM4_IRQn
#define            PPM_TIM_IRQHandler        TIM4_IRQHandler

void PPM_Config(void);

#endif	/* SJ_PPM_H */



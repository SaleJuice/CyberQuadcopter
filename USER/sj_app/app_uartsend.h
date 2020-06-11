//
//  app_uartsend.h
//

#ifndef app_uartsend_h
#define app_uartsend_h

#include "sj_headfile.h"

extern float senddata[4];

extern u8 TIM4_CH1_CAPTURE_STA;
extern u16 TIM4_CH1_CAPTURE_VAL;
extern u8 TIM4_CH1_CAPTURE_TIMES;

void UartInit(void);
void UartDataGetAndSend(void);

#endif/*app_uartsend_h*/

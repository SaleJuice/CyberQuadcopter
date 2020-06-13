//
// sj_led.h
//

#ifndef SJ_LED_H
#define SJ_LED_H

#include "stm32f10x.h"

#define    	LED1_GPIO_RCC    	RCC_APB2Periph_GPIOE
#define    	LED1_GPIO_PORT   	GPIOE
#define    	LED1_GPIO_PIN		  GPIO_Pin_2

#define    	LED2_GPIO_RCC    	RCC_APB2Periph_GPIOE
#define    	LED2_GPIO_PORT   	GPIOE
#define    	LED2_GPIO_PIN		  GPIO_Pin_3

#define    	LED3_GPIO_RCC    	RCC_APB2Periph_GPIOE
#define    	LED3_GPIO_PORT   	GPIOE
#define    	LED3_GPIO_PIN		  GPIO_Pin_4

#define    	LED4_GPIO_RCC    	RCC_APB2Periph_GPIOE
#define    	LED4_GPIO_PORT   	GPIOE
#define    	LED4_GPIO_PIN		  GPIO_Pin_5

#define LED_GPIO_RCC 	LED1_GPIO_RCC|LED2_GPIO_RCC|LED3_GPIO_RCC|LED4_GPIO_RCC

void LED_Config(void);
void LED_Operate(int led,int status);

#endif/*SJ_LED_H*/

//
//  Author:	SaleJuice
//  Laboratory:	CyberSmartCar
//  School:	CJLU
//

//
// sj_key.h
//

#ifndef KEY_H
#define KEY_H

#include "stm32f10x.h"

// KEY_INTERVAL_TIME时间周期，为按键的扫描间隔时间，单位ms
#define KEY_INTERVAL_MS         (100)

//定义按键读取端口
#define    	KEY1_GPIO_RCC    	RCC_APB2Periph_GPIOC
#define    	KEY1_GPIO_PORT   	GPIOC
#define    	KEY1_GPIO_PIN		GPIO_Pin_0

#define    	KEY2_GPIO_RCC    	RCC_APB2Periph_GPIOC
#define    	KEY2_GPIO_PORT   	GPIOC
#define    	KEY2_GPIO_PIN		GPIO_Pin_1

#define    	KEY3_GPIO_RCC    	RCC_APB2Periph_GPIOC
#define    	KEY3_GPIO_PORT   	GPIOC
#define    	KEY3_GPIO_PIN		GPIO_Pin_2

#define    	KEY4_GPIO_RCC    	RCC_APB2Periph_GPIOC
#define    	KEY4_GPIO_PORT   	GPIOC
#define    	KEY4_GPIO_PIN		GPIO_Pin_3

#define    	KEY5_GPIO_RCC    	RCC_APB2Periph_GPIOC
#define    	KEY5_GPIO_PORT   	GPIOC
#define    	KEY5_GPIO_PIN		GPIO_Pin_4

#define    	KEY6_GPIO_RCC    	RCC_APB2Periph_GPIOC
#define    	KEY6_GPIO_PORT   	GPIOC
#define    	KEY6_GPIO_PIN		GPIO_Pin_5

#define 	KEY_GPIO_RCC 	KEY1_GPIO_RCC|KEY2_GPIO_RCC|KEY3_GPIO_RCC|KEY4_GPIO_RCC|KEY5_GPIO_RCC|KEY6_GPIO_RCC

//定义按键状态
#define KEY_ON				0	//按键按下
#define KEY_OFF		   		1	//按键松开
#define KEY_CONTINUOUS 		2   //按键持续

// 按键模式
typedef enum
{
    _KEY_NONE_,     // 无按键按下
	_KEY_START_,
    _KEY_ADD_PAGE_,
    _KEY_UP_LINE_,
	_KEY_DOWN_LINE_,
    _KEY_ADD_NUM_,
    _KEY_POOR_NUM_,
	_KEY_CONTINUOUS_
}keyMode_t;

extern keyMode_t keyMode;

void KEY_Config(void);
keyMode_t Key_Scanf(void);
void Key_AddTimeAfterLast(void);    // 此函数放在定时器中断里

#endif/*SJ_KEY_H*/

//
// sj_led.c
//

#include "sj_led.h"

//LED始化函数
void LED_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(LED1_GPIO_RCC, ENABLE);			//使能时钟,stm32配置外设时必须先使能外设时钟
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		//设置成推挽输出(可以输出高低电平)
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_InitStructure.GPIO_Pin  = LED1_GPIO_PIN;
 	GPIO_Init(LED1_GPIO_PORT, &GPIO_InitStructure);			//初始化GPIO
	
	GPIO_InitStructure.GPIO_Pin  = LED2_GPIO_PIN;
 	GPIO_Init(LED2_GPIO_PORT, &GPIO_InitStructure);			//初始化GPIO
	
	GPIO_InitStructure.GPIO_Pin  = LED3_GPIO_PIN;
 	GPIO_Init(LED3_GPIO_PORT, &GPIO_InitStructure);			//初始化GPIO
	
	GPIO_InitStructure.GPIO_Pin  = LED4_GPIO_PIN;
 	GPIO_Init(LED4_GPIO_PORT, &GPIO_InitStructure);			//初始化GPIO
	
}

void LED_Operate(int led,int status)
{
	if(status == 1)
	{
		switch (led)
		{
		case 1:
			GPIO_ResetBits(LED1_GPIO_PORT,LED1_GPIO_PIN);
			break;
		
		case 2:
			GPIO_ResetBits(LED2_GPIO_PORT,LED2_GPIO_PIN);
			break;

		case 3:
			GPIO_ResetBits(LED3_GPIO_PORT,LED3_GPIO_PIN);
			break;

		case 4:
			GPIO_ResetBits(LED4_GPIO_PORT,LED4_GPIO_PIN);
			break;

		default:
			GPIO_ResetBits(LED1_GPIO_PORT,LED1_GPIO_PIN);
			GPIO_ResetBits(LED2_GPIO_PORT,LED2_GPIO_PIN);
			GPIO_ResetBits(LED3_GPIO_PORT,LED3_GPIO_PIN);
			GPIO_ResetBits(LED4_GPIO_PORT,LED4_GPIO_PIN);
			break;
		}
	}
	else if(status == 0)
	{
		switch (led)
		{
		case 1:
			GPIO_SetBits(LED1_GPIO_PORT,LED1_GPIO_PIN);
			break;
		
		case 2:
			GPIO_SetBits(LED2_GPIO_PORT,LED2_GPIO_PIN);
			break;

		case 3:
			GPIO_SetBits(LED3_GPIO_PORT,LED3_GPIO_PIN);
			break;

		case 4:
			GPIO_SetBits(LED4_GPIO_PORT,LED4_GPIO_PIN);
			break;

		default:
			GPIO_SetBits(LED1_GPIO_PORT,LED1_GPIO_PIN);
			GPIO_SetBits(LED2_GPIO_PORT,LED2_GPIO_PIN);
			GPIO_SetBits(LED3_GPIO_PORT,LED3_GPIO_PIN);
			GPIO_SetBits(LED4_GPIO_PORT,LED4_GPIO_PIN);
			break;
		}
	}
}

//
//  Author:	SaleJuice
//  Laboratory:	CyberSmartCar
//  School:	CJLU
//

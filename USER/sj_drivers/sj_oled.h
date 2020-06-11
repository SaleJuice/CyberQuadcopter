//
// sj_oled.h
//

#ifndef SJ_OLED_H
#define SJ_OLED_H

#include "stm32f10x.h"

//此OLED程序适合6/7脚OLED（7脚OLED的CS脚需要设置为低电平才使能有效）

#define	SCL_GPIO_RCC	RCC_APB2Periph_GPIOE
#define	SCL_GPIO_PORT	GPIOE
#define	SCL_GPIO_PIN	GPIO_Pin_10

#define	SDA_GPIO_RCC	RCC_APB2Periph_GPIOE
#define	SDA_GPIO_PORT	GPIOE
#define	SDA_GPIO_PIN	GPIO_Pin_11

#define	RES_GPIO_RCC	RCC_APB2Periph_GPIOE
#define	RES_GPIO_PORT	GPIOE
#define	RES_GPIO_PIN	GPIO_Pin_12

#define	DC_GPIO_RCC		RCC_APB2Periph_GPIOE
#define	DC_GPIO_PORT	GPIOE
#define	DC_GPIO_PIN		GPIO_Pin_13

#define OLED_GPIO_RCC 	SCL_GPIO_RCC|SDA_GPIO_RCC|RES_GPIO_RCC|DC_GPIO_RCC

//OLED端口定义
//OLED的时钟引脚(SCL)
#define OLED_SCLL 	GPIO_ResetBits(SCL_GPIO_PORT,SCL_GPIO_PIN)
#define OLED_SCLH 	GPIO_SetBits(SCL_GPIO_PORT,SCL_GPIO_PIN)
//OLED的数据引脚(SDA)
#define OLED_SDAL 	GPIO_ResetBits(SDA_GPIO_PORT,SDA_GPIO_PIN)
#define OLED_SDAH 	GPIO_SetBits(SDA_GPIO_PORT,SDA_GPIO_PIN)
//OLED的复位引脚(RES)
#define OLED_RSTL 	GPIO_ResetBits(RES_GPIO_PORT,RES_GPIO_PIN)
#define OLED_RSTH 	GPIO_SetBits(RES_GPIO_PORT,RES_GPIO_PIN)
//OLED的数据/命令控制引脚(DC)
#define OLED_DCL 	GPIO_ResetBits(DC_GPIO_PORT,DC_GPIO_PIN)
#define OLED_DCH 	GPIO_SetBits(DC_GPIO_PORT,DC_GPIO_PIN)

/*
* @func OLED初始化
*/
void OLED_Config(void);
/*
* @func 清屏函数
*/
void OLED_Cls(void);
/*
* @func 写入一组6x8标准ASCII字符串
* @para y 行数
* @para x 列数
* @para ch[] 要显示的字符串
*/
void OLED_P6x8Str(unsigned char y, unsigned char x, char ch[]);
/*
* @func 显示一个int类型的数据
* @para y 行数
* @para x 列数
* @para length 设定的整个数据长度（右对齐）
* @para ival 数据
*/
void OLED_P6x8Int(unsigned char y, unsigned char x, int32_t ival, unsigned char length);
/*
* @func 显示一个float类型的数据
* @para y 行数
* @para x 列数
* @para length 设定的整个数据长度（右对齐）
* @para lenFloat 小数位数
* @para dval 数据
*/
void OLED_P6x8Float(unsigned char y, unsigned char x, float dval, unsigned char length, unsigned char lenFloat);
/*
* @func 显示一副图像
* @para unsigned char 类型的数组（保存了图像信息）
* @para rowNum, columnNum
*/
void OLED_PrintImage(unsigned char *pucTable, unsigned char rowNum,  unsigned char columnNum);

#endif/*SJ_OLED_H*/

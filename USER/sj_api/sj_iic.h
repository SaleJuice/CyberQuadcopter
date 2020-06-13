//
//  sj_iic.h
//

#ifndef SJ_IIC_H
#define SJ_IIC_H

#include "stm32f10x.h"

#define	IIC_SCL_GPIO_RCC	RCC_APB2Periph_GPIOA
#define	IIC_SCL_GPIO_PORT	GPIOA
#define	IIC_SCL_GPIO_PIN	GPIO_Pin_0

#define	IIC_SDA_GPIO_RCC	RCC_APB2Periph_GPIOA
#define	IIC_SDA_GPIO_PORT	GPIOA
#define	IIC_SDA_GPIO_PIN	GPIO_Pin_1

#define 	IIC_GPIO_RCC 	IIC_SCL_GPIO_RCC|IIC_SDA_GPIO_RCC

#define SDA             GPIO_ReadInputDataBit(IIC_SDA_GPIO_PORT, IIC_SDA_GPIO_PIN)

#define SDA0()          GPIO_ResetBits(IIC_SDA_GPIO_PORT, IIC_SDA_GPIO_PIN)		//IO口输出低电平
#define SDA1()          GPIO_SetBits(IIC_SDA_GPIO_PORT, IIC_SDA_GPIO_PIN)		//IO口输出高电平  
#define SCL0()          GPIO_ResetBits(IIC_SCL_GPIO_PORT, IIC_SCL_GPIO_PIN)		//IO口输出低电平
#define SCL1()          GPIO_SetBits(IIC_SCL_GPIO_PORT, IIC_SCL_GPIO_PIN)		//IO口输出高电平

typedef enum IIC       //IIC模块
{
  IIC,
  SCCB
} IIC_type;

void IIC_Config(void);
void IIC_start(void);
void IIC_stop(void);
void IIC_ack_main(unsigned char ack_main);
void send_ch(unsigned char c);
unsigned char read_ch(unsigned char ack);
void iic_write_reg(unsigned char dev_add, unsigned char reg, unsigned char dat);
unsigned char iic_read_reg(unsigned char dev_add, unsigned char reg, IIC_type type);
void iic_read_regs(unsigned char dev_add, unsigned char reg, int *dat_add, unsigned char num, IIC_type type);

#endif/*SJ_IIC_H*/

//
//  Author:	SaleJuice
//  Laboratory:	CyberSmartCar
//  School:	CJLU
//

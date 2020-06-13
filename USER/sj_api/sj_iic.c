//
//  sj_iic.c
//

#include "sj_iic.h"

//内部数据定义
unsigned char IIC_ad_main; //器件从地址
unsigned char IIC_ad_sub;  //器件子地址
unsigned char *IIC_buf;    //发送|接收数据缓冲区
unsigned char IIC_num;     //发送|接收数据个数

#define ack 1      //主应答
#define no_ack 0   //从应答	 

//-------------------------------------------------------------------------------------------------------------------
//  @brief      模拟IIC延时
//  @return     void
//  @since      v1.0
//  Sample usage:				如果IIC通讯失败可以尝试增加j的值
//-------------------------------------------------------------------------------------------------------------------
void iic_delay(void)
{
  //j=10通讯速率大约为100K （内核频率40M）
  //j=0 通讯速率大约为140K （内核频率40M）
  unsigned short j = 0;
  while (j--);
}


//内部使用，用户无需调用
void IIC_start(void)//iicDevStart
{
  SDA1();
  SCL1();
  iic_delay();
  SDA0();
  iic_delay();
  SCL0();
}

//内部使用，用户无需调用
void IIC_stop(void)//iicDevStop
{
  SDA0();
  SCL0();
  iic_delay();
  SCL1();
  iic_delay();
  SDA1();
  iic_delay();
}

//主应答(包含ack:SDA=0和no_ack:SDA=0)
//内部使用，用户无需调用
void I2C_SendACK(unsigned char ack_dat)//iicDevAck
{
  SCL0();
  iic_delay();
  if (ack_dat) SDA0();
  else    	SDA1();

  SCL1();
  iic_delay();
  SCL0();
  iic_delay();
}


static int SCCB_WaitAck(void)//iicDevWaitAck
{
  SCL0();
  //DIR_IN();
  iic_delay();

  SCL1();
  iic_delay();

  if (SDA)          //应答为高电平，异常，通信失败
  {
    //DIR_OUT();
    SCL0();
    return 0;
  }
  //DIR_OUT();
  SCL0();
  iic_delay();
  return 1;
}

//字节发送程序
//发送c(可以是数据也可是地址)，送完后接收从应答
//不考虑从应答位
//内部使用，用户无需调用
void send_ch(unsigned char c)//iicDevSendByte
{
  unsigned char i = 8;
  while (i--)
  {
    if (c & 0x80)	SDA1(); //SDA 输出数据
    else			SDA0();
    c <<= 1;
    iic_delay();
    SCL1();                //SCL 拉高，采集信号
    iic_delay();
    SCL0();                //SCL 时钟线拉低
  }
  SCCB_WaitAck();
}

//字节接收程序
//接收器件传来的数据，此程序应配合|主应答函数|IIC_ack_main()使用
//内部使用，用户无需调用
unsigned char read_ch(unsigned char ack_x)//iicDevReadByte
{
  unsigned char i;
  unsigned char c;
  c = 0;
  SCL0();
  iic_delay();
  SDA1();             //置数据线为输入方式
  //DIR_IN();
  for (i = 0; i < 8; i++)
  {
    iic_delay();
    SCL0();         //置时钟线为低，准备接收数据位
    iic_delay();
    SCL1();         //置时钟线为高，使数据线上数据有效
    iic_delay();
    c <<= 1;
    if (SDA) c += 1; //读数据位，将接收的数据存c
  }
  //DIR_OUT();
  SCL0();
  iic_delay();
  I2C_SendACK(ack_x);

  return c;
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      模拟IIC写数据到设备寄存器函数
//  @param      dev_add			设备地址(低七位地址)
//  @param      reg				寄存器地址
//  @param      dat				写入的数据
//  @return     void
//  @since      v1.0
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void IIC_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(IIC_GPIO_RCC, ENABLE);	/* 打开GPIO时钟 */
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;  	/* 开漏输出 */
  GPIO_InitStructure.GPIO_Pin = IIC_SCL_GPIO_PIN;
	GPIO_Init(IIC_SCL_GPIO_PORT, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = IIC_SDA_GPIO_PIN;
	GPIO_Init(IIC_SDA_GPIO_PORT, &GPIO_InitStructure);

	/* 给一个停止信号, 复位I2C总线上的所有设备到待机模式 */
	IIC_stop();
}

void iic_write_reg(unsigned char dev_add, unsigned char reg, unsigned char dat)
{
  IIC_start();
  send_ch( (dev_add << 1) | 0x00); //发送器件地址加写位
  send_ch( reg );   				 //发送从机寄存器地址
  send_ch( dat );   				 //发送需要写入的数据
  IIC_stop();
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      模拟IIC从设备寄存器读取数据
//  @param      dev_add			设备地址(低七位地址)
//  @param      reg				寄存器地址
//  @param      type			选择通信方式是IIC  还是 SCCB
//  @return     uint8			返回寄存器的数据
//  @since      v1.0
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
unsigned char iic_read_reg(unsigned char dev_add, unsigned char reg, IIC_type type)
{
  unsigned char dat;
  IIC_start();
  send_ch( (dev_add << 1) | 0x00); //发送器件地址加写位
  send_ch( reg );   				//发送从机寄存器地址
  if (type == SCCB)IIC_stop();

  IIC_start();
  send_ch( (dev_add << 1) | 0x01); //发送器件地址加读位
  dat = read_ch(no_ack);   				//读取数据
  IIC_stop();

  return dat;
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      模拟IIC读取多字节数据
//  @param      dev_add			设备地址(低七位地址)
//  @param      reg				寄存器地址
//  @param      dat_add			数据保存的地址指针
//  @param      num				读取字节数量
//  @param      type			选择通信方式是IIC  还是 SCCB
//  @return     uint8			返回寄存器的数据
//  @since      v1.0
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void iic_read_regs(unsigned char dev_add, unsigned char reg, int *dat_add, unsigned char num, IIC_type type)
{
  IIC_start();
  send_ch( (dev_add << 1) | 0x00); //发送器件地址加写位
  send_ch( reg );   				//发送从机寄存器地址
  if (type == SCCB)IIC_stop();

  IIC_start();
  send_ch( (dev_add << 1) | 0x01); //发送器件地址加读位
  while (--num)
  {
    *dat_add = read_ch(ack); //读取数据
    dat_add++;
  }
  *dat_add = read_ch(no_ack); //读取数据
  IIC_stop();
}

//
//  Author:	SaleJuice
//  Laboratory:	CyberSmartCar
//  School:	CJLU
//

//
// main.c
//

#include "stm32f10x.h"
#include "sj_headfile.h"

//定义主机主循环状态机
typedef enum
{
  GET_INFO,        //传感器获取信息
  ANALYZE_INFO,    //处理采集到的信息
  OUTPUT_INFO,     //输出计算后的信息
  SHOW_INFO,       //显示信息
  SEND_INFO,       //发送信息
}MainStatusNode;

//初始化循环状态机
MainStatusNode MainStatus = GET_INFO;

//硬件初始化
void HardWareInit(void)
{
	OLED_Config(); 		//OLED初始化
	KEY_Config();		//KEY初始化
	LED_Config();		//LED初始化
	LED_Operate(-1,0);	//关闭所有LED灯
	PWM_Config();			//PWM初始化
	IIC_Config(); 		//IIC初始化
	MPU9250_Config(); 	//MPU9250初始化
	BMP280_Config();	//BMP280初始化
}

//软件初始化
void SoftWareInit(void)
{
	TIM_Config();	//TIM初始化
	USART_Config();	//UART初始化
	//PPM_Config(); //PPM信号输入捕获
	CyberDataInit();//系统参数初始化
	PidDataInit();
	ControlArithmeticInit();
	
}

//主函数
int main(void)
{
	//系统初始化
	HardWareInit();//硬件初始化
	SoftWareInit();//软件初始化

	//主循环函数
	while(1)
	{
		//主函数状态机
		switch (MainStatus)
		{
		case GET_INFO:
			/* 传感器获取信息 */
			//ModuleDataGetAndHandle();
			MainStatus = ANALYZE_INFO;
			break;
		
		case ANALYZE_INFO:
			/* 处理采集到的信息 */
			//ControlArithmeticDeal();
			MainStatus = OUTPUT_INFO;
			break;
		
		case OUTPUT_INFO:
			/* 输出计算后的信息 */
			//MotorPwmOutputControl();
			MainStatus = SHOW_INFO;
			break;
		
		case SHOW_INFO:
			/* 显示信息 */
			OledShowDebugFace();
			MainStatus = SEND_INFO;
			break;
		
		case SEND_INFO:
			/* 发送信息 */
			UartDataGetAndSend();
			MainStatus = GET_INFO;
			break;
		}
		//系统时间累计
		//Cyber.SysTime++;
	}
}

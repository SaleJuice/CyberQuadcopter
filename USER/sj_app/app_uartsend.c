//
//  app_uartsend.c
//

#include "app_uartsend.h"

float senddata[4];

u8 TIM4_CH1_CAPTURE_STA=0;
u16 TIM4_CH1_CAPTURE_VAL;
u8 TIM4_CH1_CAPTURE_TIMES;

void UartInit(void)
{
  /*Serial.begin(115200);
  Serial1.begin(115200);
  Serial2.begin(115200);
  Serial3.begin(115200);*/
}

void uart_send_char(USART_TypeDef* USARTx,uint8_t c)
{
	while(USART_GetFlagStatus(USARTx,USART_FLAG_TXE)==RESET);
	USART_SendData(USARTx,c);
}

//
void uart_putbuff(USART_TypeDef* USARTx,uint8_t *buff, uint32_t len)
{
    while(len--)
    {
        uart_send_char(USARTx,*buff);
        buff++;
    }
}

void vcan_sendware(USART_TypeDef* USARTx,uint8_t *wareaddr, uint32_t waresize)
{
    uint8_t cmdf[2] = {0x03, 0xfc};
    uint8_t cmdr[2] = {0xfc, 0x03};

    uart_putbuff(USARTx,cmdf, sizeof(cmdf));
    uart_putbuff(USARTx,wareaddr, waresize);
    uart_putbuff(USARTx,cmdr, sizeof(cmdr));
}

void UartDataGetAndSend(void)
{
	/*senddata[0]=Pitch.Gyro;
	senddata[1]=Pitch.GyroAngle;
	senddata[2]=Pitch.AccAngle;
	senddata[3]=Pitch.Angle;*/
	/*senddata[0]=HighGPid.error_curr * HighGPid.kp;
	senddata[1]=HighGPid.sum_error * HighGPid.ki;
	senddata[2]=HighGPid.cc * HighGPid.kd;
	senddata[3]=NavigationMot.z - 400;*/
	/*senddata[0]=posturedata.realG.y;
	senddata[1]=posturedata.realA.y;
	senddata[2]=PostureMot.y;
	senddata[3]=NavigationMot.z;*/
	/*senddata[0]=navigationdata.realA.z;
	senddata[1]=navigationdata.realG.z;
	senddata[2]=navigationdata.realC.z;
	senddata[3]=NavigationMot.z;*/
	senddata[0]=posturedata.expectA.r;
	senddata[1]=posturedata.expectA.p;
	senddata[2]=opticalflowdata.delta_x;
	senddata[3]=opticalflowdata.delta_y;
	vcan_sendware(REMOTE_USARTx,(uint8_t*)senddata,sizeof(senddata));
	
	/*if(opticalflowdata.receive_flag)
	{
		printf("%d:",strlen(OpticalFlowBuff));
		printf(OpticalFlowBuff);
		opticalflowdata.receive_flag=0;
	}*/
	
	/* ‰»Î≤∂ªÒ*/
	/*u8 i;
	u8 flag=0;
	u32 indata;
	
	if((TIM4_CH1_CAPTURE_STA&0x80)) //????
	{
			indata = TIM4_CH1_CAPTURE_TIMES&0x3f;
			indata *=0xffff;
			indata += TIM4_CH1_CAPTURE_VAL;
			printf("hightime: %d us\r\n",indata);
			TIM4_CH1_CAPTURE_STA = 0;
	}
	i++;
	if(i%20==0)
	{
		if(flag==0)
			flag=1;
		else
			flag=0;
		LED_Operate(4,flag);
	}*/
}

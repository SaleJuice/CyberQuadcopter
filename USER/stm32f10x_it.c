/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.4.0
  * @date    10/15/2010
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @copy
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2010 STMicroelectronics</center></h2>
  */ 

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "stm32f10x_it.h"
#include "sj_headfile.h"

/** @addtogroup STM32F10x_StdPeriph_Template
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief   This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
}

/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

//总定时器中断服务函数
void  GENERAL_TIM_IRQHandler (void)
{
	if ( TIM_GetITStatus(GENERAL_TIM, TIM_IT_Update) != RESET ) 
	{
		LED_Operate(1,1);
    Cyber.SysTime++;
    Key_AddTimeAfterLast();
		ModuleDataGetAndHandle();
		ControlArithmeticDeal();
		MotorPwmOutputControl();
		LED_Operate(1,0);
		TIM_ClearITPendingBit(GENERAL_TIM , TIM_FLAG_Update);
	}		 	
}

//调试串口中断服务函数
void DEBUG_USART_IRQHandler(void)
{
  uint8_t ucTemp;
	if(USART_GetITStatus(DEBUG_USARTx,USART_IT_RXNE) != RESET)
	{
		ucTemp = USART_ReceiveData(DEBUG_USARTx);
		USART_SendData(DEBUG_USARTx,ucTemp);  
	}	 
}

//遥控器串口中断服务函数
void REMOTE_USART_IRQHandler(void)
{
	static int i=0;
	static char rebuf[30];
	if(USART_GetITStatus(REMOTE_USARTx,USART_IT_RXNE) != RESET)
	{
		LED_Operate(2,1);
		rebuf[i] = USART_ReceiveData(REMOTE_USARTx);
		if(rebuf[i] == '\n')
		{
			rebuf[i]='\0';
			i=0;
			if(!remotedata.receive_flag)
			{
				memcpy(RemoteBuff,rebuf,30);
				remotedata.receive_flag=1;
			}
		}
    else
			i++;
		LED_Operate(2,0);
	}
}

//测距模块串口中断服务函数
void DISTANCE_USART_IRQHandler(void)
{
	static uint8_t i=0,rebuf[20]={0};
	if(USART_GetITStatus(DISTANCE_USARTx,USART_IT_RXNE) != RESET)
	{
		LED_Operate(4,1);
		rebuf[i++]=USART_ReceiveData(DISTANCE_USARTx);
		if (rebuf[0]!=0x5a)
			i=0;	
		if ((i==2)&&(rebuf[1]!=0x5a))
			i=0;
	
		if(i>3)
		{
			if(i!=(rebuf[3]+5))
				return;	
			switch(rebuf[2])
			{
				case 0x15:
					if(!laserdata.receive_flag)
					{
						memcpy(DistanceBuff,rebuf,8);
						laserdata.receive_flag=1;
					}
					break;
			
			}
			i=0;
		}
		LED_Operate(4,0);
	}
}

//光流模块串口中断服务函数
void OPTICALFLOW_USART_IRQHandler(void)
{
	const char packet_ID[4] = {'#', 'J', 'B', '#'};
	static int rc_counter = 0;
	signed char sum=0;
	char rdata;
	if(USART_GetITStatus(OPTICALFLOW_USARTx,USART_IT_RXNE) != RESET)
	{
		LED_Operate(3,1);
		rdata=USART_ReceiveData(OPTICALFLOW_USARTx);
		if (rc_counter < 4)
		{
			if (rdata == packet_ID[rc_counter])
			{
				rc_counter++;
				sum = 0;
			}
			else
			{
				rc_counter = 0;
			}
		}
		else if ( rc_counter <= 17 )
		{
			( (unsigned char*)&Flow )[rc_counter - 4] = rdata;
			sum += (signed char)rdata;
			rc_counter++;
		}
		else if ( rc_counter == 18 )
		{
			if ( sum != 0 || rdata != '\r' )
				rc_counter = 0;
			else
				rc_counter++;
		}
		else
		{
			if ( rdata == '\n' )
			{
				if(!opticalflowdata.receive_flag)
					opticalflowdata.receive_flag=1;
			}
			rc_counter = 0;
		}
		LED_Operate(3,0);
	}
}

/*PPM信号输入捕获*/
/*
void TIM4_IRQHandler()
{
    //3*
    if((TIM4_CH1_CAPTURE_STA&0x80)==0)
    {
        
        //1*
        if(TIM_GetITStatus(TIM4,TIM_IT_CC1)==1)
        {
            if((TIM4_CH1_CAPTURE_STA&0x40)==0)
            {
                TIM4_CH1_CAPTURE_STA=0;
                TIM4_CH1_CAPTURE_VAL=0;
                TIM4_CH1_CAPTURE_STA |= 0x40;
                
                TIM_Cmd(TIM4,DISABLE);
                TIM_SetCounter(TIM4,0);
                TIM_OC1PolarityConfig(TIM4,TIM_ICPolarity_Falling);
                TIM_Cmd(TIM4,ENABLE);
                
            }
            else
            {
                TIM4_CH1_CAPTURE_STA |= 0x80;
                TIM4_CH1_CAPTURE_VAL = TIM_GetCapture1(TIM4);
                TIM_OC1PolarityConfig(TIM4,TIM_ICPolarity_Rising);
                
            }
        }
        
        //2*
        if(TIM_GetITStatus(TIM4,TIM_IT_Update)==1)
        {
            if(TIM4_CH1_CAPTURE_STA&0x40)
            {           
                if((TIM4_CH1_CAPTURE_TIMES&0x3f)==0x3f)
                {
                    TIM4_CH1_CAPTURE_STA |= 0x80;
                    TIM4_CH1_CAPTURE_VAL = 0xffff;
                }
                else
                {                                                                                                                                                                                                                                                                                               
                    TIM4_CH1_CAPTURE_TIMES++;
                }
            }
        }
        
    }
    TIM_ClearITPendingBit(TIM4,TIM_IT_CC1|TIM_IT_Update);
}
*/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 


/******************* (C) COPYRIGHT 2010 STMicroelectronics *****END OF FILE****/

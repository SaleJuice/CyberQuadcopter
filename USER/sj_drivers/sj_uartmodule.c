//
//  sj_uartmodule.cpp
//

#include "sj_uartmodule.h"

//遥控器总结构体
_REMOTEDATA_ remotedata;
char RemoteBuff[30];

//摄像头总结构体
_CAMERADATA_ cameradata;
char CameraStr[30];

//测距总结构体
_DISTANCEDATA_ laserdata;
uint8_t DistanceBuff[8]={0};

float fusespeed,laserspeed,acczspeed,onlyacczspeed;

#define COUNT_CYCLE 0.005 //秒//0.013785

//光流总结构体
_OpticalFlow_Packed_ Flow;
_OPTICALFLOWDATA_ opticalflowdata;
const char packet_ID[4] = {'#', 'J', 'B', '#'};
char OpticalFlowBuff[30];

//遥控器部分
int RemoteDataCheck(char c[])
{
  int c1 = 0, c2 = 0, c3 = 0;
  if (strlen(c) > 20)
    return 0;
  for (int i = 0; i < strlen(c); i++)
  {
    if (c[i] == '(')
      c1++;
    else if (c[i] == ',')
      c2++;
    else if (c[i] == ')')
      c3++;
  }
  if (c1 == 1 && c2 == 3 && c3 == 1)
    return 1;
  else
    return 0;
}

void AnalysisRemoteData(char c[], int* data)
{
  char numStr[50];
	int numi=0;
  int flag = 0;

  for (int i = 0; i < strlen(c); i++)
  {
    if (c[i] == '(')
    {
			numi=0;
    }
    else if (c[i] == ',' && flag == 0)
    {
			numStr[numi] = '\0';
      data[0] = atoi(numStr);
      numi=0;
      flag = 1;
    }
    else if (c[i] == ',' && flag == 1)
    {
			numStr[numi] = '\0';
      data[1] = atoi(numStr);
      numi=0;
      flag = 2;
    }
    else if (c[i] == ',' && flag == 2)
    {
			numStr[numi] = '\0';
      data[2] = atoi(numStr);
      numi=0;
      flag = 3;
    }
    else if (c[i] == ')' && flag == 3)
    {
			numStr[numi] = '\0';
      data[3] = atoi(numStr);
      numi=0;
      flag = 4;
    }
    else
    {
      numStr[numi++] = c[i];
    }
  }
}

void REMOTEDATA_DataGet(char buff[])
{
	if(remotedata.receive_flag)//串口接收完毕
	{
		if (RemoteDataCheck(buff))
			AnalysisRemoteData(buff, remotedata.channel);
		remotedata.receive_flag=0;//处理数据完毕标志
	}
}
void highspeeddatafuse(float laser,float acc_z)
{
	static float last_laser,last_laserspeed;
	//float fusespeed,laserspeed,acczspeed,onlyacczspeed;
	float a = 0.99,b=0.5;//0.7

	onlyacczspeed += acc_z;
	acczspeed = fusespeed + acc_z * COUNT_CYCLE;
	laserspeed = (laser - last_laser) / COUNT_CYCLE;
	if(laserspeed==0)
		laserspeed = last_laserspeed;
	laserspeed = b * laserspeed+(1.0f - b) * last_laserspeed;
	fusespeed = a * acczspeed + (1.0f - a) * laserspeed;
	
	last_laser = laser;
	last_laserspeed = laserspeed;
}

void DISTANCEDATA_DataGet(uint8_t buff[])
{
	u8 sum=0,i=0;
	if(laserdata.receive_flag)//串口接收完毕
	{
		for(sum=0,i=0;i<(buff[3]+4);i++)
			sum+=buff[i];
		if(sum==buff[i])//校验和判断
		{
			laserdata.distance=(float) (buff[4]<<8|buff[5]);
			laserdata.distance = laserdata.distance / 1000.0f;
		}
		laserdata.receive_flag=0;//处理数据完毕标志
	}
}

//光流部分
/*void OpticalFlowDataRead(void)
{
  if (Serial2.available() > 0)
  {
    int rc_counter = 0;
    signed char sum = 0;
    //状态机接收数据
    while (Serial2.available() > 0)
    {
      //接收
      char rdata = char(Serial2.read());

      if (rc_counter < 4)
      {
        //接收包头
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
        }
        rc_counter = 0;
      }
    }
  }
}*/

void OPTICALFLOW_DataGet(float anglep,float angler,float height)
{
	static float last_anglep,last_angler;
	static float last_deltax,last_deltay;
	float delta_anglep,delta_angler;
	opticalflowdata.ka = 2.5f;
	opticalflowdata.km = 1.0f;
	if(opticalflowdata.receive_flag)
	{
		delta_anglep = anglep - last_anglep;
		delta_angler = angler - last_angler;
		Flow.Delta_X = 0.7f * Flow.Delta_X + (1.0f - 0.7f) * last_deltax;
		Flow.Delta_Y = 0.7f * Flow.Delta_Y + (1.0f - 0.7f) * last_deltay;
		opticalflowdata.delta_x = (Flow.Delta_X * opticalflowdata.km * height);
		opticalflowdata.delta_y = (Flow.Delta_Y * opticalflowdata.km * height);
		
		opticalflowdata.quality = Flow.Quality;
		
		last_anglep = anglep;
		last_angler = angler;
		last_deltax = Flow.Delta_X;
		last_deltay = Flow.Delta_Y;
		opticalflowdata.receive_flag=0;
	}
}

/*
//摄像头部分
int CameraDataCheck(String c)
{
  int flag = 0, c1 = 0, c2 = 0, c3 = 0;
  if (c.length() > 20)
    return 0;
  for (int i = 0; i < c.length(); i++)
  {
    if (c[i] == '(')
      c1++;
    else if (c[i] == ',')
      c2++;
    else if (c[i] == ')')
      c3++;
  }
  if (c1 == 1 && c2 == 2 && c3 == 1)
    return 1;
  else
    return 0;
}

void AnalysisCameraData(String c, int* data)
{
  String numStr = "";
  static int flag = 0;

  for (int i = 0; i < c.length(); i++)
  {
    if (c[i] == '(')
    {
      numStr = "";
    }
    else if (c[i] == ',' && flag == 0)
    {
      data[0] = numStr.toInt();
      numStr = "";
      flag = 1;
    }
    else if (c[i] == ',' && flag == 1)
    {
      data[1] = numStr.toInt();
      numStr = "";
      flag = 2;
    }
    else if (c[i] == ',' && flag == 2)
    {
      data[2] = numStr.toInt();
      numStr = "";
      flag = 0;
    }
    else if (c[i] == ')')
    {
      data[3] = numStr.toInt();
      numStr = "";
      flag = 0;
    }
    else
    {
      numStr += c[i];
    }
  }
}

void CAMERADATA_DataGet(void)
{
  int dat[4];
  //串口3获取摄像头信号
  if (Serial3.available() > 0)
  {
    CameraStr = (Serial3.readStringUntil('\n'));
    CameraStr = CameraStr.substring(0, CameraStr.length() - 2);
    if (CameraDataCheck(CameraStr))
      AnalysisCameraData(CameraStr, dat);
  }
  cameradata.target_get = dat[0];
  cameradata.target_x = dat[1];
  cameradata.target_y = dat[2];
  cameradata.target_area = dat[3];
}*/

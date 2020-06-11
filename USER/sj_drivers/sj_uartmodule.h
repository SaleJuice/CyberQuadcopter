//
//  sj_uartmodule.h
//

#ifndef SJ_UARTMODULE_H 
#define SJ_UARTMODULE_H

#include "sj_headfile.h"

typedef struct
{
  int channel[4];
	int receive_flag;
} _REMOTEDATA_;

typedef struct
{
  float distance;
	float last_distance;
	int receive_flag;
} _DISTANCEDATA_;

typedef struct
{
  unsigned char Reserved;
  unsigned char Quality;
  signed char Delta_X;
  signed char Delta_Y;
  unsigned char Reference;
  unsigned char Lightness;
  signed char Delta_Z;
  signed char Sub_X;
  signed char Sub_Y;
  unsigned char Timestamp[4];
  signed char Checksum;
  unsigned char Packet_footer[2];
} _OpticalFlow_Packed_;

typedef struct
{
  float delta_x;
  float delta_y;
	float sum_x;
  float sum_y;
  float ka;
  float km;
  int quality;
	int receive_flag;
} _OPTICALFLOWDATA_;

typedef struct
{
  int target_get;
  int target_x;
  int target_y;
  int target_area;
} _CAMERADATA_;

extern _REMOTEDATA_ remotedata;
extern _CAMERADATA_ cameradata;
extern _OPTICALFLOWDATA_ opticalflowdata;
extern _OpticalFlow_Packed_ Flow;

extern char RemoteBuff[30];
extern char CameraStr[30];
extern char OpticalFlowBuff[30];

extern _DISTANCEDATA_ laserdata;
extern uint8_t DistanceBuff[8];

extern float fusespeed,laserspeed,acczspeed,onlyacczspeed;

extern void highspeeddatafuse(float laser,float acc_z);
extern void REMOTEDATA_DataGet(char buff[]);//外部调用，通过_REMOTEDATA_结构体读取信息
extern void CAMERADATA_DataGet(void);//外部调用，通过_CAMERADATA_结构体读取信息
extern void DISTANCEDATA_DataGet(uint8_t buff[]);
extern void OPTICALFLOW_DataGet(float anglep,float angler,float height);//外部调用，通过_OPTICALFLOWDATA_结构体读取信息

#endif /*SJ_UARTMODULE_H*/

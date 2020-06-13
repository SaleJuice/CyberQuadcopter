//
//  app_dataget.h
//

#ifndef app_dataget_h
#define app_dataget_h

#include "sj_headfile.h"

typedef struct
{
  _PYR_Float_ realA;
  _PYR_Float_ expectA;
  _PYR_Float_ offsetA;

  _PYR_Float_ realG;
  _PYR_Float_ expectG;
  _PYR_Float_ offsetG;
} _POSTUREDATA_;

typedef struct
{
  _XYZ_Float_ realA;
  _XYZ_Float_ expectA;
  _XYZ_Float_ offsetA;

  _XYZ_Float_ realG;
  _XYZ_Float_ expectG;
  _XYZ_Float_ offsetG;
	
	_XYZ_Float_ realC;
  _XYZ_Float_ expectC;
  _XYZ_Float_ offsetC;
} _NAVIGATIONDATA_;

typedef struct
{
  int SysTime;
  int StopFlag;
} _CYBERCOREDATA_;

extern _CYBERCOREDATA_ Cyber;
extern _POSTUREDATA_ posturedata;
extern _NAVIGATIONDATA_ navigationdata;

extern void CyberDataInit(void);
extern void ModuleDataGetAndHandle(void);

#endif/*app_dataget_h*/

//
//  Author:	SaleJuice
//  Laboratory:	CyberSmartCar
//  School:	CJLU
//

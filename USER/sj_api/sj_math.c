//
//  File:	sj_math.c
//

#include "sj_math.h"

//快速开平方根
float invSqrt(float x)
{
  float halfx = 0.5f * x;
  float y = x;
  long i = *(long*)&y;  
  i = 0x5f3759df - (i>>1);
  y = *(float*)&i;
  y = y * (1.5f - (halfx * y * y));
  return y;
}

//幅度限制
void rangelimitInt(int* x, int min, int max)
{
  if(*x <=  min)
    *x = min;
  else if(*x >= max)
    *x = max;
}

void rangelimitFloat(float* x, float min, float max)
{
  if(*x <=  min)
    *x = min;
  else if(*x >= max)
    *x = max;
}

//
//  Author:	SaleJuice
//  Laboratory:	CyberSmartCar
//  School:	CJLU
//

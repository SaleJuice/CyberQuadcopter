//
//  sj_headfile.h
//

#ifndef SJ_HEADFILE_H
#define SJ_HEADFILE_H

typedef struct
{
  int p;//pitch
  int y;//yaw
  int r;//roll
} _PYR_Int_;

typedef struct
{
  float p;//pitch
  float y;//yaw
  float r;//roll
} _PYR_Float_;

typedef struct
{
  int x;//x轴向-俯仰
  int y;//y轴向-横滚
  int z;//z轴向-高度
} _XYZ_Int_;

typedef struct
{
  float x;//x轴向-俯仰
  float y;//y轴向-横滚
  float z;//z轴向-高度
} _XYZ_Float_;

//self
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

//api
#include "sj_iic.h"
#include "sj_pid.h"
#include "sj_filter.h"
#include "sj_math.h"

//driver
#include "sj_led.h"
#include "sj_key.h"
#include "sj_pwm.h"
#include "sj_uart.h"
#include "sj_tim.h"
#include "sj_ppm.h"
#include "sj_oled.h"
#include "sj_mpu9250.h"
#include "sj_bmp280.h"
#include "sj_uartmodule.h"

//app
#include "app_dataget.h"
#include "app_mcudeal.h"
#include "app_motorctrl.h"
#include "app_showdebug.h"
#include "app_uartsend.h"

#endif/*SJ_HEADFILE_H*/

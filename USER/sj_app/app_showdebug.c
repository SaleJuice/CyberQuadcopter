//
//  app_showdebug.c
//

#include "app_showdebug.h"
static int menus = 0, lines = 0;

void MenuDataChange()
{
  /*------按键解析------*/
  switch (Key_Scanf())
  {
    case _KEY_NONE_: break;
    case _KEY_START_:
      {
        if (Cyber.StopFlag == 0)
        {
          Cyber.StopFlag = 1;
        }
        else
        {
					for(int i=0;i<2000;i++)
					{
						for(int j=0;j<10000;j++)
							;
					}
          Cyber.StopFlag = 0;
        }
      }
      break;
    case _KEY_ADD_PAGE_:
      menus++;        //换下一页
      if (menus > 2)
        menus = 0;
      OLED_Config();
      break;
    case _KEY_UP_LINE_:
      lines--;
      if (lines < 1)
        lines = 7;
      OLED_Cls();
      break;
    case _KEY_DOWN_LINE_:
      lines++;
      if (lines > 7)
        lines = 1;
      OLED_Cls();
      break;
    case _KEY_ADD_NUM_: break;
    case _KEY_POOR_NUM_: break;
  }
}

void OledShowMessage()
{
  if (menus == 0)
  {
    OLED_P6x8Str(0, 0, "P1:");       OLED_P6x8Int(0, 15, Cyber.StopFlag,5);
    OLED_P6x8Str(1, 0, "PitchA");    OLED_P6x8Float(1, 15, posturedata.realA.p, 5,2);
    OLED_P6x8Str(2, 0, "YawA");      OLED_P6x8Float(2, 15, posturedata.realA.y, 5,2);
    OLED_P6x8Str(3, 0, "RollA");     OLED_P6x8Float(3, 15, posturedata.realA.r, 5,2);
    OLED_P6x8Str(4, 0, "PitchG");    OLED_P6x8Float(4, 15, posturedata.realG.p, 5,2);
    OLED_P6x8Str(5, 0, "YawG");      OLED_P6x8Float(5, 15, posturedata.realG.y, 5,2);
    OLED_P6x8Str(6, 0, "RollG");     OLED_P6x8Float(6, 15, posturedata.realG.r, 5,2);
    OLED_P6x8Str(7, 0, "SysTime");   OLED_P6x8Int(7, 15, Cyber.SysTime,5);
  }
  else if (menus == 1)
  {
    OLED_P6x8Str(0, 0, "P2:");       OLED_P6x8Int(0, 15, Cyber.StopFlag,5);
    OLED_P6x8Str(1, 0, "Mot_P");     OLED_P6x8Int(1, 15, PostureMot.p,5);
    OLED_P6x8Str(2, 0, "Mot_Y");     OLED_P6x8Int(2, 15, PostureMot.y,5);
    OLED_P6x8Str(3, 0, "Mot_R");     OLED_P6x8Int(3, 15, PostureMot.r,5);
    OLED_P6x8Str(4, 0, "Mot_F");     OLED_P6x8Int(4, 15, NavigationMot.z,5);
    OLED_P6x8Str(5, 0, "bmpdata");   OLED_P6x8Float(5, 15, bmpdata.altitude,5, 2);
    OLED_P6x8Str(6, 0, "na_z");      OLED_P6x8Float(6, 15, navigationdata.realA.z,5, 2);
    OLED_P6x8Str(7, 0, "SysTime");   OLED_P6x8Int(7, 15, Cyber.SysTime,5);
  }
  else if (menus == 2)
  {
    OLED_P6x8Str(0, 0, "P3:");       OLED_P6x8Int(0, 15, Cyber.StopFlag,5);
    OLED_P6x8Str(1, 0, "exp_P");     OLED_P6x8Int(1, 15, remotedata.channel[0],5);
    OLED_P6x8Str(2, 0, "exp_Y");     OLED_P6x8Int(2, 15, remotedata.channel[1],5);
    OLED_P6x8Str(3, 0, "exp_R");     OLED_P6x8Int(3, 15, remotedata.channel[2],5);
    OLED_P6x8Str(4, 0, "exp_H");     OLED_P6x8Int(4, 15, remotedata.channel[3],5);
    OLED_P6x8Str(5, 0, "opflow_x");  OLED_P6x8Float(5, 15, navigationdata.realG.x,5,2);
    OLED_P6x8Str(6, 0, "opflow_y");  OLED_P6x8Float(6, 15, navigationdata.realG.y,5,2);
    OLED_P6x8Str(7, 0, "SysTime");   OLED_P6x8Int(7, 15, Cyber.SysTime,5);
  }
}

void DebugDataPart()
{
  if (menus == 0)
  {
    switch (lines)
    {
    }
  }
  else if (menus == 1)
  {
    switch (lines)
    {
    }
  }
  else if (menus == 2)
  {
    switch (lines)
    {
    }
  }
}

void OledShowDebugFace(void)
{
  MenuDataChange();
  OledShowMessage();
  DebugDataPart();
}

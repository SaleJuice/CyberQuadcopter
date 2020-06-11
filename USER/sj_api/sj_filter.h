//
//  sj_filter.h
//

#ifndef SJ_FILTER_H
#define SJ_FILTER_H

#define PI 3.141592653589793f

typedef struct
{
 float Input_Butter[3];
 float Output_Butter[3];
}Butter_BufferData;

typedef struct
{
  float a[3];
  float b[3];
}Butter_Parameter;

extern Butter_Parameter Bandstop_Filter_Parameter_30_98;
extern Butter_Parameter Bandstop_Filter_Parameter_30_94;
extern Butter_Parameter Notch_Filter_Parameter_55hz;

extern void Set_Cutoff_Frequency(float sample_frequent, float cutoff_frequent,Butter_Parameter *LPF);
extern float LPButterworth(float curr_input,Butter_BufferData *Buffer,Butter_Parameter *Parameter);

#endif/*SJ_FILTER_H*/

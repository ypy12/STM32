#ifndef __FREQ_H
#define __FREQ_H
#include "sys.h"

#define PI 3.1415926

extern u8  TIM5CH1_CAPTURE_STA;		//输入捕获状态		    				
extern u32	TIM5CH1_CAPTURE_VAL;	//输入捕获值	
extern u8 Show_flag;
extern float Freq_value;

extern void TIM5_Cap_Init(u16 arr,u16 psc);
extern void show_frequency(void);
extern void Freq_Sample(void);

#endif

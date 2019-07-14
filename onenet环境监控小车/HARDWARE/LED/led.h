#ifndef __LED_H
#define __LED_H	 
#include "sys.h"


#define LED_ON    GPIO_ResetBits(GPIOB,GPIO_Pin_9)
#define LED_OFF   GPIO_SetBits(GPIOB,GPIO_Pin_9)
#define LED      PBout(9)

void LED_Init(void);//≥ı ºªØ
		 				    
#endif

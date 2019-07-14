#ifndef __CAR_H
#define __CAR_H

#include"stm32f10x.h"

#define in1_set GPIO_SetBits(GPIOB,GPIO_Pin_12);
#define in1_clr GPIO_ResetBits(GPIOB,GPIO_Pin_12);
#define in2_set GPIO_SetBits(GPIOB,GPIO_Pin_13);
#define in2_clr GPIO_ResetBits(GPIOB,GPIO_Pin_13);
#define in3_set GPIO_SetBits(GPIOB,GPIO_Pin_14);
#define in3_clr GPIO_ResetBits(GPIOB,GPIO_Pin_14);
#define in4_set GPIO_SetBits(GPIOB,GPIO_Pin_15);
#define in4_clr GPIO_ResetBits(GPIOB,GPIO_Pin_15);


void Car_Init(void);
void go(void);
void back(void);
void car_stop(void);
void turn_right(void);
void turn_left(void);


#endif

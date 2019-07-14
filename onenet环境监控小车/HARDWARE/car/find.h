#ifndef __FIND_H
#define __FIND_H

#include "stm32f10x.h"

#define Find_O1 GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_2)	   //定义循迹模块的输入接口
#define Find_O2 GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_3)
//#define Find_O3 GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_3)
//#define Find_O4 GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_4)
//#define Find_O5 GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_5)

//#define SET 1		//循迹模块IO输入状态
//#define RESET 0

//#define huoyan_right() GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_12)
//#define huoyan_left() GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_13)

void Find_IO_Init(void);
void Find(void);



#endif

#include "stm32f10x_gpio.h"
#include "fire.h"
void fire_init()
{
		GPIO_InitTypeDef  GPIO_InitStructure;

		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	 

		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;				 
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
		GPIO_Init(GPIOB, &GPIO_InitStructure);					 
		GPIO_SetBits(GPIOB,GPIO_Pin_9);						 //默认不亮
}


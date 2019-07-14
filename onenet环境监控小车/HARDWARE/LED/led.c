#include "led.h"
   
//LED IO初始化
void LED_Init(void)
{
		GPIO_InitTypeDef  GPIO_InitStructure;

		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	 

		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;				 
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
		GPIO_Init(GPIOB, &GPIO_InitStructure);					 
		GPIO_SetBits(GPIOB,GPIO_Pin_10);						 //默认不亮
}

 

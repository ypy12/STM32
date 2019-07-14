#include "timer.h"
#include "led.h"
#include "delay.h"
#include"stm32f10x_it.h"
#include"stm32f10x.h"
#include"lcd.h"
#include"sys.h"
u32 count0=0;
u32 count1=0;
u32 count2=0;
u32 count3=0;
u32 deta2;
u8 i=0;
u8 j=0;	
//通用定时器中断初始化
//这里时钟选择为APB1的2倍，而APB1为36M
//arr：自动重装值。
//psc：时钟预分频数
//这里使用的是定时器2
  


void Timer2_Init(u16 arr,u16 psc)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure	;
	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
//	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 	  //上拉输入
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //50M时钟速度
	GPIO_Init(GPIOA, &GPIO_InitStructure);
    
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); //时钟使能
	TIM_DeInit(TIM2);
	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值  10Khz的计数频率  		 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设0置时钟分割:TDTS = Tck_tim	   psc
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位

	  
   	TIM_ITConfig(  //使能或者失能指定的TIM中断
		TIM2, //TIM2
		TIM_IT_Update  |  //TIM 中断源
		TIM_IT_Trigger,   //TIM 触发中断源 
		ENABLE  //使能
		);
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;  //TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器
	TIM_ETRClockMode2Config(TIM2, TIM_ExtTRGPSC_OFF, TIM_ExtTRGPolarity_NonInverted, 0); //设置为外部计数模式
	TIM_SetCounter(TIM2, 0);					 //计数器清零
	TIM_Cmd(TIM2, ENABLE);  //使能TIMx外设



//	GPIO_InitTypeDef GPIO_InitStructure;
//	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 	  //上拉输入
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //50M时钟速度
	GPIO_Init(GPIOD, &GPIO_InitStructure);
    
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //时钟使能
	RCC->APB1ENR|=1<<1;//TIM3时钟使能 
	TIM3->DIER|=1<<0;   //允许更新中断
	TIM_DeInit(TIM3);
	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	 计数到5000为500ms arr
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值  10Khz的计数频率  		 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设0置时钟分割:TDTS = Tck_tim	   psc
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位


   	TIM_ITConfig(  //使能或者失能指定的TIM中断
		TIM3, //TIM2
		TIM_IT_Update  |  //TIM 中断源
		TIM_IT_Trigger,   //TIM 触发中断源 
		ENABLE  //使能
		);
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器
	TIM_ETRClockMode2Config(TIM3, TIM_ExtTRGPSC_OFF, TIM_ExtTRGPolarity_NonInverted, 0); //设置为外部计数模式 
	TIM_SetCounter(TIM3, 0);					 //计数器清零
	TIM_Cmd(TIM3, ENABLE);  //使能TIMx外设					 
}
 u32 getspeed1()
	{
	u32 deta1;
	//count0=count1;
	//count2=count3;
	LED0=1;
	delay_ms(300);
	delay_ms(300);
	delay_ms(300);
	delay_ms(70) ;
	delay_ms(30);		   //deta t=50ms
	LED0=0;	
	count1=TIM3->CNT+count0*65536;	     //N0
	count3=TIM2->CNT+count2*65536;
	deta1=count1;
//	if(count1<count0)				//如果溢出
//	{
//	deta1=0xffff-count0+count1;
//	}
//	else
//	deta1=count1-count0;
    count0=0;
	TIM3->CNT=0X0000;
	TIM2->CNT=0X0000;
	return deta1;
	}
u32 getspeed2()
{	 
   deta2=count3;
   count2=0;   
//   if(count3<count2)
//	{
//	deta2=0xffff+count3;
//	count2=0;	       //Nx
//	}								    
//	else
//	deta2=count3;
	return deta2;
}

void TIM2_IRQHandler(void)
{ 
if(TIM2->SR&0X0001)//溢出中断
	{
		count2++;
		//LED0=!LED0;
					    				   				     	    	
	}				   
	TIM2->SR&=~(1<<0);//清除中断标志位
	
}

/*******************************************************************************
* Function Name  : TIM3_IRQHandler
* Description    : This function handles TIM3 global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void TIM3_IRQHandler(void)   
{
if(TIM3->SR&0X0001)//溢出中断
	{
		count0++;
		//LED0=!LED0;
					    				   				     	    	
	}				   
	TIM3->SR&=~(1<<0);//清除中断标志位 	    
}




//void INIT_F()
//{
//count1=0;
//count3=0;
//}













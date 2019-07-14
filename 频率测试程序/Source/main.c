/*********************/
/*  STM32工程模板    */
/*********************/                  
/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "sys.h"
#include "delay.h"
#include "usart.h" 
#include "led.h"
#include "lcd.h"
#include "freq.h"


//定时器3初始化
void TIM3_Int_Init(u16 arr,u16 psc)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //时钟使能
	
	//定时器TIM3初始化
	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //根据指定的参数初始化TIMx的时间基数单位
 
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE ); //使能指定的TIM3中断,允许更新中断

	//中断优先级NVIC设置
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //初始化NVIC寄存器
	
	TIM_Cmd(TIM3, ENABLE);  //使能TIMx					 
}
//定时器3中断服务程序
void TIM3_IRQHandler(void)   //TIM3中断
{
	if(TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)  //检查TIM3更新中断发生与否
		{
     Show_flag=1;
		 TIM_ClearITPendingBit(TIM3, TIM_IT_Update);  //清除TIMx更新中断标志       
		}
}

int main(void)
 {	

  NVIC_Configuration();  //设置NVIC中断分组2
  delay_init();          //初始化延时函数
	LCD_Init();
	TIM3_Int_Init(1999,7199);
	uart_init(19200);      //初始化串口1
	LED_Init();            //初始化LED
  LCD_Clear(CYAN);
	LCD_display_char24(0,0,BLUE,BACK_COLOR,"STM32 频率测试"); 
  LCD_display_char24(0,30,BLUE,BACK_COLOR,"频率值: ");
  TIM5_Cap_Init(0XFFFF,72-1);	//以1Mhz的频率计数 
  while(1)
  {
  	Freq_Sample();
    if(Show_flag==1)
    {
     show_frequency();
    }
  }
 }


#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

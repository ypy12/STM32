#include "freq.h"
#include "lcd.h"

u8  TIM5CH1_CAPTURE_STA=0;	//输入捕获状态		    				
u32	TIM5CH1_CAPTURE_VAL;	//输入捕获值
u16 ReadValue1,ReadValue2; 

void TIM5_Cap_Init(u16 arr,u16 psc)
{	 
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
  TIM_ICInitTypeDef  TIM5_ICInitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);	//使能TIM5时钟
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  //使能GPIOA时钟
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0;  //PA0 清除之前设置  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //PA0 输入  
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOA,GPIO_Pin_0);						 //PA0 下拉
	
	//初始化定时器5 TIM5	 
	TIM_TimeBaseStructure.TIM_Period = arr; //设定计数器自动重装值 
	TIM_TimeBaseStructure.TIM_Prescaler =psc; 	//预分频器   
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
  
	//初始化TIM5输入捕获参数
	TIM5_ICInitStructure.TIM_Channel = TIM_Channel_1; //CC1S=01 	选择输入端 IC1映射到TI1上
  TIM5_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	//上升沿捕获
  TIM5_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //映射到TI1上
  TIM5_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 //配置输入分频,不分频 
  TIM5_ICInitStructure.TIM_ICFilter = 0x00;//IC1F=0000 配置输入滤波器 不滤波
  TIM_ICInit(TIM5, &TIM5_ICInitStructure);
	
	//中断分组初始化
	NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;  //TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;  //先占优先级2级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  //从优先级0级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器 
	
	TIM_ITConfig(TIM5,TIM_IT_Update|TIM_IT_CC1,ENABLE);//允许更新中断 ,允许CC1IE捕获中断	
	
  TIM_Cmd(TIM5,ENABLE ); 	//使能定时器5
   
}


//定时器5中断服务程序	 
void TIM5_IRQHandler(void)
{ 
 	if((TIM5CH1_CAPTURE_STA&0X80)==0)//还未成功捕获	
	{	  
		if (TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET)
		{
			if(TIM5CH1_CAPTURE_STA&0X40)//已经捕获到高电平了
			{
				if((TIM5CH1_CAPTURE_STA&0X3F)==0X3f)//高电平太长了
				{
					TIM5CH1_CAPTURE_STA=0X80;//标记成功捕获了一次
					TIM5CH1_CAPTURE_VAL=0;
				}
				else 
				{
					TIM5CH1_CAPTURE_STA++;
        }
					
			}
		}
	 if (TIM_GetITStatus(TIM5, TIM_IT_CC1) != RESET)//捕获1发生捕获事件
		{	
			if(TIM5CH1_CAPTURE_STA&0X40)		//第二次捕获到上升沿
			{
				ReadValue2 = TIM_GetCapture1(TIM5);
				if ((ReadValue2 > ReadValue1)&&(TIM5CH1_CAPTURE_STA==0x40))  //无溢出
        {
          TIM5CH1_CAPTURE_VAL = (ReadValue2 - ReadValue1); 
        }
        else  //有溢出
        {
          TIM5CH1_CAPTURE_VAL =  ((0xFFFF - ReadValue1) +ReadValue2);
        }
				TIM5CH1_CAPTURE_STA|=0X80;		//标记成功捕获到一次上升沿
			}
			else  								//第一次捕获上升沿
			{
				ReadValue1=TIM_GetCapture1(TIM5);
				//TIM5CH1_CAPTURE_STA=0;			//清空
				TIM5CH1_CAPTURE_VAL=0;
				TIM5CH1_CAPTURE_STA = 0X40;		//标记捕获到了上升沿
			}
		}
 	} 
  TIM_ClearITPendingBit(TIM5, TIM_IT_CC1|TIM_IT_Update); //清除中断标志位
 
}


#define TempLen 10
float Freq_value=0;  //频率浮点值
u32 Freq[TempLen];        //频率值缓存数组
u32 Freq_Sum=0;      //
u32 Overflow_ptr=0;  //溢出计数值
u8 Freq_ptr1=0;      //滤波计数值1
u8 Freq_ptr2=0;      //滤波计数值2
u8 Show_flag=0;      //频率值显示标志位

//频率采样滤波处理
void Freq_Sample(void)
{
 u32 Dtemp=0; 
 u8 i;
 //捕获了两次高电平
 if(TIM5CH1_CAPTURE_STA&0X80)
 {
  //滤波处理
  Freq_ptr1++;
  Freq_ptr2++;
  if(Freq_ptr1>=TempLen)
    Freq_ptr1=0;    
  Dtemp = TIM5CH1_CAPTURE_STA&0X3F;
  Dtemp *= 65536;//溢出时间总和  
  if(Dtemp<=65536)  //溢出次数小于2
    Dtemp = TIM5CH1_CAPTURE_VAL;
  else 
    Dtemp = Dtemp-65536+TIM5CH1_CAPTURE_VAL;
     
  Freq[Freq_ptr1] = Dtemp;
  Dtemp=0;
  
  if(Freq_ptr2>=TempLen)
  {
    for(i=0;i<TempLen;i++)
        Dtemp += Freq[i];     
    Freq_value = 10000000.0/Dtemp;
    Freq_ptr2=TempLen;
  }
  TIM5CH1_CAPTURE_STA=0;
  Overflow_ptr=0;
 }
 else //未捕获等待清零
 {
 	Overflow_ptr++;
 	if(Overflow_ptr>720000)
 	{
 	 Freq_value=Freq_value/10;
 	 Show_flag=1;
 	 Overflow_ptr=0;
 	}		
 }
 
}

void show_frequency(void)
{
 float Ftemp;
 Ftemp=Freq_value;
 if(Ftemp>10000.0)
  LCD_display_char24(89,30,RED,BACK_COLOR,"%5.2f KHz",Ftemp/1000.0);   
 else if((Ftemp>1000.0)&&(Ftemp<10000.0))
 	LCD_display_char24(89,30,RED,BACK_COLOR,"%5.3f KHz",Ftemp/1000.0);
 else if((Ftemp<1000.0)&&(Ftemp>100.0))
	LCD_display_char24(89,30,RED,BACK_COLOR,"%5.1f Hz ",Ftemp);
 else if((Ftemp<100.0)&&(Ftemp>10.0))
  LCD_display_char24(89,30,RED,BACK_COLOR,"%5.2f Hz ",Ftemp);
 else 
  LCD_display_char24(89,30,RED,BACK_COLOR,"%5.3f Hz ",Ftemp);
 Show_flag=0;
}

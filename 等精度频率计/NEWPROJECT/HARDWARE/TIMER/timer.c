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
//ͨ�ö�ʱ���жϳ�ʼ��
//����ʱ��ѡ��ΪAPB1��2������APB1Ϊ36M
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��
//����ʹ�õ��Ƕ�ʱ��2
  


void Timer2_Init(u16 arr,u16 psc)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure	;
	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
//	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 	  //��������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //50Mʱ���ٶ�
	GPIO_Init(GPIOA, &GPIO_InitStructure);
    
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); //ʱ��ʹ��
	TIM_DeInit(TIM2);
	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  10Khz�ļ���Ƶ��  		 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //��0��ʱ�ӷָ�:TDTS = Tck_tim	   psc
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ

	  
   	TIM_ITConfig(  //ʹ�ܻ���ʧ��ָ����TIM�ж�
		TIM2, //TIM2
		TIM_IT_Update  |  //TIM �ж�Դ
		TIM_IT_Trigger,   //TIM �����ж�Դ 
		ENABLE  //ʹ��
		);
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;  //TIM3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���
	TIM_ETRClockMode2Config(TIM2, TIM_ExtTRGPSC_OFF, TIM_ExtTRGPolarity_NonInverted, 0); //����Ϊ�ⲿ����ģʽ
	TIM_SetCounter(TIM2, 0);					 //����������
	TIM_Cmd(TIM2, ENABLE);  //ʹ��TIMx����



//	GPIO_InitTypeDef GPIO_InitStructure;
//	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 	  //��������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //50Mʱ���ٶ�
	GPIO_Init(GPIOD, &GPIO_InitStructure);
    
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //ʱ��ʹ��
	RCC->APB1ENR|=1<<1;//TIM3ʱ��ʹ�� 
	TIM3->DIER|=1<<0;   //��������ж�
	TIM_DeInit(TIM3);
	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	 ������5000Ϊ500ms arr
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  10Khz�ļ���Ƶ��  		 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //��0��ʱ�ӷָ�:TDTS = Tck_tim	   psc
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ


   	TIM_ITConfig(  //ʹ�ܻ���ʧ��ָ����TIM�ж�
		TIM3, //TIM2
		TIM_IT_Update  |  //TIM �ж�Դ
		TIM_IT_Trigger,   //TIM �����ж�Դ 
		ENABLE  //ʹ��
		);
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���
	TIM_ETRClockMode2Config(TIM3, TIM_ExtTRGPSC_OFF, TIM_ExtTRGPolarity_NonInverted, 0); //����Ϊ�ⲿ����ģʽ 
	TIM_SetCounter(TIM3, 0);					 //����������
	TIM_Cmd(TIM3, ENABLE);  //ʹ��TIMx����					 
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
//	if(count1<count0)				//������
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
if(TIM2->SR&0X0001)//����ж�
	{
		count2++;
		//LED0=!LED0;
					    				   				     	    	
	}				   
	TIM2->SR&=~(1<<0);//����жϱ�־λ
	
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
if(TIM3->SR&0X0001)//����ж�
	{
		count0++;
		//LED0=!LED0;
					    				   				     	    	
	}				   
	TIM3->SR&=~(1<<0);//����жϱ�־λ 	    
}




//void INIT_F()
//{
//count1=0;
//count3=0;
//}













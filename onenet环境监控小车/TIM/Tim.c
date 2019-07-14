#include "Tim.h"
#include "sys.h"

/*		��ʱ��1��Ϊ��ʱ����ʱ�����ú���   */
void Tim1_Init(int arr,int psc)
{ 
   TIM_TimeBaseInitTypeDef TIM_Structure;               //���嶨ʱ���ṹ�����
	 NVIC_InitTypeDef NVIC_TIM;                           //�����ж�Ƕ�׽ṹ�����
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);  //�򿪶�ʱ��ʱ��
	 
	 TIM_Structure.TIM_Period = (arr-1) ;         //�����Զ���װ�ؼĴ�������ֵ  ���ʱ��TimeOut= (arr)*(psc)/Tic    ��λΪus
   TIM_Structure.TIM_Prescaler = (psc-1);       //����Ԥ��Ƶֵ     
   TIM_Structure.TIM_CounterMode = TIM_CounterMode_Up ;     //����ģʽ ��������
	 TIM_Structure.TIM_ClockDivision = TIM_CKD_DIV1;     //ʱ�ӷ�Ƶ      Tic=72M/��TIM_ClockDivision+1��
	 TIM_Structure.TIM_RepetitionCounter = 0; //�ظ������Ĵ���
	
	 TIM_TimeBaseInit(TIM1,&TIM_Structure);   //��ʼ����ʱ��1
	
	 NVIC_TIM.NVIC_IRQChannel = TIM1_UP_IRQn;  //��ʱ��1�����ϼ���ͨ��
	 NVIC_TIM.NVIC_IRQChannelCmd = ENABLE ;    //ʹ��
	 NVIC_TIM.NVIC_IRQChannelPreemptionPriority = 0 ;    //��ռ���ȼ�
	 NVIC_TIM.NVIC_IRQChannelSubPriority = 0;            //��Ӧ���ȼ� 
	
	 NVIC_Init(&NVIC_TIM);                     //��ʼ���ṹ��
	 
	 TIM_ClearFlag(TIM1,TIM_FLAG_Update);      //������б�־λ  ��֤����״̬��ʼ�� 
	 
	 TIM_ITConfig(TIM1,TIM_IT_Update,ENABLE);  //�򿪼�ʱ��
	
	 TIM_Cmd(TIM1,ENABLE);      	  	 	       //��TIM1
	
	 
}	

void TIM1_UP_IRQHandler (void)                
{
   if(TIM_GetITStatus(TIM1,TIM_IT_Update) != RESET)   //����жϱ�־����1 ֤�����ж�
	 {
		 
		  TIM_ClearITPendingBit(TIM1,TIM_IT_Update);    // ��ձ�־λ��Ϊ��һ�ν����ж���׼��
	    //��������	  
		  Heart_Pack++;            //��ʱ�Լ�һ��
	 }
  
}




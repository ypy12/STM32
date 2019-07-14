#include "freq.h"
#include "lcd.h"

u8  TIM5CH1_CAPTURE_STA=0;	//���벶��״̬		    				
u32	TIM5CH1_CAPTURE_VAL;	//���벶��ֵ
u16 ReadValue1,ReadValue2; 

void TIM5_Cap_Init(u16 arr,u16 psc)
{	 
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
  TIM_ICInitTypeDef  TIM5_ICInitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);	//ʹ��TIM5ʱ��
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  //ʹ��GPIOAʱ��
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0;  //PA0 ���֮ǰ����  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //PA0 ����  
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOA,GPIO_Pin_0);						 //PA0 ����
	
	//��ʼ����ʱ��5 TIM5	 
	TIM_TimeBaseStructure.TIM_Period = arr; //�趨�������Զ���װֵ 
	TIM_TimeBaseStructure.TIM_Prescaler =psc; 	//Ԥ��Ƶ��   
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
  
	//��ʼ��TIM5���벶�����
	TIM5_ICInitStructure.TIM_Channel = TIM_Channel_1; //CC1S=01 	ѡ������� IC1ӳ�䵽TI1��
  TIM5_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	//�����ز���
  TIM5_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //ӳ�䵽TI1��
  TIM5_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 //���������Ƶ,����Ƶ 
  TIM5_ICInitStructure.TIM_ICFilter = 0x00;//IC1F=0000 ���������˲��� ���˲�
  TIM_ICInit(TIM5, &TIM5_ICInitStructure);
	
	//�жϷ����ʼ��
	NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;  //TIM3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;  //��ռ���ȼ�2��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  //�����ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ��� 
	
	TIM_ITConfig(TIM5,TIM_IT_Update|TIM_IT_CC1,ENABLE);//��������ж� ,����CC1IE�����ж�	
	
  TIM_Cmd(TIM5,ENABLE ); 	//ʹ�ܶ�ʱ��5
   
}


//��ʱ��5�жϷ������	 
void TIM5_IRQHandler(void)
{ 
 	if((TIM5CH1_CAPTURE_STA&0X80)==0)//��δ�ɹ�����	
	{	  
		if (TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET)
		{
			if(TIM5CH1_CAPTURE_STA&0X40)//�Ѿ����񵽸ߵ�ƽ��
			{
				if((TIM5CH1_CAPTURE_STA&0X3F)==0X3f)//�ߵ�ƽ̫����
				{
					TIM5CH1_CAPTURE_STA=0X80;//��ǳɹ�������һ��
					TIM5CH1_CAPTURE_VAL=0;
				}
				else 
				{
					TIM5CH1_CAPTURE_STA++;
        }
					
			}
		}
	 if (TIM_GetITStatus(TIM5, TIM_IT_CC1) != RESET)//����1���������¼�
		{	
			if(TIM5CH1_CAPTURE_STA&0X40)		//�ڶ��β���������
			{
				ReadValue2 = TIM_GetCapture1(TIM5);
				if ((ReadValue2 > ReadValue1)&&(TIM5CH1_CAPTURE_STA==0x40))  //�����
        {
          TIM5CH1_CAPTURE_VAL = (ReadValue2 - ReadValue1); 
        }
        else  //�����
        {
          TIM5CH1_CAPTURE_VAL =  ((0xFFFF - ReadValue1) +ReadValue2);
        }
				TIM5CH1_CAPTURE_STA|=0X80;		//��ǳɹ�����һ��������
			}
			else  								//��һ�β���������
			{
				ReadValue1=TIM_GetCapture1(TIM5);
				//TIM5CH1_CAPTURE_STA=0;			//���
				TIM5CH1_CAPTURE_VAL=0;
				TIM5CH1_CAPTURE_STA = 0X40;		//��ǲ�����������
			}
		}
 	} 
  TIM_ClearITPendingBit(TIM5, TIM_IT_CC1|TIM_IT_Update); //����жϱ�־λ
 
}


#define TempLen 10
float Freq_value=0;  //Ƶ�ʸ���ֵ
u32 Freq[TempLen];        //Ƶ��ֵ��������
u32 Freq_Sum=0;      //
u32 Overflow_ptr=0;  //�������ֵ
u8 Freq_ptr1=0;      //�˲�����ֵ1
u8 Freq_ptr2=0;      //�˲�����ֵ2
u8 Show_flag=0;      //Ƶ��ֵ��ʾ��־λ

//Ƶ�ʲ����˲�����
void Freq_Sample(void)
{
 u32 Dtemp=0; 
 u8 i;
 //���������θߵ�ƽ
 if(TIM5CH1_CAPTURE_STA&0X80)
 {
  //�˲�����
  Freq_ptr1++;
  Freq_ptr2++;
  if(Freq_ptr1>=TempLen)
    Freq_ptr1=0;    
  Dtemp = TIM5CH1_CAPTURE_STA&0X3F;
  Dtemp *= 65536;//���ʱ���ܺ�  
  if(Dtemp<=65536)  //�������С��2
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
 else //δ����ȴ�����
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

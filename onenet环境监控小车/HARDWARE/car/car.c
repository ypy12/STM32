/*****************************
��������car.c
������  go()ǰ��	
		back()����
		turn_left()��ת
		turn_right()��ת
		Ҳ��ʵ��PWM���Թ���
Ӳ�����ӣ�Pb12-in1,Pb13-in2,Pb14-in3,Pb15-in4
		  PB8-PWMA,PB8-PWMB
��汾��3.5
��д���ϿƼ�
*****************************/
#include "car.h"

/*****************************
��������Car_Init
������ ��ʼ�������IO�˿�
���룺
�����
���ã��ⲿ
******************************/
void Car_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	//����GPIOCʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);//�˳���ʱҪ����JATG����ΪSWDģʽ��ע�⣡��������������������������������

	//��ʼ��GPIOC�˿�
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//����Ϊ�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);

	   //�˴��ǽ�PWM�����������Ϊ�ߵ�ƽ
//		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
//		 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 ;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//����Ϊ�������
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_Init(GPIOB,&GPIO_InitStructure);
	GPIO_ResetBits(GPIOB,GPIO_Pin_4) ;
	GPIO_ResetBits(GPIOB,GPIO_Pin_5) ;
	GPIO_ResetBits(GPIOB,GPIO_Pin_6) ;
	GPIO_ResetBits(GPIOB,GPIO_Pin_7) ;
	GPIO_ResetBits(GPIOB,GPIO_Pin_3) ;
	go();

}
/*******************************
��������go()
������ǰ��
���ã��ⲿ

*******************************/
void go(void)
{
	in1_set;   //����
	in2_clr;
	in3_set;  //����
	in4_clr;
}

/*******************************
��������back()
����������
���ã��ⲿ

*******************************/
void back(void)
{
	in1_clr;	 //����
	in2_set;
	in3_clr;	  //����
	in4_set;
}

/*******************************
��������back()
����������
���ã��ⲿ

*******************************/
void car_stop(void)
{
	in1_clr;	 //����
	in2_clr;
	in3_clr;	  //����
	in4_clr;
}

/*******************************
��������turn_right()
��������ת
���ã��ⲿ

*******************************/
void turn_right(void)
{
	in1_set;
	in2_clr;	    //����
	in3_clr;
	in4_set;//����
}

/*******************************
��������turn_left()
��������ת
���ã��ⲿ

*******************************/
void turn_left(void)
{
	in1_clr;
	in2_set;	    //����
	in3_set;
	in4_clr;//����
}




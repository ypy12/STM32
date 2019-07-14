/*
�ļ���:find.c
Ӳ�����ӣ�
			O1 ---- PB2
		   	O2 ---- PB3
			
������ѭ�����򡣴˳���ֻ����ϿƼ���2·ѭ��ģ����Ч��
		�����ô˳���ʱҪ����JATG����ΪSWDģʽ��ע�⣡��������������������������������
��д���ϿƼ�
�Ա����꣺http://shop60670850.taobao.com/
*/
#include "find.h"
//#include "PWM.h"
#include "bsp_pwm_output.h"
#include "car.h"
#include "bsp_SysTick.h"

//ѭ��IO��ʼ��
//Ҫ��ʼ��Ϊ����ģʽ
void Find_IO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	//����GPIOʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);//�˳���ʱҪ����JATG����ΪSWDģʽ��ע�⣡��������������������������������

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;//ѡ��IO�˿�
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//����Ϊ��������
	GPIO_Init(GPIOB, &GPIO_InitStructure);//����GPIO_InitStructure��ָ���Ĳ�����ʼ������GPIOD�Ĵ���



}


//ѭ������ѭ��ģ�� ��������͵�ƽ  ���߸ߵ�ƽ
//С���������O1----���Ҷ���O2
//ѭ��·�棺��ɫ·���ɫ�����ߣ���Ѱ���ߡ�
//���� ���������0���������1
//PWM PB8 ---PWM_ch3����  PB9 ---PWM_ch4 ����
void Find(void)
{
	if((Find_O1 == 1) && (Find_O2 == 1))// ǰ��
	{
			PWM_Init(1000,0,700,700);//PWM  Ƶ��= 72M/1000 =72KHz  ����ռ�ձ� = 500/1000������ռ�ձ� =  500/1000
			go();
	}

	//O3 Ѱ������  ǰ��
	if((Find_O1 == 0) && (Find_O2 == 0) )
	{
			//PWM_Init(1000,0,800,800);//PWM  Ƶ��= 72M/1000 =72KHz  ����ռ�ձ� = 500/1000������ռ�ձ� =  500/1000
			stop();
	}
	//O1Ѱ������ ˵��С��ƫ������ʻ�� Ӧ����ת������ǰ������	�������ּ������ּ���
	if((Find_O1 == 0) && (Find_O2 == 1))
	{
			PWM_Init(1000,0,0,700);//PWM  Ƶ��= 72M/1000 =72KHz  ����ռ�ձ� = 500/1000������ռ�ձ� =  500/1000
			go();
	}

	//O2Ѱ������ ˵��С��ƫ������ʻ�� Ӧ����ת������ǰ������	�������ּ������ּ���
	if((Find_O1 == 1) && (Find_O2 == 0))
	{
			PWM_Init(1000,0,700,0);//PWM  Ƶ��= 72M/1000 =72KHz  ����ռ�ձ� = 500/1000������ռ�ձ� =  500/1000
			go();
	}
	

	
	
}













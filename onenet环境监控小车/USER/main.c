#include "stm32f10x.h"
#include "esp8266.h"
#include "delay.h"
#include "usart.h"
#include "stdio.h"
#include "sys.h"
#include "led.h"
#include "onenet.h"
#include "string.h" 
#include "Tim.h"
#include "dht11.h"
#include "car.h"
#include "OLED_IIC.h"
#include "fire.h"

uint8_t d;
/*   ȫ�ֱ�������  */
unsigned int Heart_Pack=0;  //���ڶ�ʱ��TIM1�ԼӼ��������������趨�Լ���ֵʱ����EDP�������ı�־λ
u8  AT_Mode=0;     //��ATָ���ģʽ ���ڱ�ʾ����������Wifiģ���ģʽ  �Դ��ڽ������ݽ��в�ͬ�Ĵ���
u8  Contral_flag=0;  //���ڴ�������ָ��  �жϽ���ʲô����

void System_Init()
{
/*  �������ܳ�ʼ��   */
		delay_init();					//��ʱ��ʼ��
		uart_init(115200);    //�򿪴���2����Wifiģ��ͨ��
		LED_Init();           //LED�Ƴ�ʼ��
	
/*  ����wifiģ��     */
		ESP8266_GPIO_Init();  //��ʼ��ESP8266��CH_DP���ţ����߼���
		ESP8266Mode_inti();   //��ʼ��ESP8266��ATָ��

/*  �򿪶�ʱ��       */
		Tim1_Init(5000,72);   //��ʱ����������Ϊ5MS
}

void Connect_OneNet(void)
{
     while(!OneNet_Connect)     //�ж��Ƿ����ƽ̨�ɹ�
	   {	
	    	while(OneNet_DevLink())			//����OneNET
	   	  delay_ms(1000);       
	   }
}

extern unsigned char hanzi1[];
int main(void)
{	 
	u8 t = 0;
	u8 temperature; //�����¶� 	    
	u8 humidity;//����ʪ��
	u8 fire;//�������
	
	delay_init();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ� 
	
	DHT11_Init();
	OLED_Init();		    //��ʼ��OLED
  initial_olcd();		  //��ʼ��
	clear_screen();			//����
	Car_Init();
	delay_init();
	//LED_Init();
	fire_init();
	USART3_Init(38400);
	  fire=GPIO_ReadOutputDataBit(GPIOB,GPIO_Pin_9);
		EDP_PACKET_STRUCTURE edpPacket = {NULL, 0, 0, 0};   //����һ���ṹ�����   ��������װ��������
		
    System_Init();       //��ʼ����ģ��
		
		Connect_OneNet();    //����OneNetƽ̨

		while(1)	             //whileѭ��ִ�и��ֹ��� 
		{  
      /*  ��ʱ����������  */			
			if(Heart_Pack==5000)              //ÿ��25�뷢һ��������   OneNetƽ̨Ĭ���ķ��������ݽ����ͻ�Ͽ����豸������ �����������Ա�֤���ӡ�
			{
				EDP_PacketPing(&edpPacket);     //װ�ز�����������
				Heart_Pack=0;
			
			}    
			 
		/*��strstr�������жϲ���ָ���Ƿ�ƥ�� �Ըú������˽���������аٶȰ�*/	 
			 if(strstr((const char*)Message_Buf,"OPEN"))    //�жϵ�����ָ��ΪOPEN 
			 {
					LED_ON;   //��LED
					OneNet_SendData(1,temperature,humidity);  //��ƽ̨������1
					delay_ms(20); 
					memset(Message_Buf, 0, sizeof(Message_Buf));    //ִ����ָ�� ���ָ������ռ� ��ֹ����ִ�и�ָ��
			 }
			 if(strstr((const char*)Message_Buf,"CLOSE"))   //�жϵ�����ָ��ΪCLOSE 
			 {
					LED_OFF; //�ر�LED
					OneNet_SendData(0,temperature,humidity);  //��ƽ̨������0
					delay_ms(20);   //�ӳ�
					memset(Message_Buf, 0, sizeof(Message_Buf));    //ִ����ָ�� ���ָ������ռ� ��ֹ����ִ�и�ָ��
			 }
			 
			/*****************OLED��ʾ��ʪ��*******************/
		disp_string_8x16_16x16(1,1,"�¶�:");
		disp_string_8x16_16x16(3,1,"ʪ��:");
		disp_string_8x16_16x16(1,80,"C");
		disp_string_8x16_16x16(3,80,"%");
		disp_string_8x16_16x16(5,1,"�Һò˰�");
		if(t%10==0)			//ÿ100ms��ȡһ��
		{									  
			DHT11_Read_Data(&temperature,&humidity);	//��ȡ��ʪ��ֵ	

			display_number_16x8(1,46,temperature);
			display_number_16x8(3,46,humidity);
		}				   
	 	delay_ms(10);
		t++;
		if(t==20)
		{
			t=0;
		}
		}
			
 }

void USART3_IRQHandler(void)
{
	//extern uint8_t d;
	/* USART in Receiver mode */
	if (USART_GetITStatus(USART3, USART_IT_RXNE) == SET)  
	{
		d = USART_ReceiveData(USART3);
		switch(d)
			{
				case '0': car_stop(); break;
				case '1': go(); break;
				case '2': back(); break;
				case '3': turn_left(); break;
				case '4': turn_right(); break;
			}
		USART_SendData(USART3, d);//����
		/* Loop until the end of transmission */
		while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET);
	}
}
		

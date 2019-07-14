/**
	************************************************************
	************************************************************
	************************************************************
	*	�ļ����� 	esp8266.c
	*
	*	ԭ���ߣ� 		�ż���
	*
	*	���ڣ� 		2017-05-08
	*
	*	�汾�� 		V1.0
	*
	*	˵���� 		ESP8266�ļ�����
	*
	*	�汾�޸ļ�¼��	��
	
	* �޸��ߣ� �㶫�����ѧ���ܿ����Ŷ� Allen
	
	* ����  �� 2018.05.18
	************************************************************
	************************************************************
	************************************************************
**/

//��Ƭ��ͷ�ļ�
#include "stm32f10x.h"

//�����豸����
#include "esp8266.h"

//Ӳ������
#include "delay.h"
#include "usart.h"
#include "Led.h"
//C��
#include <string.h>
#include <stdio.h>

/** ���ڷ���һ�����ݵĺ���
  *
  *
  *
  */ 
void USART_Write(unsigned char *cmd, int len)
{  
	  int i;
	   USART_ClearFlag(USART2,USART_FLAG_TC);    //����֮ǰ��շ��ͱ�־  û����һ�� �����׶��� ��һ���������׶�ʧ
	   for(i=0;i<len;i++)
	   {
		   USART_SendData(USART2,*cmd);   //���͵�ǰ����
			 while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!= SET);  //�������
//			 delay_ms(30);
			 cmd++;      //ָ���Լ� ׼��������һ������
		   
		 }
 
}

/**
  * @brief  ��ʼ��ESP8266�õ���GPIO����
  * @param  ��
  * @retval ��
  */
void ESP8266_GPIO_Init(void)
{
		/*����һ��GPIO_InitTypeDef���͵Ľṹ��*/
		GPIO_InitTypeDef GPIO_InitStructure;


		/* ���� CH_PD ����*/
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);

		GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;               //�����������ģʽ
		GPIO_InitStructure.GPIO_Pin=GPIO_Pin_13;                      //��ʼ��CH-DP���� ����
		GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;              //����ʱ��
		GPIO_Init(GPIOG,&GPIO_InitStructure);   
		GPIO_SetBits(GPIOG,GPIO_Pin_13); 

		GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;               //�����������ģʽ
		GPIO_InitStructure.GPIO_Pin=GPIO_Pin_14;                      //��ʼ��RST���� ����
		GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;              //����ʱ��
		GPIO_Init(GPIOG,&GPIO_InitStructure);   
		GPIO_SetBits(GPIOG,GPIO_Pin_14); 
  
}

void ESP8266Mode_inti(void)
{
	  int Delay = 500;
	
	  AT_Mode = 1;  //���뷢ATָ��ģʽ
	  
/****wifiģ����OneNetƽ̨���ݶԽ����� */
    SendCmd(AT, "OK", Delay);		//ģ����Ч�Լ��
    SendCmd(CWMODE, "OK", Delay);	//ģ�鹤��ģʽ
    SendCmd(wifi_RST, "OK", Delay);		//ģ������
	
	  delay_ms(1000);								//�ȴ�ģ�鸴λ���
	
    SendCmd(CWJAP, "OK", 1700);		//������Ҫ���ӵ�WIFI�ȵ�SSID������  CIFSR
    SendCmd(CIPSTART, "OK", Delay);	//TCP����
    SendCmd(CIPMODE1, "OK", Delay);	//����͸��ģʽ
	  SendCmd(CIPSEND, ">", Delay);	//����͸��ģʽ
		
		AT_Mode = 0;   //ATָ������ �˳���ģʽ
}


	
/** 
	* �������ܣ� ����Cmd����ĺ���
  * CMD�� 		��Ҫ���͵�ATָ��
  * result �� ���ͳɹ�ʱ���ص���ֵ��result��������Ա�
  * timeOut ���ӳ�ʱ��
  *	
*/	
	
void SendCmd(char* cmd, char* result, int timeOut)
{
    while(1)
    {
			  memset(Rx_Buff, 0, sizeof(Rx_Buff)); //��������֮ǰ��������������飬�����ڴ����н��ա�
			  Rx_count=0;
        USART_Write((unsigned char *)cmd,strlen((const char *)cmd));   //�ô��ڰ�cmd����д��ESP8266
        delay_ms(timeOut);                                          //�ӳٵȴ�
			  LED=~LED;        //��������
        if(ok_flag==1)	//�Ƚ�����ָ������������Ƿ�һ�����ж��Ƿ���Ԥ�ڵĽ��  ��Ԥ�ڽ����ͬ����ʾָ�����óɹ�������
        {  
					  ok_flag=0;   //��ձ�־
            break;
        }
        else
        {
            delay_ms(100);
					
        }
    }
}


/**   
 * �������ܣ�ESP8266д���ݵĺ���

 *   

*/
void ESP8266_SendData(unsigned char *data, unsigned short len)
{
	 
  	USART_Write(data,len);   //��������

}



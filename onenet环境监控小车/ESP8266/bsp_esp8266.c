#include "bsp_esp8266.h"
#include "common.h"
#include <stdio.h>  
#include <string.h>  
#include <stdbool.h>
#include "usart.h"
//#include "bsp_SysTick.h"
#include "sys.h"
#include "delay.h"

//EdpPacket* send_pkg;
char send_buf[1000];
//static void                   ESP8266_GPIO_Config                 ( void );
//static void                   ESP8266_USART_Config                ( void );
//static void                   ESP8266_USART_NVIC_Configuration    ( void );


#define LED PBout(9)
struct  STRUCT_USARTx_Fram strEsp8266_Fram_Record = { 0 };




/** ESP8266���ӵ���wifi������
  *
  *
  *
   */
void ESP8266Mode_inti(void)
{
	  int i=0;
	  AT_Mode = 1;  //���뷢ATָ��ģʽ
	
/****wifiģ����OneNetƽ̨���ݶԽ����� */
    SendCmd(AT, "OK", 10000);		//ģ����Ч�Լ��
    SendCmd(CWMODE, "OK", 10000);	//ģ�鹤��ģʽ
    SendCmd(wifi_RST, "OK", 10000);		//ģ������
    SendCmd(CIFSR, "OK", 10000);		//��ѯ������Ϣ
    SendCmd(CWJAP, "OK", 10000);		//������Ҫ���ӵ�WIFI�ȵ�SSID������
    SendCmd(CIPSTART, "OK", 10000);	//TCP����
    SendCmd(CIPMODE1, "OK", 10000);	//����͸��ģʽ
//	  SendCmd(CIPSEND, ">", 10000);	//����͸��ģʽ
     
  
/****wifiģ������Է��������ݶԽ�����   */
//	  SendCmd(AT, "OK", 10000);		//ģ����Ч�Լ��
//	  SendCmd(wifi_RST, "OK", 10000);		//ģ������
//    SendCmd(CWMODE, "OK", 10000);	//ģ�鹤��ģʽ ����STA��APģʽ
//	  SendCmd(CWJAP, "OK", 10000);		//������Ҫ���ӵ�WIFI�ȵ�SSID������ 
//    SendCmd(Check_CWJAP, "OK", 10000);  //��ѯ�Ƿ�ɹ�����·���� 
//	  SendCmd(CIPMUX, "OK", 10000);  //��������ģʽ
//	  SendCmd(CIPSERVER, "OK", 10000);//����������ģʽ
//	  SendCmd(CIFSR, "OK", 10000);   //��ѯ·��������ķ�������ַ
//	  SendCmd(CIPMUX, "OK", 10000);  //�ٴο�������ģʽ
//		SendCmd(CIPSTO, "OK", 10000);  //���÷�������ʱʱ��
		
		
  /***������Ϊ��������wifiģ������ͨ��  
	  SendCmd(AT, "OK", 10000);		//ģ����Ч�Լ��
	  SendCmd(wifi_RST, "OK", 10000);		//ģ������
    SendCmd(CWMODE, "OK", 10000);	//ģ�鹤��ģʽ ����STA��APģʽ
		SendCmd(CWJAP, "OK", 10000);		//������Ҫ���ӵ�WIFI�ȵ�SSID������ 
		SendCmd(CIFSR, "OK", 10000);   //��ѯ·��������ķ�������ַ
		SendCmd(CIPSTART, "OK", 10000);   //������Է�����  
		SendCmd(CIPMODE1, "OK", 10000);   //����͸��ģʽ
		SendCmd(CIPSEND, "OK", 10000);   //����͸��ģʽ
		*/
		
		 AT_Mode = 0;   //ATָ������ �˳���ģʽ
	}
/** ����Cmd����ĺ���
  *
  *
  *
  */ 
void SendCmd(char* cmd, char* result, int timeOut)
{
    while(1)
    {
//        memset(Rx_Buff, 0, sizeof(Rx_Buff));   //����������������0
        USART_Write((unsigned char *)cmd,strlen((const char *)cmd));   //�ô��ڰ�cmd����д��ESP8266
        delay_ms(timeOut);                                          //�ӳٵȴ�
			  LED=~LED;
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
/** ���ڷ��ͺ���
  *
  *
  *
  */ 
void USART_Write(unsigned char *cmd, int len)
{  
	  int i;
	   USART_ClearFlag(USART1,USART_FLAG_TC); 
	   for(i=0;i<len;i++)
	   {
		   USART_SendData(USART1,*cmd);
			 while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!= SET);
			 cmd++;
		   
		 }
     
}


/**
  * @brief   ��HTTP POST����
  * @param   pkt   ���Ļ���ָ��
  * @param   key   API_KEY������Main.c�ļ��У���Ҫ�����Լ����豸�޸�
  *	@param 	 devid �豸ID��������main.c�ļ��У���Ҫ�����Լ����豸�޸�
  *	@param 	 dsid  ������ID
  *	@param 	 val   �ַ�����ʽ�����ݵ��ֵ
  * @retval  �������ĳ���
  *///���HTTPЭ�����ݵĸ�ʽ
uint32_t HTTP_PostPkt(char *pkt, char *key, char *devid, char *dsid, char *val)
{
    char dataBuf[100] = {0};
    char lenBuf[10] = {0};
    *pkt = 0;

    sprintf(dataBuf, ",;%s,%s", dsid, val);     //���÷ָ��ַ�����ʽ:type = 5
    sprintf(lenBuf, "%d", strlen(dataBuf));

    strcat(pkt, "POST /devices/");
    strcat(pkt, devid);
    strcat(pkt, "/datapoints?type=5 HTTP/1.1\r\n");

    strcat(pkt, "api-key:");
    strcat(pkt, key);
    strcat(pkt, "\r\n");

    strcat(pkt, "Host:api.heclouds.com\r\n");

    strcat(pkt, "Content-Length:");
    strcat(pkt, lenBuf);
    strcat(pkt, "\r\n\r\n");

    strcat(pkt, dataBuf);

    return strlen(pkt);
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
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_14;                      //��ʼ��CH-DP���� ����
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;              //����ʱ��
	GPIO_Init(GPIOA,&GPIO_InitStructure);   
	GPIO_SetBits(GPIOA,GPIO_Pin_14); 
  
}
/**
  * @brief  ESP8266��ʼ������
  * @param  ��
  * @retval ��
  */

void ESP8266_Init ( void )
{
	ESP8266_GPIO_Init(); 
	
	//SP8266_USART_Config(); 
	
	
	macESP8266_RST_HIGH_LEVEL();

	macESP8266_CH_ENABLE();
	
	
}

/**
  * @brief  ��ʼ��ESP8266�õ��� USART
  * @param  ��
  * @retval ��
  */
void ESP8266_USART_Config( void )
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	
	
	/* config USART clock */
	macESP8266_USART_APBxClock_FUN ( macESP8266_USART_CLK, ENABLE );
	macESP8266_USART_GPIO_APBxClock_FUN ( macESP8266_USART_GPIO_CLK, ENABLE );
	
	/* USART GPIO config */
	/* Configure USART Tx as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin =  macESP8266_USART_TX_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(macESP8266_USART_TX_PORT, &GPIO_InitStructure);  
  
	/* Configure USART Rx as input floating */
	GPIO_InitStructure.GPIO_Pin = macESP8266_USART_RX_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(macESP8266_USART_RX_PORT, &GPIO_InitStructure);
	
	/* USART1 mode config */
	USART_InitStructure.USART_BaudRate = macESP8266_USART_BAUD_RATE;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(macESP8266_USARTx, &USART_InitStructure);
	
	
	/* �ж����� */
	USART_ITConfig ( macESP8266_USARTx, USART_IT_RXNE, ENABLE ); //ʹ�ܴ��ڽ����ж� 
	USART_ITConfig ( macESP8266_USARTx, USART_IT_IDLE, ENABLE ); //ʹ�ܴ������߿����ж� 	

	ESP8266_USART_NVIC_Configuration ();
	
	
	USART_Cmd(macESP8266_USARTx, ENABLE);
	
	
}


/**
  * @brief  ���� ESP8266 USART �� NVIC �ж�
  * @param  ��
  * @retval ��
  */
void ESP8266_USART_NVIC_Configuration ( void )
{
	NVIC_InitTypeDef NVIC_InitStructure; 
	
	
	/* Configure the NVIC Preemption Priority Bits */  
	NVIC_PriorityGroupConfig ( macNVIC_PriorityGroup_x );

	/* Enable the USART2 Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = macESP8266_USART_IRQ;	 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

}


/*
 * ��������ESP8266_Rst
 * ����  ������WF-ESP8266ģ��
 * ����  ����
 * ����  : ��
 * ����  ���� ESP8266_AT_Test ����
 */
void ESP8266_Rst ( void )
{
	#if 0
	 ESP8266_Cmd ( "AT+RST", "OK", "ready", 2500 );   	
	
	#else
	 macESP8266_RST_LOW_LEVEL();
	 delay_ms ( 500 ); 
	 macESP8266_RST_HIGH_LEVEL();
	 
	#endif

}


/*
 * ��������ESP8266_Cmd
 * ����  ����WF-ESP8266ģ�鷢��ATָ��
 * ����  ��cmd�������͵�ָ��
 *         reply1��reply2���ڴ�����Ӧ��ΪNULL������Ӧ������Ϊ���߼���ϵ
 *         waittime���ȴ���Ӧ��ʱ��
 * ����  : 1��ָ��ͳɹ�
 *         0��ָ���ʧ��
 * ����  �����ⲿ����
 */
bool ESP8266_Cmd ( char * cmd, char * reply1, char * reply2, u32 waittime )
{    
	strEsp8266_Fram_Record .InfBit .FramLength = 0;               //���¿�ʼ�����µ����ݰ�

	macESP8266_Usart ( "%s\r\n", cmd );

	if ( ( reply1 == 0 ) && ( reply2 == 0 ) )                      //����Ҫ��������
		return true;
	
	delay_ms ( waittime );                 //��ʱ
	
	strEsp8266_Fram_Record .Data_RX_BUF [ strEsp8266_Fram_Record .InfBit .FramLength ]  = '\0';

	macPC_Usart ( "%s", strEsp8266_Fram_Record .Data_RX_BUF );
  
	if ( ( reply1 != 0 ) && ( reply2 != 0 ) )
		return ( ( bool ) strstr ( strEsp8266_Fram_Record .Data_RX_BUF, reply1 ) || 
						 ( bool ) strstr ( strEsp8266_Fram_Record .Data_RX_BUF, reply2 ) ); 
 	
	else if ( reply1 != 0 )
		return ( ( bool ) strstr ( strEsp8266_Fram_Record .Data_RX_BUF, reply1 ) );
	
	else
		return ( ( bool ) strstr ( strEsp8266_Fram_Record .Data_RX_BUF, reply2 ) );
	
}



/*
 * ��������ESP8266_ReceiveString
 * ����  ��WF-ESP8266ģ������ַ���
 * ����  ��enumEnUnvarnishTx�������Ƿ���ʹ����͸��ģʽ
 * ����  : ���յ����ַ����׵�ַ
 * ����  �����ⲿ����
 */
char * ESP8266_ReceiveString ( FunctionalState enumEnUnvarnishTx )
{
	char * pRecStr = 0;
	
	
	strEsp8266_Fram_Record .InfBit .FramLength = 0;
	strEsp8266_Fram_Record .InfBit .FramFinishFlag = 0;
	
	while ( ! strEsp8266_Fram_Record .InfBit .FramFinishFlag );
	strEsp8266_Fram_Record .Data_RX_BUF [ strEsp8266_Fram_Record .InfBit .FramLength ] = '\0';
	
	if ( enumEnUnvarnishTx )
		pRecStr = strEsp8266_Fram_Record .Data_RX_BUF;
	
	else 
	{
		if ( strstr ( strEsp8266_Fram_Record .Data_RX_BUF, "+IPD" ) )
			pRecStr = strEsp8266_Fram_Record .Data_RX_BUF;

	}

	return pRecStr;
	
}

/**
  * @brief  ��ƽ̨�����豸����
**/
//void ESP8266_DevLink(const char* devid, const char* auth_key, int timeOut)
//{

//	  AT_Mode = 1;
//		SendCmd(CIPSEND, "OK", 10000);  //��ESP8266��������͸��ָ��
//	  AT_Mode = 0;
//	
//		send_pkg = PacketConnect1(devid,auth_key);
//		delay_ms(2000);
//	  
//		USART_Write((unsigned char *)send_pkg,strlen((const char *)send_pkg));   //�����豸������������
//		
//		
//		delay_ms(5000);
//		DeleteBuffer(&send_pkg);
//		delay_ms(2000);
//	
//	  AT_Mode = 1;
//		USART_Write("+++",3);  //��ESP8266����+++����͸����ʹESP8266����ָ��ģʽ
//	  AT_Mode = 0;
//		delay_ms(10000);
//		printf("%s\r\n","[ESP8266_DevLink]EXIT device link...");
//}


//void ESP8266_SendDat(void)
//{		
//	    int count=0;
//	  ok_flag=1;
//	  AT_Mode = 1;
//	  delay_ms(20000);
//	  SendCmd(CIPSEND, "OK", 10000);  //��ESP8266��������͸��ָ��
//	  AT_Mode = 0;
//		GetSendBuf();		
////	    for(count=0;count<30;count++)
////		{
////				delay_ms(300);
////				if((NULL != strstr((const char *)Rx_Buff,">")))
////				{
////						break;
////				}
////		}
//		send_pkg = PacketSavedataSimpleString(DEV_ID,send_buf);   
//		USART_Write((unsigned char *)send_pkg,strlen((const char *)send_pkg));	//��ƽ̨�ϴ����ݵ�
//		DeleteBuffer(&send_pkg);
//		AT_Mode = 1;
//		USART_Write("+++",3);  //��ESP8266����+++����͸����ʹESP8266����ָ��ģʽ
//		delay_ms(10000);
//		USART_Write("+++",3);  //��ESP8266����+++����͸����ʹESP8266����ָ��ģʽ
//		delay_ms(10000);
////		USART_Write("+++",3);  //��ESP8266����+++����͸����ʹESP8266����ָ��ģʽ
////		delay_ms(500);
//	  AT_Mode = 0;
//	  delay_ms(20000);
//}

/**
  * @brief  ����LED��ǰ״̬���ϴ����ݣ��ָ��ַ�����ʽ
**/
void GetSendBuf(void)
{
		char text[25] = {0};
			

		memset(send_buf,0,sizeof(send_buf));
		
		strcat(send_buf, ",;");	
		strcat(send_buf, "test,");
		sprintf(text,"%d",'a');
		strcat(send_buf, text);
		strcat(send_buf, ";");
		
//		strcat(send_buf, "green_statu,");
//		sprintf(text,"%d",'b');
//		strcat(send_buf, text);
//		strcat(send_buf, ";");

//		strcat(send_buf, "yellow_statu,");
//		sprintf(text,"%d",'c');
//		strcat(send_buf, text);
//		strcat(send_buf, ";");

//		strcat(send_buf, "blue_statu,");
//		sprintf(text,"%d",'d');
//		strcat(send_buf, text);
//		strcat(send_buf, ";");	
}
//void Send_Pack()
//{	
//    cJSON *json_data = cJSON_CreateObject();

//    cJSON_AddNumberToObject(json_data, "temperature", 18);

//    cJSON_AddNumberToObject(json_data, "humidity", 80);

//    EdpPacket* send_pkg = PacketSavedataJson(json_data, kTypeSimpleJsonWithoutTime, 0);

//    /*���ʹ洢������,�ϴ����ݵ�*/
//    int ret = DoSend (sockfd, send_pkg-&gt;_data, send_pkg-&gt;_write_pos);

//    /*ʹ��������ɾ��send_pkg �� ���������ڴ�й©*/
//    DeleteBuffer(&amp;send_pkg);

//    /*ɾ�������json����*/
//    cJSON_Delete(json_data);
//	
//}

//==========================================================
//	�������ƣ�	ESP8266_GetIPD
//
//	�������ܣ�	��ȡƽ̨���ص�����
//
//	��ڲ�����	�ȴ���ʱ��(����10ms)
//
//	���ز�����	ƽ̨���ص�ԭʼ����
//
//	˵����		��ͬ�����豸���صĸ�ʽ��ͬ����Ҫȥ����
//				��ESP8266�ķ��ظ�ʽΪ	"+IPD,x:yyy"	x�������ݳ��ȣ�yyy����������
//==========================================================
//unsigned char *ESP8266_GetIPD(unsigned short timeOut)
//{

//	char *ptrIPD = NULL;
//	
//	do
//	{
//		if(ESP8266_WaitRecive() == REV_OK)								//����������
//		{
//			ptrIPD = strstr((char *)esp8266_buf, "IPD,");				//������IPD��ͷ
//			if(ptrIPD == NULL)											//���û�ҵ���������IPDͷ���ӳ٣�������Ҫ�ȴ�һ�ᣬ�����ᳬ���趨��ʱ��
//			{
//				//UsartPrintf(USART_DEBUG, "\"IPD\" not found\r\n");
//			}
//			else
//			{
//				ptrIPD = strchr(ptrIPD, ':');							//�ҵ�':'
//				if(ptrIPD != NULL)
//				{
//					ptrIPD++;
//					return (unsigned char *)(ptrIPD);
//				}
//				else
//					return NULL;
//				
//			}
//		}
//		
//		DelayXms(5);													//��ʱ�ȴ�
//	} while(timeOut--);
//	
//	return NULL;														//��ʱ��δ�ҵ������ؿ�ָ��

//}

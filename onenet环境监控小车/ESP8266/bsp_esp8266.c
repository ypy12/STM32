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




/** ESP8266连接电脑wifi发数据
  *
  *
  *
   */
void ESP8266Mode_inti(void)
{
	  int i=0;
	  AT_Mode = 1;  //进入发AT指令模式
	
/****wifi模块与OneNet平台数据对接配置 */
    SendCmd(AT, "OK", 10000);		//模块有效性检查
    SendCmd(CWMODE, "OK", 10000);	//模块工作模式
    SendCmd(wifi_RST, "OK", 10000);		//模块重置
    SendCmd(CIFSR, "OK", 10000);		//查询网络信息
    SendCmd(CWJAP, "OK", 10000);		//配置需要连接的WIFI热点SSID和密码
    SendCmd(CIPSTART, "OK", 10000);	//TCP连接
    SendCmd(CIPMODE1, "OK", 10000);	//配置透传模式
//	  SendCmd(CIPSEND, ">", 10000);	//进入透传模式
     
  
/****wifi模块与电脑服务器数据对接配置   */
//	  SendCmd(AT, "OK", 10000);		//模块有效性检查
//	  SendCmd(wifi_RST, "OK", 10000);		//模块重置
//    SendCmd(CWMODE, "OK", 10000);	//模块工作模式 兼容STA和AP模式
//	  SendCmd(CWJAP, "OK", 10000);		//配置需要连接的WIFI热点SSID和密码 
//    SendCmd(Check_CWJAP, "OK", 10000);  //查询是否成功接入路由器 
//	  SendCmd(CIPMUX, "OK", 10000);  //开启多连模式
//	  SendCmd(CIPSERVER, "OK", 10000);//开启服务器模式
//	  SendCmd(CIFSR, "OK", 10000);   //查询路由器分配的服务器地址
//	  SendCmd(CIPMUX, "OK", 10000);  //再次开启多连模式
//		SendCmd(CIPSTO, "OK", 10000);  //设置服务器超时时间
		
		
  /***电脑作为服务器，wifi模块与其通信  
	  SendCmd(AT, "OK", 10000);		//模块有效性检查
	  SendCmd(wifi_RST, "OK", 10000);		//模块重置
    SendCmd(CWMODE, "OK", 10000);	//模块工作模式 兼容STA和AP模式
		SendCmd(CWJAP, "OK", 10000);		//配置需要连接的WIFI热点SSID和密码 
		SendCmd(CIFSR, "OK", 10000);   //查询路由器分配的服务器地址
		SendCmd(CIPSTART, "OK", 10000);   //接入电脑服务器  
		SendCmd(CIPMODE1, "OK", 10000);   //配置透传模式
		SendCmd(CIPSEND, "OK", 10000);   //开启透传模式
		*/
		
		 AT_Mode = 0;   //AT指令发送完毕 退出该模式
	}
/** 发送Cmd命令的函数
  *
  *
  *
  */ 
void SendCmd(char* cmd, char* result, int timeOut)
{
    while(1)
    {
//        memset(Rx_Buff, 0, sizeof(Rx_Buff));   //将数组所有数据清0
        USART_Write((unsigned char *)cmd,strlen((const char *)cmd));   //用串口把cmd命令写给ESP8266
        delay_ms(timeOut);                                          //延迟等待
			  LED=~LED;
        if(ok_flag==1)	//比较两个指针里面的数据是否一样，判断是否有预期的结果  和预期结果相同，表示指令设置成功，跳出
        {  
					  ok_flag=0;   //清空标志
            break;
        }
        else
        {
            delay_ms(100);
					
        }
    }
}
/** 串口发送函数
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
  * @brief   组HTTP POST报文
  * @param   pkt   报文缓存指针
  * @param   key   API_KEY定义在Main.c文件中，需要根据自己的设备修改
  *	@param 	 devid 设备ID，定义在main.c文件中，需要根据自己的设备修改
  *	@param 	 dsid  数据流ID
  *	@param 	 val   字符串形式的数据点的值
  * @retval  整个包的长度
  *///组成HTTP协议数据的格式
uint32_t HTTP_PostPkt(char *pkt, char *key, char *devid, char *dsid, char *val)
{
    char dataBuf[100] = {0};
    char lenBuf[10] = {0};
    *pkt = 0;

    sprintf(dataBuf, ",;%s,%s", dsid, val);     //采用分割字符串格式:type = 5
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
  * @brief  初始化ESP8266用到的GPIO引脚
  * @param  无
  * @retval 无
  */
void ESP8266_GPIO_Init(void)
{
	/*定义一个GPIO_InitTypeDef类型的结构体*/
	GPIO_InitTypeDef GPIO_InitStructure;


	/* 配置 CH_PD 引脚*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);

	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;               //设置推挽输出模式
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_14;                      //初始化CH-DP引脚 拉高
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;              //设置时钟
	GPIO_Init(GPIOA,&GPIO_InitStructure);   
	GPIO_SetBits(GPIOA,GPIO_Pin_14); 
  
}
/**
  * @brief  ESP8266初始化函数
  * @param  无
  * @retval 无
  */

void ESP8266_Init ( void )
{
	ESP8266_GPIO_Init(); 
	
	//SP8266_USART_Config(); 
	
	
	macESP8266_RST_HIGH_LEVEL();

	macESP8266_CH_ENABLE();
	
	
}

/**
  * @brief  初始化ESP8266用到的 USART
  * @param  无
  * @retval 无
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
	
	
	/* 中断配置 */
	USART_ITConfig ( macESP8266_USARTx, USART_IT_RXNE, ENABLE ); //使能串口接收中断 
	USART_ITConfig ( macESP8266_USARTx, USART_IT_IDLE, ENABLE ); //使能串口总线空闲中断 	

	ESP8266_USART_NVIC_Configuration ();
	
	
	USART_Cmd(macESP8266_USARTx, ENABLE);
	
	
}


/**
  * @brief  配置 ESP8266 USART 的 NVIC 中断
  * @param  无
  * @retval 无
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
 * 函数名：ESP8266_Rst
 * 描述  ：重启WF-ESP8266模块
 * 输入  ：无
 * 返回  : 无
 * 调用  ：被 ESP8266_AT_Test 调用
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
 * 函数名：ESP8266_Cmd
 * 描述  ：对WF-ESP8266模块发送AT指令
 * 输入  ：cmd，待发送的指令
 *         reply1，reply2，期待的响应，为NULL表不需响应，两者为或逻辑关系
 *         waittime，等待响应的时间
 * 返回  : 1，指令发送成功
 *         0，指令发送失败
 * 调用  ：被外部调用
 */
bool ESP8266_Cmd ( char * cmd, char * reply1, char * reply2, u32 waittime )
{    
	strEsp8266_Fram_Record .InfBit .FramLength = 0;               //从新开始接收新的数据包

	macESP8266_Usart ( "%s\r\n", cmd );

	if ( ( reply1 == 0 ) && ( reply2 == 0 ) )                      //不需要接收数据
		return true;
	
	delay_ms ( waittime );                 //延时
	
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
 * 函数名：ESP8266_ReceiveString
 * 描述  ：WF-ESP8266模块接收字符串
 * 输入  ：enumEnUnvarnishTx，声明是否已使能了透传模式
 * 返回  : 接收到的字符串首地址
 * 调用  ：被外部调用
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
  * @brief  和平台建立设备连接
**/
//void ESP8266_DevLink(const char* devid, const char* auth_key, int timeOut)
//{

//	  AT_Mode = 1;
//		SendCmd(CIPSEND, "OK", 10000);  //向ESP8266发送数据透传指令
//	  AT_Mode = 0;
//	
//		send_pkg = PacketConnect1(devid,auth_key);
//		delay_ms(2000);
//	  
//		USART_Write((unsigned char *)send_pkg,strlen((const char *)send_pkg));   //发送设备连接请求数据
//		
//		
//		delay_ms(5000);
//		DeleteBuffer(&send_pkg);
//		delay_ms(2000);
//	
//	  AT_Mode = 1;
//		USART_Write("+++",3);  //向ESP8266发送+++结束透传，使ESP8266返回指令模式
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
//	  SendCmd(CIPSEND, "OK", 10000);  //向ESP8266发送数据透传指令
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
//		USART_Write((unsigned char *)send_pkg,strlen((const char *)send_pkg));	//向平台上传数据点
//		DeleteBuffer(&send_pkg);
//		AT_Mode = 1;
//		USART_Write("+++",3);  //向ESP8266发送+++结束透传，使ESP8266返回指令模式
//		delay_ms(10000);
//		USART_Write("+++",3);  //向ESP8266发送+++结束透传，使ESP8266返回指令模式
//		delay_ms(10000);
////		USART_Write("+++",3);  //向ESP8266发送+++结束透传，使ESP8266返回指令模式
////		delay_ms(500);
//	  AT_Mode = 0;
//	  delay_ms(20000);
//}

/**
  * @brief  生成LED当前状态的上传数据，分割字符串格式
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

//    /*发送存储包数据,上传数据点*/
//    int ret = DoSend (sockfd, send_pkg-&gt;_data, send_pkg-&gt;_write_pos);

//    /*使用完后必须删除send_pkg ， 否则会造成内存泄漏*/
//    DeleteBuffer(&amp;send_pkg);

//    /*删除构造的json对象*/
//    cJSON_Delete(json_data);
//	
//}

//==========================================================
//	函数名称：	ESP8266_GetIPD
//
//	函数功能：	获取平台返回的数据
//
//	入口参数：	等待的时间(乘以10ms)
//
//	返回参数：	平台返回的原始数据
//
//	说明：		不同网络设备返回的格式不同，需要去调试
//				如ESP8266的返回格式为	"+IPD,x:yyy"	x代表数据长度，yyy是数据内容
//==========================================================
//unsigned char *ESP8266_GetIPD(unsigned short timeOut)
//{

//	char *ptrIPD = NULL;
//	
//	do
//	{
//		if(ESP8266_WaitRecive() == REV_OK)								//如果接收完成
//		{
//			ptrIPD = strstr((char *)esp8266_buf, "IPD,");				//搜索“IPD”头
//			if(ptrIPD == NULL)											//如果没找到，可能是IPD头的延迟，还是需要等待一会，但不会超过设定的时间
//			{
//				//UsartPrintf(USART_DEBUG, "\"IPD\" not found\r\n");
//			}
//			else
//			{
//				ptrIPD = strchr(ptrIPD, ':');							//找到':'
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
//		DelayXms(5);													//延时等待
//	} while(timeOut--);
//	
//	return NULL;														//超时还未找到，返回空指针

//}

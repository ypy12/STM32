/**
	************************************************************
	************************************************************
	************************************************************
	*	文件名： 	onenet.c
	*
	*	作者： 		张继瑞
	*
	*	日期： 		2017-05-08
	*
	*	版本： 		V1.1
	*
	*	说明： 		与onenet平台的数据交互接口层
	*
	*	版本修改记录：	V1.0：协议封装、返回判断都在同一个文件，并且不同协议接口不同。
	*				V1.1：提供统一接口供应用层使用，根据不同协议文件来封装协议相关的内容。
	
	*
	************************************************************
	************************************************************
	************************************************************
**/

//单片机头文件
#include "stm32f10x.h"

//网络设备
#include "esp8266.h"

//协议文件
#include "onenet.h"
#include "edpkit.h"

//硬件驱动
#include "usart.h"
#include "delay.h"
#include "led.h"

//C库
#include <string.h>
#include <stdio.h>


#define DEVID	"525099161"

#define APIKEY	"Z=uHKrj9dO2iMMnZ=vikZAUsNjY="

//==========================================================
//	函数名称：	OneNet_DevLink
//
//	函数功能：	与onenet创建连接
//
//	入口参数：	无
//
//	返回参数：	1-成功	0-失败
//
//	说明：		与onenet平台建立连接
//==========================================================
_Bool OneNet_DevLink(void)
{
	
	EDP_PACKET_STRUCTURE edpPacket = {NULL, 0, 0, 0};				//协议包
	
	unsigned char status = 1;
	

	if(EDP_PacketConnect1(DEVID, APIKEY, 256, &edpPacket) == 0)		//根据devid 和 apikey封装协议包
	{
		
		ESP8266_SendData(edpPacket._data, edpPacket._len);			//上传平台
	
		delay_ms(500);      //延迟等待时间大于300ms 一定要加 不然连不上
		EDP_DeleteBuffer(&edpPacket);								//删包
		status=0;
	}
	
	return status;
	
}

unsigned char OneNet_FillBuf(char *buf,int data,int data1,int data2,int data3)
{
	
	char text[16];
	
	memset(text, 0, sizeof(text));
	
	strcpy(buf, "{");
	
	memset(text, 0, sizeof(text));
	sprintf(text, "\"Green_Led\":%d,", 1);
	strcat(buf, text);
	
	memset(text, 0, sizeof(text));
	sprintf(text, "\"test\":%d,", data);
	strcat(buf, text);
	
	memset(text, 0, sizeof(text));
	sprintf(text, "\"Temperture\":%d,", data1);//text将温度数据传输进来
	strcat(buf, text);
	
	memset(text, 0, sizeof(text));
	sprintf(text, "\"humidity\":%d,", data2);//text将湿度数据
	strcat(buf, text);
	
		memset(text, 0, sizeof(text));
	sprintf(text, "\"fire\":%d", data3);//text将火焰数据
	strcat(buf, text);
	
	strcat(buf, "}");
	memset(text, 0, sizeof(text));
	return strlen(buf);

}

//==========================================================
//	函数名称：	OneNet_SendData
//
//	函数功能：	上传数据到平台
//
//	入口参数：	type：发送数据的格式
//
//	返回参数：	无
//
//	说明：		
//==========================================================
void OneNet_SendData(int data,int data1,int data2,int data3)
{
	
	EDP_PACKET_STRUCTURE edpPacket = {NULL, 0, 0, 0};												//协议包
	
	char buf[128];
	
	short body_len = 0, i = 0;

	memset(buf, 0, sizeof(buf));
	memset(Rx_Buff, 0, sizeof(Rx_Buff));
	Rx_count=0;
	body_len = OneNet_FillBuf(buf,data,data1,data2,data3);																	//获取当前需要发送的数据流的总长度
	
	if(body_len)
	{
		if(EDP_PacketSaveData(DEVID, body_len, NULL, kTypeSimpleJsonWithoutTime, &edpPacket) == 0)	//封包
		{
			for(; i < body_len; i++)
				edpPacket._data[edpPacket._len++] = buf[i];
			
			ESP8266_SendData(edpPacket._data, edpPacket._len);										//上传数据到平台
			
			EDP_DeleteBuffer(&edpPacket);															//删包	
		}
	}
	
}

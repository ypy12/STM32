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
/*   全局变量定义  */
unsigned int Heart_Pack=0;  //用于定时器TIM1自加计数，用于满足设定自加数值时发送EDP心跳包的标志位
u8  AT_Mode=0;     //发AT指令的模式 用于表示程序处于配置Wifi模块的模式  对串口接收数据进行不同的处理
u8  Contral_flag=0;  //用于存贮控制指令  判断进行什么操作

void System_Init()
{
/*  基本功能初始化   */
		delay_init();					//延时初始化
		uart_init(115200);    //打开串口2，与Wifi模块通信
		LED_Init();           //LED灯初始化
	
/*  配置wifi模块     */
		ESP8266_GPIO_Init();  //初始化ESP8266的CH_DP引脚，拉高即可
		ESP8266Mode_inti();   //初始化ESP8266的AT指令

/*  打开定时器       */
		Tim1_Init(5000,72);   //定时器周期配置为5MS
}

void Connect_OneNet(void)
{
     while(!OneNet_Connect)     //判断是否接入平台成功
	   {	
	    	while(OneNet_DevLink())			//接入OneNET
	   	  delay_ms(1000);       
	   }
}

extern unsigned char hanzi1[];
int main(void)
{	 
	u8 t = 0;
	u8 temperature; //定义温度 	    
	u8 humidity;//定义湿度
	u8 fire;//定义火焰
	
	delay_init();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //设置NVIC中断分组2:2位抢占优先级，2位响应优先级 
	
	DHT11_Init();
	OLED_Init();		    //初始化OLED
  initial_olcd();		  //初始化
	clear_screen();			//清屏
	Car_Init();
	delay_init();
	//LED_Init();
	fire_init();
	USART3_Init(38400);
	  fire=GPIO_ReadOutputDataBit(GPIOB,GPIO_Pin_9);
		EDP_PACKET_STRUCTURE edpPacket = {NULL, 0, 0, 0};   //定义一个结构体变量   供心跳包装载数据用
		
    System_Init();       //初始化各模块
		
		Connect_OneNet();    //接入OneNet平台

		while(1)	             //while循环执行各种功能 
		{  
      /*  定时发送心跳包  */			
			if(Heart_Pack==5000)              //每隔25秒发一次心跳包   OneNet平台默认四分钟无数据交换就会断开与设备的连接 发心跳包可以保证连接。
			{
				EDP_PacketPing(&edpPacket);     //装载并发送心跳包
				Heart_Pack=0;
			
			}    
			 
		/*用strstr函数来判断操作指令是否匹配 对该函数不了解的朋友自行百度吧*/	 
			 if(strstr((const char*)Message_Buf,"OPEN"))    //判断到操作指令为OPEN 
			 {
					LED_ON;   //打开LED
					OneNet_SendData(1,temperature,humidity);  //向平台发数据1
					delay_ms(20); 
					memset(Message_Buf, 0, sizeof(Message_Buf));    //执行完指令 清空指令存贮空间 防止继续执行该指令
			 }
			 if(strstr((const char*)Message_Buf,"CLOSE"))   //判断到操作指令为CLOSE 
			 {
					LED_OFF; //关闭LED
					OneNet_SendData(0,temperature,humidity);  //向平台发数据0
					delay_ms(20);   //延迟
					memset(Message_Buf, 0, sizeof(Message_Buf));    //执行完指令 清空指令存贮空间 防止继续执行该指令
			 }
			 
			/*****************OLED显示温湿度*******************/
		disp_string_8x16_16x16(1,1,"温度:");
		disp_string_8x16_16x16(3,1,"湿度:");
		disp_string_8x16_16x16(1,80,"C");
		disp_string_8x16_16x16(3,80,"%");
		disp_string_8x16_16x16(5,1,"我好菜啊");
		if(t%10==0)			//每100ms读取一次
		{									  
			DHT11_Read_Data(&temperature,&humidity);	//读取温湿度值	

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
		USART_SendData(USART3, d);//回显
		/* Loop until the end of transmission */
		while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET);
	}
}
		

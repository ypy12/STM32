#ifndef  __BSP_ESP8266_H
#define	 __BSP_ESP8266_H



#include "stm32f10x.h"
#include "common.h"
#include <stdio.h>
#include <stdbool.h>
#include "string.h"
extern u8  AT_Mode;     //发AT指令的模式 用于表示程序处于配置Wifi模块的模式  对串口接收数据进行不同的处理
extern u8  Contral_flag;  //用于存贮控制指令  判断进行什么操作

#define API_KEY     "LZDYWJJkQ0a4Wa1bu=cG2IRu3Ws="		//需要定义为用户自己的参数
#define DEV_ID      "30027152"							//需要定义为用户自己的参数

//#define API_KEY     "4js4HIFpihbdrVuZJmKm6syRyxo="		//需要定义为用户自己的参数
//#define DEV_ID      "29802187"							//需要定义为用户自己的参数

/******与OneNet服务器进行数据对接时的配置 */
#define AT          "AT\r\n"	
#define CWMODE      "AT+CWMODE=3\r\n"		//STA+AP模式
#define wifi_RST    "AT+RST\r\n"
#define CIFSR       "AT+CIFSR\r\n"
#define CWJAP       "AT+CWJAP=\"ICT\",\"icteam2017\"\r\n"	//ssid: onenet 密码：空
#define CIPSTART    "AT+CIPSTART=\"TCP\",\"jjfaedp.hedevice.com\",876\r\n"	//EDP服务器 183.230.40.39/876
//#define CIPSTART    "AT+CIPSTART=\"TCP\",\"183.230.40.33\",80\r\n"		//HTTP服务器183.230.40.33/80    
#define CIPMODE0    "AT+CIPMODE=0\r\n"		//非透传模式
#define CIPMODE1    "AT+CIPMODE=1\r\n"		//透传模式
#define CIPSEND     "AT+CIPSEND\r\n"   
#define Out_CIPSEND     "+++" 
#define CIPSTATUS   "AT+CIPSTATUS\r\n"		//网络状态查询



/*********与电脑服务器进行数据对接时的配置  
#define AT          "AT\r\n"	
#define CWMODE      "AT+CWMODE=3\r\n"		//STA+AP模式
#define wifi_RST    "AT+RST\r\n"
#define CWJAP       "AT+CWJAP=\"ICT\",\"icteam2017\"\r\n"	//ssid: ICT 密码：icteam2017
#define Check_CWJAP "AT+CWJAP?\r\n"	//查询是否接入路由器
#define CIFSR       "AT+CIFSR\r\n"  //查询路由器分配的地址
#define CIPMUX       "AT+CIPMUX=1\r\n"  //开启多连模式
#define CIPSERVER     "AT+CIPSERVER=1,8089\r\n"  //创建服务器
#define CIPSTO     "AT+CIPSTO=2880\r\n"    //设置服务器超时时间
*/


#if defined ( __CC_ARM   )
#pragma anon_unions
#endif



/******************************* ESP8266 数据类型定义 ***************************/
typedef enum{
	STA,
  AP,
  STA_AP  
} ENUM_Net_ModeTypeDef;


typedef enum{
	 enumTCP,
	 enumUDP,
} ENUM_NetPro_TypeDef;
	

typedef enum{
	Multiple_ID_0 = 0,
	Multiple_ID_1 = 1,
	Multiple_ID_2 = 2,
	Multiple_ID_3 = 3,
	Multiple_ID_4 = 4,
	Single_ID_0 = 5,
} ENUM_ID_NO_TypeDef;
	

typedef enum{
	OPEN = 0,
	WEP = 1,
	WPA_PSK = 2,
	WPA2_PSK = 3,
	WPA_WPA2_PSK = 4,
} ENUM_AP_PsdMode_TypeDef;



/******************************* ESP8266 外部全局变量声明 ***************************/
#define RX_BUF_MAX_LEN     1024                                     //最大接收缓存字节数

extern struct  STRUCT_USARTx_Fram                                  //串口数据帧的处理结构体
{
	char  Data_RX_BUF [ RX_BUF_MAX_LEN ];
	
  union {
    __IO u16 InfAll;
    struct {
		  __IO u16 FramLength       :15;                               // 14:0 
		  __IO u16 FramFinishFlag   :1;                                // 15 
	  } InfBit;
  }; 
	
} strEsp8266_Fram_Record;



/******************************** ESP8266 连接引脚定义 ***********************************/
#define      macESP8266_CH_PD_APBxClock_FUN                   RCC_APB2PeriphClockCmd
#define      macESP8266_CH_PD_CLK                             RCC_APB2Periph_GPIOA
#define      macESP8266_CH_PD_PORT                            GPIOA
#define      macESP8266_CH_PD_PIN                             GPIO_Pin_2

#define      macESP8266_RST_APBxClock_FUN                     RCC_APB2PeriphClockCmd
#define      macESP8266_RST_CLK                               RCC_APB2Periph_GPIOA
#define      macESP8266_RST_PORT                              GPIOA
#define      macESP8266_RST_PIN                               GPIO_Pin_3

 

#define      macESP8266_USART_BAUD_RATE                       115200

#define      macESP8266_USARTx                                USART3
#define      macESP8266_USART_APBxClock_FUN                   RCC_APB1PeriphClockCmd
#define      macESP8266_USART_CLK                             RCC_APB1Periph_USART3
#define      macESP8266_USART_GPIO_APBxClock_FUN              RCC_APB2PeriphClockCmd
#define      macESP8266_USART_GPIO_CLK                        RCC_APB2Periph_GPIOB     
#define      macESP8266_USART_TX_PORT                         GPIOB   
#define      macESP8266_USART_TX_PIN                          GPIO_Pin_10
#define      macESP8266_USART_RX_PORT                         GPIOB
#define      macESP8266_USART_RX_PIN                          GPIO_Pin_11
#define      macESP8266_USART_IRQ                             USART3_IRQn
#define      macESP8266_USART_INT_FUN                         USART3_IRQHandler



/*********************************************** ESP8266 函数宏定义 *******************************************/
#define     macESP8266_Usart( fmt, ... )           USART_printf ( macESP8266_USARTx, fmt, ##__VA_ARGS__ ) 
#define     macPC_Usart( fmt, ... )                printf ( fmt, ##__VA_ARGS__ )    

#define     macESP8266_CH_ENABLE()                 GPIO_SetBits ( macESP8266_CH_PD_PORT, macESP8266_CH_PD_PIN )
#define     macESP8266_CH_DISABLE()                GPIO_ResetBits ( macESP8266_CH_PD_PORT, macESP8266_CH_PD_PIN )

#define     macESP8266_RST_HIGH_LEVEL()            GPIO_SetBits ( macESP8266_RST_PORT, macESP8266_RST_PIN )
#define     macESP8266_RST_LOW_LEVEL()             GPIO_ResetBits ( macESP8266_RST_PORT, macESP8266_RST_PIN )



/****************************************** ESP8266 函数声明 ***********************************************/
void                     ESP8266_Init                        ( void );
void                     ESP8266_Rst                         ( void );
bool                     ESP8266_Cmd                         ( char * cmd, char * reply1, char * reply2, u32 waittime );
void                     ESP8266_AT_Test                     ( void );
bool                     ESP8266_Net_Mode_Choose             ( ENUM_Net_ModeTypeDef enumMode );
bool                     ESP8266_JoinAP                      ( char * pSSID, char * pPassWord );
bool                     ESP8266_BuildAP                     ( char * pSSID, char * pPassWord, ENUM_AP_PsdMode_TypeDef enunPsdMode );
bool                     ESP8266_Enable_MultipleId           ( FunctionalState enumEnUnvarnishTx );
bool                     ESP8266_Link_Server                 ( ENUM_NetPro_TypeDef enumE, char * ip, char * ComNum, ENUM_ID_NO_TypeDef id);
bool                     ESP8266_StartOrShutServer           ( FunctionalState enumMode, char * pPortNum, char * pTimeOver );
uint8_t                  ESP8266_Get_LinkStatus              ( void );
uint8_t                  ESP8266_Get_IdLinkStatus            ( void );
uint8_t                  ESP8266_Inquire_ApIp                ( char * pApIp, uint8_t ucArrayLength );
bool                     ESP8266_UnvarnishSend               ( void );
void                     ESP8266_ExitUnvarnishSend           ( void );
bool                     ESP8266_SendString                  ( FunctionalState enumEnUnvarnishTx, char * pStr, u32 ulStrLength, ENUM_ID_NO_TypeDef ucId );
char *                   ESP8266_ReceiveString               ( FunctionalState enumEnUnvarnishTx );
void ESP8266_GPIO_Init(void);
void                   ESP8266_USART_NVIC_Configuration    ( void );
void ESP8266_USART_Config( void );
void ESP8266Mode_inti(void);
void ESP8266Mode_inti(void);
void SendCmd(char* cmd, char* result, int timeOut);
void USART_Write(unsigned char *cmd, int len);
uint32_t HTTP_PostPkt(char *pkt, char *key, char *devid, char *dsid, char *val);
void ESP8266_DevLink(const char* devid, const char* auth_key, int timeOut);
void ESP8266_SendDat(void);
void GetSendBuf(void);
unsigned char *ESP8266_GetIPD(unsigned short timeOut);
extern unsigned char  usart2_rcv_buf[1024];
uint8_t                  ESP8266_CWLIF                       ( char * pStaIp );
uint8_t                  ESP8266_CIPAP                       ( char * pApIp );

extern char Rx_Buff[200];



#endif



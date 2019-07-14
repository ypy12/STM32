#include "sys.h"
#include "usart.h"	
#include "delay.h"
#include "string.h" 	 
#include "esp8266.h"
#include "led.h"


unsigned char Message_length;

int OneNet_Connect=0;      //用于判断wifi模块是否连接上OneNet平台的标志

char Rx_Buff[200];
int  Rx_count=0;   //用于ESP8266判断接受数据的多少
int  ok_flag=0;
char Message_Buf[20];  //用于存贮操作指令
u16 USART_RX_STA=0;       //接收状态标记	  


//初始化IO 串口2
void uart_init(u32 bound)
{
		//GPIO端口设置
		GPIO_InitTypeDef GPIO_InitStructure;
		USART_InitTypeDef USART_InitStructure;
		NVIC_InitTypeDef NVIC_InitStructure;

		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);	//使能USART2
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
		USART_DeInit(USART2);  //复位串口1
		//USART2_TX   PA.2
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; //PA.9
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
		GPIO_Init(GPIOA, &GPIO_InitStructure); //初始化PA2

		//USART2_RX	  PA.3
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
		GPIO_Init(GPIOA, &GPIO_InitStructure);  //初始化PA3

		//USART2 NVIC 配置

		NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//抢占优先级3
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
		NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器

		//USART 初始化设置

		USART_InitStructure.USART_BaudRate = bound;//一般设置为9600;
		USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
		USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
		USART_InitStructure.USART_Parity = USART_Parity_No;//偶校验位
		USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
		USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式

		USART_Init(USART2, &USART_InitStructure); //初始化串口
		USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//开启中断
		USART_Cmd(USART2, ENABLE);                    //使能串口 

}


void Receive_AT_Data()
{    //判断ESP8266是否返回'OK'、'>'、'ready' 
		if(strstr(Rx_Buff,"OK")||strstr(Rx_Buff,">")||strstr(Rx_Buff,"ready"))    
		{
			 memset(Rx_Buff, 0, sizeof(Rx_Buff));  //清空Rx_Buff数组
			 ok_flag=1;			                //判断成功，标志位置1
			 Rx_count=0;				
		}
		else                                   //否则继续接受数据
		{
			 Rx_count++;	
			 if(Rx_count==150)                   //最多接收150个数据，数据溢出 清空数组
			 {
					Rx_count=0;
					memset(Rx_Buff, 0, sizeof(Rx_Buff));
			 }
		}
}

void Receive_Normal_Data()
{
	  static int i;        //静态变量i
	
		if(Rx_count==150)                   //最多接收150个数据，数据溢出 清空数组
		{
			Rx_count=0;                 	
			memset(Rx_Buff, 0, sizeof(Rx_Buff));  //清空当前数组所有数据
		}

		if(Message_length>0)                   //表示可以开始存贮操作指令
		{
				Message_Buf[i]=Rx_Buff[Rx_count];   //存贮操作指令数据
				i++;
				Message_length--;                  //存一个指令，剩余数量减一,判断操作指令是否存贮完成
		}
		 
		if(Rx_count>3&&Rx_Buff[Rx_count-2]==0&&Rx_Buff[Rx_count-1]==0&&Rx_Buff[Rx_count]>0)   
		//如果当前接收到的数据大于0，并且这个数据的前两个数据为00 代表当前数据就是操作指令的长度。
		{
			 memset(Message_Buf, 0, sizeof(Message_Buf)); //清空存贮操作指令的数组，准备存贮新的操作指令
			
			 Message_length=Rx_Buff[Rx_count];      //将接收到的数据存为操作指令长度。
			 i=0;                                   //清空i
		}
		Rx_count++;                               //准备存储下一个数据 
						
}


void USART2_IRQHandler(void)                	//串口1中断服务程序
{
		u8 Res;	

		if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
		{
			
			Res =USART_ReceiveData(USART2);//(USART2->DR);	//读取接收到的数据	
			Rx_Buff[Rx_count]=Res;
			
	  	/*  配置AT指令模式下 数据的处理 */
			if(AT_Mode==1)//配置AT指令下 数据的处理
			{
				 Receive_AT_Data();  //处理AT指令模式下的数据
		  }
			
			/*  连接平台后 数据的处理 */
			else if(AT_Mode==0)    //正常模式接收数据
			{			 	
				 Receive_Normal_Data();  //正常模式下处理数据
			}
           
			USART_ClearITPendingBit(USART2,USART_IT_RXNE);
		 }	 
		
		 
		/**********判断WiFi模块是否连接上OneNet 平台*******/
		 {
		  if(Rx_Buff[Rx_count-3]==0x20&&Rx_Buff[Rx_count-2]==0x02&&Rx_Buff[Rx_count-1]==0x00&&Rx_Buff[Rx_count]==0x00)  //连接上平台时 平台会返回 20 02 00 00给我们
			{
			     OneNet_Connect=1;         //连接完成的标志位
			}
		 }
		 if(Rx_Buff[Rx_count-2]==0x64&&Rx_Buff[Rx_count-1]==0x01&&Rx_Buff[Rx_count]==0x60)  //连接上平台时 平台会返回 20 02 00 00给我们
		 {
			     OneNet_Connect=0;         //连接断开
			
		 }
		 
} 


void USART3_Init(u32 baud)   
{  
    USART_InitTypeDef USART_InitStructure;  
    NVIC_InitTypeDef NVIC_InitStructure;   
    GPIO_InitTypeDef GPIO_InitStructure;    //声明一个结构体变量，用来初始化GPIO  
    //使能串口的RCC时钟  
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB , ENABLE); //使能UART3所在GPIOB的时钟  
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);  

    //串口使用的GPIO口配置  
    // Configure USART3 Rx (PB.11) as input floating    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;  
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  
    GPIO_Init(GPIOB, &GPIO_InitStructure);  

    // Configure USART3 Tx (PB.10) as alternate function push-pull  
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;  
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  
    GPIO_Init(GPIOB, &GPIO_InitStructure);  

    //配置串口  
    USART_InitStructure.USART_BaudRate = baud;  
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;  
    USART_InitStructure.USART_StopBits = USART_StopBits_1;  
    USART_InitStructure.USART_Parity = USART_Parity_No;  
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;  
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;  


    // Configure USART3   
    USART_Init(USART3, &USART_InitStructure);//配置串口3 
    // Enable USART3 Receive interrupts 使能串口接收中断  
    USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);  
    // Enable the USART3   
    USART_Cmd(USART3, ENABLE);//使能串口3  

    //串口中断配置  
    //Configure the NVIC Preemption Priority Bits     
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  

    // Enable the USART3 Interrupt   
    NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;   
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//抢占优先级3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;        //子优先级3
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  
    NVIC_Init(&NVIC_InitStructure);       
      
} 


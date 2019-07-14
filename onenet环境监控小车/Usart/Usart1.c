//#include "Usart1.h"
//#include "stm32f10x.h"
//Door door;//门的开关状态

//void Data_Receive_Prepare(u8 data);
//void Data_Receive_Anl(u8 *data_buf,u8 num);


////
//void USART_Config(u32 baud)
//{ 
//    GPIO_InitTypeDef GPIO_InitStructure;
//    USART_InitTypeDef USART_InitStructure;
//   	NVIC_InitTypeDef NVIC_InitStructure; 

//    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_USART1,ENABLE);
//    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);  
//    
//    //接收Rx A10 与Tx A9 脚  
//    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_9; //Tx
//    GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
//    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;
//    GPIO_Init(GPIOA,&GPIO_InitStructure);

//    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_10;//RX
//    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;
//    GPIO_Init(GPIOA,&GPIO_InitStructure);     

//    USART_InitStructure.USART_BaudRate=baud;    //设置波特率为9600|115200都可以
//    USART_InitStructure.USART_WordLength=USART_WordLength_8b;   //数据长度为8位
//    USART_InitStructure.USART_StopBits=USART_StopBits_1;
//    USART_InitStructure.USART_Parity=USART_Parity_No;       //是否打开奇偶校验
//    USART_InitStructure.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
//    USART_InitStructure.USART_Mode=USART_Mode_Rx|USART_Mode_Tx;

//    USART_Init(USART1,&USART_InitStructure);
//    USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);
//    USART_Cmd(USART1,ENABLE);
//    USART_ClearFlag(USART1,USART_FLAG_TC);
//    
//    //以下是中断优先级配置
//	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 
//   
//	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; 
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1; 
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
//	NVIC_Init(&NVIC_InitStructure);
//    
//}




//void USART1_IRQHandler(void)
//{
//    if(USART1->SR & USART_SR_ORE)//ORE中断
//	{
//		u8 com_data = USART1->DR;//USART_ClearFlag(USART1,USART_IT_ORE);
//	}
//    
//	if(USART1->SR & (1<<5))//if(USART_GetITStatus(USART1,USART_IT_RXNE)!=RESET)
//   {
//       u8 com_data = USART1->DR;
////       printf("%c",com_data);
//       Data_Receive_Prepare(com_data);
//   }

//}

////Data_Receive_Prepare函数是协议预解析，根据协议的格式，将收到的数据进行一次格式性解析，格式正确的话再进行数据解析
////移植时，此函数应由用户根据自身使用的通信方式自行调用，比如串口每收到一字节数据，则调用此函数一次
////此函数解析出符合格式的数据帧后，会自行调用数据解析函数
//void Data_Receive_Prepare(u8 data)
//{
//	static u8 RxBuffer[50];
//	static u8 _data_len = 0,_data_cnt = 0;
//	static u8 state = 0;
//	
//	if(state==0&&data==0xAA)
//	{
//		state=1;
//		RxBuffer[0]=data;
//	}
//	else if(state==1&&data==0xAF)
//	{
//		state=2;
//		RxBuffer[1]=data;
//	}
//	else if(state==2&&data<0XF1)//功能字
//	{
//		state=3;
//		RxBuffer[2]=data;
//	}
//	else if(state==3&&data<50)//长度
//	{
//		state = 4;
//		RxBuffer[3]=data;
//		_data_len = data;
//		_data_cnt = 0;
//	}
//	else if(state==4&&_data_len>0)
//	{
//		_data_len--;
//		RxBuffer[4+_data_cnt++]=data;//数据
//		if(_data_len==0)
//			state = 5;
//	}
//	else if(state==5)
//	{
//		state = 0;
//		RxBuffer[4+_data_cnt]=data;//校正码
//		Data_Receive_Anl(RxBuffer,_data_cnt+5);//长度=数据cnt+帧头4+校正码1
//	}
//	else
//		state = 0;
//}


////Data_Receive_Anl函数是协议数据解析函数，函数参数是符合协议格式的一个数据帧，该函数会首先对协议数据进行校验
////校验通过后对数据进行解析，实现相应功能
////此函数可以不用用户自行调用，由函数Data_Receive_Prepare自动调用
//void Data_Receive_Anl(u8 *data_buf,u8 num)
//{
//	u8 sum = 0,i=0;
//	for(i=0;i<(num-1);i++)
//		sum += *(data_buf+i);
//	if(!(sum==*(data_buf+num-1)))		return;		//判断sum
//	if(!(*(data_buf)==0xAA && *(data_buf+1)==0xAF))		return;		//判断帧头
//	
//	if(*(data_buf+2)==0X01)//以下是功能
//	{
//        switch(*(data_buf+4))
//        {
//            case 0XAB://门是开的状态
//                door.status='Y';
//                break;
//            case 0xCD://门是关的状态
//                door.status='N';
//                break;
//            
//            default:break;
//        }
//        
//        
//	}
//	
//}

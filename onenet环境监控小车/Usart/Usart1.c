//#include "Usart1.h"
//#include "stm32f10x.h"
//Door door;//�ŵĿ���״̬

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
//    //����Rx A10 ��Tx A9 ��  
//    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_9; //Tx
//    GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
//    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;
//    GPIO_Init(GPIOA,&GPIO_InitStructure);

//    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_10;//RX
//    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;
//    GPIO_Init(GPIOA,&GPIO_InitStructure);     

//    USART_InitStructure.USART_BaudRate=baud;    //���ò�����Ϊ9600|115200������
//    USART_InitStructure.USART_WordLength=USART_WordLength_8b;   //���ݳ���Ϊ8λ
//    USART_InitStructure.USART_StopBits=USART_StopBits_1;
//    USART_InitStructure.USART_Parity=USART_Parity_No;       //�Ƿ����żУ��
//    USART_InitStructure.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
//    USART_InitStructure.USART_Mode=USART_Mode_Rx|USART_Mode_Tx;

//    USART_Init(USART1,&USART_InitStructure);
//    USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);
//    USART_Cmd(USART1,ENABLE);
//    USART_ClearFlag(USART1,USART_FLAG_TC);
//    
//    //�������ж����ȼ�����
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
//    if(USART1->SR & USART_SR_ORE)//ORE�ж�
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

////Data_Receive_Prepare������Э��Ԥ����������Э��ĸ�ʽ�����յ������ݽ���һ�θ�ʽ�Խ�������ʽ��ȷ�Ļ��ٽ������ݽ���
////��ֲʱ���˺���Ӧ���û���������ʹ�õ�ͨ�ŷ�ʽ���е��ã����紮��ÿ�յ�һ�ֽ����ݣ�����ô˺���һ��
////�˺������������ϸ�ʽ������֡�󣬻����е������ݽ�������
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
//	else if(state==2&&data<0XF1)//������
//	{
//		state=3;
//		RxBuffer[2]=data;
//	}
//	else if(state==3&&data<50)//����
//	{
//		state = 4;
//		RxBuffer[3]=data;
//		_data_len = data;
//		_data_cnt = 0;
//	}
//	else if(state==4&&_data_len>0)
//	{
//		_data_len--;
//		RxBuffer[4+_data_cnt++]=data;//����
//		if(_data_len==0)
//			state = 5;
//	}
//	else if(state==5)
//	{
//		state = 0;
//		RxBuffer[4+_data_cnt]=data;//У����
//		Data_Receive_Anl(RxBuffer,_data_cnt+5);//����=����cnt+֡ͷ4+У����1
//	}
//	else
//		state = 0;
//}


////Data_Receive_Anl������Э�����ݽ������������������Ƿ���Э���ʽ��һ������֡���ú��������ȶ�Э�����ݽ���У��
////У��ͨ��������ݽ��н�����ʵ����Ӧ����
////�˺������Բ����û����е��ã��ɺ���Data_Receive_Prepare�Զ�����
//void Data_Receive_Anl(u8 *data_buf,u8 num)
//{
//	u8 sum = 0,i=0;
//	for(i=0;i<(num-1);i++)
//		sum += *(data_buf+i);
//	if(!(sum==*(data_buf+num-1)))		return;		//�ж�sum
//	if(!(*(data_buf)==0xAA && *(data_buf+1)==0xAF))		return;		//�ж�֡ͷ
//	
//	if(*(data_buf+2)==0X01)//�����ǹ���
//	{
//        switch(*(data_buf+4))
//        {
//            case 0XAB://���ǿ���״̬
//                door.status='Y';
//                break;
//            case 0xCD://���ǹص�״̬
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

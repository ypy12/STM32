#include "delay.h"
#include "usart.h"
#include "remote.h"  
#include "MQ-135_ADC2.h"
#include "oled.h"
#include "ADC.h"
#include "dht11.h"
#include "ds18b20.h"
#include "lsens.h"
u8 key;              // �����ֵ
u8 a;                // a=1�ж�����>100ppm����֮С��
u16 temperature;    //  ds18b20���¶�
u8 temp;            //   DHT11���¶���Ϊ��׼�����Բ���
u8 humidity;        //   DHT11��ʪ��
u16 vx=15542,vy=11165;  //�������ӣ���ֵ����1000֮���ʾ���ٸ�ADֵ����һ�����ص�
u16 chx=140,chy=146;//Ĭ�����ص�����Ϊ0ʱ��AD��ʼֵ
void xianshi()
{
	u8 adcx;            //   ����ֵ
	u16 air;           //     �������к�����Ũ��ֵ
  u16 sound;             //   �����ֱ�ֵ
	u32 temp_val=0;       
	u8 t;               
	u32 air_READ_TIMES=10;
	BACK_COLOR=WHITE;          //   ��ʾ������ɫ
	POINT_COLOR=RED;	           //   ����ɫ
	LCD_ShowString(10,20,"temperature:");   
	LCD_ShowString(10,50,"humidity:");      
	LCD_ShowString(105,50,"%");            
	DHT11_Read_Data(&temp,&humidity);          //	��ȡʪ��	   
	LCD_ShowNum(85,50,humidity,2);			       //	��ʾʪ��			  
			temperature=DS18B20_Get_Temp();    //��ȡ�¶�
			if(temperature<0)                // �ж��¶�����
			{
				LCD_ShowString(110,20,"-");		//��ʾ����
				temperature=-temperature;	
			LCD_ShowNum(125,20,temperature/10,3);//��ʾ��������	
			LCD_ShowString(145,20,".");    
   			LCD_ShowNum(147,20,temperature%10,1);				
			}
			else LCD_ShowString(110,20," ");			//ȥ������
			LCD_ShowNum(125,20,temperature/10,2);//��ʾ��������	
			LCD_ShowString(145,20,".");    
   			LCD_ShowNum(147,20,temperature%10,2);	//��ʾС������ 		   
	
			//��ʾ����
	adcx=Lsens_Get_Val();    //��ʾADC3ͨ��6��ֵ  
	LCD_ShowString(10,110,"Brightness:");
	LCD_ShowNum(95,110,adcx,3);  
		
			//��ʾ�ֱ�
	sound=sound_Get_Val();   //��ʾADC1ͨ��8��ֵ 
  if(sound<40)sound=53;
	LCD_ShowString(10,80,"Sound:");
	LCD_ShowString(95,80,"dB");
	delay_ms(50);
	LCD_ShowNum(65,80,sound,3);
			
			//��ʾ��������	
	for(t=0;t<air_READ_TIMES;t++)
	{
		temp_val+=ADC_GetConversionValue(ADC2);;	//��ȡADC2 10ͨ��ֵ
		delay_ms(5);
	}
	temp_val/=air_READ_TIMES;//�õ�ƽ��ֵ 
	air=((1000*temp_val/4095)+10);	//��ȡ��ģ����ӳ�䵽10-1000ppm		
	if(air>1000)air=1000;
	if(air>200)a=1;                 //Ũ�ȴ���200ppm��������������
	LCD_ShowString(10,140,"Air quality:");
	LCD_ShowNum(110,140,air,4);
	LCD_ShowString(150,140,"ppm");		

		} 
void firewarning()                                        //  ��ʾ���־���
{
	  LCD_ShowString(40,170,"Fire Warning           ");

}
void gaswarning()                                              // ��ʾ����й©����
{
	  LCD_ShowString(40,170,"Gas Leak Warning       ");
}
void airwarning()                                                 //  ��ʾ�ж����徯��
{
	  LCD_ShowString(40,170,"Deleterious Gas Warning");
}
 void fmq()                                                   //  �������˿�PE1
   {
	     GPIO_InitTypeDef GPIO_InitStructure;                                 
	     RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE,ENABLE);
	     GPIO_InitStructure.GPIO_Pin=GPIO_Pin_1;
       GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP; 
	     GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
       GPIO_Init(GPIOE,&GPIO_InitStructure); 	
}

int main(void)
{	 
	 fmq();
   ADC1_Config();         
	 ADC2_Config();
	 uart_init(115200);
	 NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
   delay_init()	; 
	 Remote_Init();   // �����ʼ��
	 Lcd_Init();			//��ʼ��OLED  
	 LCD_Clear(WHITE); //����
	 BACK_COLOR=WHITE;;POINT_COLOR=RED;	
   DHT11_Init()	;   //ʪ�ȴ�������ʼ��
   DS18B20_Init();  //�¶ȴ�������ʼ��
	 Lsens_Init();    //���������ʼ��
	 xianshi();       //��ʾ��Ϣ
	while(1)
	{
		xianshi();
	  key=Remote_Scan();
		if(key)
		{
			switch(key)
			{
				case 162:
				{
          Lcd_Init();                  //  ������Ļ
					LCD_Clear(WHITE);
					xianshi();
					while(key==162)key=Remote_Scan();
				}
				break;			   
				case 98:                              // �ر���Ļ
				{
		      GPIO_ResetBits(GPIOD, GPIO_Pin_9);
		    }
				break;
        case 226:                         //   �رշ���������Ļ������ʾ
				{
				  GPIO_ResetBits(GPIOE, GPIO_Pin_1);
					a=0;
					LCD_ShowString(40,170,"                       ");     
				}
				break;	
			default:break;	
			}
		}
      if(!GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_0)) // Һ����й©����
       {
	       GPIO_SetBits(GPIOE, GPIO_Pin_1);
         gaswarning();
       }    
		
      if(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_2))//  ���ֱ���
       {
	       GPIO_SetBits(GPIOE, GPIO_Pin_1);
				 firewarning();
       } 
			  if(DS18B20_Get_Temp()>270)                //  �¶ȴ���40�汨��  
       {
	       GPIO_SetBits(GPIOE, GPIO_Pin_1);
         firewarning();
       } 
			  if(a==1)                                  // �ж�����Ũ�ȳ���200ppm����
       {
	       GPIO_SetBits(GPIOE, GPIO_Pin_1);
         airwarning();
       }  
		 
       }
}



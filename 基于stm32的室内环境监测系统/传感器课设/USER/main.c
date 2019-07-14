#include "delay.h"
#include "usart.h"
#include "remote.h"  
#include "MQ-135_ADC2.h"
#include "oled.h"
#include "ADC.h"
#include "dht11.h"
#include "ds18b20.h"
#include "lsens.h"
u8 key;              // 红外键值
u8 a;                // a=1有毒气体>100ppm，反之小于
u16 temperature;    //  ds18b20的温度
u8 temp;            //   DHT11的温定因为不准，所以不用
u8 humidity;        //   DHT11的湿度
u16 vx=15542,vy=11165;  //比例因子，此值除以1000之后表示多少个AD值代表一个像素点
u16 chx=140,chy=146;//默认像素点坐标为0时的AD起始值
void xianshi()
{
	u8 adcx;            //   亮度值
	u16 air;           //     空气中有害气体浓度值
  u16 sound;             //   声音分贝值
	u32 temp_val=0;       
	u8 t;               
	u32 air_READ_TIMES=10;
	BACK_COLOR=WHITE;          //   显示屏背景色
	POINT_COLOR=RED;	           //   字体色
	LCD_ShowString(10,20,"temperature:");   
	LCD_ShowString(10,50,"humidity:");      
	LCD_ShowString(105,50,"%");            
	DHT11_Read_Data(&temp,&humidity);          //	读取湿度	   
	LCD_ShowNum(85,50,humidity,2);			       //	显示湿度			  
			temperature=DS18B20_Get_Temp();    //读取温度
			if(temperature<0)                // 判断温度正负
			{
				LCD_ShowString(110,20,"-");		//显示负号
				temperature=-temperature;	
			LCD_ShowNum(125,20,temperature/10,3);//显示正数部分	
			LCD_ShowString(145,20,".");    
   			LCD_ShowNum(147,20,temperature%10,1);				
			}
			else LCD_ShowString(110,20," ");			//去掉负号
			LCD_ShowNum(125,20,temperature/10,2);//显示正数部分	
			LCD_ShowString(145,20,".");    
   			LCD_ShowNum(147,20,temperature%10,2);	//显示小数部分 		   
	
			//显示亮度
	adcx=Lsens_Get_Val();    //显示ADC3通道6的值  
	LCD_ShowString(10,110,"Brightness:");
	LCD_ShowNum(95,110,adcx,3);  
		
			//显示分贝
	sound=sound_Get_Val();   //显示ADC1通道8的值 
  if(sound<40)sound=53;
	LCD_ShowString(10,80,"Sound:");
	LCD_ShowString(95,80,"dB");
	delay_ms(50);
	LCD_ShowNum(65,80,sound,3);
			
			//显示空气质量	
	for(t=0;t<air_READ_TIMES;t++)
	{
		temp_val+=ADC_GetConversionValue(ADC2);;	//读取ADC2 10通道值
		delay_ms(5);
	}
	temp_val/=air_READ_TIMES;//得到平均值 
	air=((1000*temp_val/4095)+10);	//读取的模拟量映射到10-1000ppm		
	if(air>1000)air=1000;
	if(air>200)a=1;                 //浓度大于200ppm触发蜂鸣器报警
	LCD_ShowString(10,140,"Air quality:");
	LCD_ShowNum(110,140,air,4);
	LCD_ShowString(150,140,"ppm");		

		} 
void firewarning()                                        //  显示火灾警告
{
	  LCD_ShowString(40,170,"Fire Warning           ");

}
void gaswarning()                                              // 显示气体泄漏警告
{
	  LCD_ShowString(40,170,"Gas Leak Warning       ");
}
void airwarning()                                                 //  显示有毒气体警告
{
	  LCD_ShowString(40,170,"Deleterious Gas Warning");
}
 void fmq()                                                   //  蜂鸣器端口PE1
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
	 Remote_Init();   // 红外初始化
	 Lcd_Init();			//初始化OLED  
	 LCD_Clear(WHITE); //清屏
	 BACK_COLOR=WHITE;;POINT_COLOR=RED;	
   DHT11_Init()	;   //湿度传感器初始化
   DS18B20_Init();  //温度传感器初始化
	 Lsens_Init();    //光敏电阻初始化
	 xianshi();       //显示信息
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
          Lcd_Init();                  //  开启屏幕
					LCD_Clear(WHITE);
					xianshi();
					while(key==162)key=Remote_Scan();
				}
				break;			   
				case 98:                              // 关闭屏幕
				{
		      GPIO_ResetBits(GPIOD, GPIO_Pin_9);
		    }
				break;
        case 226:                         //   关闭蜂鸣器和屏幕警告显示
				{
				  GPIO_ResetBits(GPIOE, GPIO_Pin_1);
					a=0;
					LCD_ShowString(40,170,"                       ");     
				}
				break;	
			default:break;	
			}
		}
      if(!GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_0)) // 液化气泄漏报警
       {
	       GPIO_SetBits(GPIOE, GPIO_Pin_1);
         gaswarning();
       }    
		
      if(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_2))//  火灾报警
       {
	       GPIO_SetBits(GPIOE, GPIO_Pin_1);
				 firewarning();
       } 
			  if(DS18B20_Get_Temp()>270)                //  温度大于40℃报警  
       {
	       GPIO_SetBits(GPIOE, GPIO_Pin_1);
         firewarning();
       } 
			  if(a==1)                                  // 有毒气体浓度超过200ppm警告
       {
	       GPIO_SetBits(GPIOE, GPIO_Pin_1);
         airwarning();
       }  
		 
       }
}



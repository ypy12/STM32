#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "timer.h"
#include "lcd.h"
#include "led.h"
  double Nx;;
  double N0;
  double speed;
 int main(void)
 {
 	SystemInit(); 			 //ϵͳʱ�ӳ�ʼ��Ϊ72M	  SYSCLK_FREQ_72MHz
	delay_init(72);	    	 //��ʱ������ʼ��	  
	uart_init(9600);
	LED_Init();
	NVIC_Configuration(); 	 //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	LED_Init();
 	Timer2_Init(0xFFFF,0);
	LCD_Init();	
	LCD_Fill(0,0,240,320,GREEN);
	LED0=0;     
		while(1)
	 {
		 Nx=getspeed2();
		 N0=getspeed1();	
		 speed=(Nx/N0)*2000000;	

		 LCD_ShowNum(0,10,N0,8,16);
		 LCD_ShowNum(0,40,Nx,8,16);
		 LCD_ShowNum(0,80,speed,8,16);	  
	}
 }

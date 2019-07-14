#ifndef __OLED_H
#define __OLED_H			  	 
#include "sys.h"
#include "stdlib.h"	

//-----------------OLED�˿ڶ���----------------  					   
#define SCL  PBout(0)  //�ӿڶ���:lcd_sclk����LCD��SCLK   //SCLK�ӵ���B0����
#define SDA   PBout(1)	 //�ӿڶ���:lcd_sda����LCD��SDA	    //SDIN�ӵ���B1����
#define lcd_reset PCout(3)	 //�ӿڶ���:lcd_reset����LCD��RESET 


//OLED�����ú���
void OLED_Init(void);        //OLED �ӿڳ�ʼ��
void initial_olcd(void);     //OLED ������ʼ��
void clear_screen(void);
void full_display(u8 data1,u8 data2);
void disp_string_8x16_16x16(u8 page,u8 column,const u8 *text);
void display_string_5x8(u16 page,u16 column,u8 reverse,const char *text);
void display_graphic_64x64(u8 page,u8 column,u8 *dp);
void display_graphic_128x64(u8 page,u8 column,u8 *dp);
void display_graphic_32x32(u8 page,u8 column,const u8 *dp);
void display_graphic_16x8(u8 page,u8 column,u16 *dp);
void display_number_16x8(u8 page,u8 colum,u8 number);

#endif  
	 

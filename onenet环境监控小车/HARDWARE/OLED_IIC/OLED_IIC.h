#ifndef __OLED_H
#define __OLED_H			  	 
#include "sys.h"
#include "stdlib.h"	

//-----------------OLED端口定义----------------  					   
#define SCL  PBout(0)  //接口定义:lcd_sclk就是LCD的SCLK   //SCLK接到“B0”脚
#define SDA   PBout(1)	 //接口定义:lcd_sda就是LCD的SDA	    //SDIN接到“B1”脚
#define lcd_reset PCout(3)	 //接口定义:lcd_reset就是LCD的RESET 


//OLED控制用函数
void OLED_Init(void);        //OLED 接口初始化
void initial_olcd(void);     //OLED 参数初始化
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
	 

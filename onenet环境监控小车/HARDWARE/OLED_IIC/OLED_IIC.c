#include "OLED_IIC.h"
#include "stdlib.h"
#include "delay.h"

#include "ASCII_CODE_8X16_5X8_VERTICAL.h"
#include "Chinese_And_Graphic.h"

void start()
{
     SCL=1;
     SDA=1;
     SDA=0;
     SCL=0;
}
void stop()
{
     SCL=0;
     SDA=0;
     SDA=1;
     SCL=1;
}

void write_w(unsigned char dat)
{
     unsigned char m,da;
     unsigned char j;
     da=dat;
     for(j=0;j<8;j++)
{
     m=da;
     SCL=0;
     m=m&0x80;
     if(m==0x80)
{
     SDA=1;
}
     else
{
     SDA=0;
}
     da=da<<1;
     SCL=1;
}
     SCL=0;
     delay_us(3); 
     SCL=1;
}



//写指令到OLED显示模块
void transfer_command(int data1)   
{
     start();
     write_w(0x78);
     write_w(0x00);
     write_w(data1);
     stop();
}

//写数据到OLED显示模块
void transfer_data(int data1)
{
     start();
     write_w(0x78);
     write_w(0x40);
     write_w(data1);
     stop();
}

void OLED_Init(void)
{

	GPIO_InitTypeDef  GPIO_InitStructure;
 	
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	 //使能PB端口时钟

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;	 //PB0,PB1推挽输出  
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//速度50MHz
 	GPIO_Init(GPIOB, &GPIO_InitStructure);	  //初始化GPIOB0,1
}

//OLED显示模块初始化
void initial_olcd(void)
{
  
  	lcd_reset=0;        	//低电平复位
  	delay_ms(500);
  	lcd_reset=1;		   	  //复位完毕
	       
	transfer_command(0xae);	//关显示
	transfer_command(0xd5);	//晶振频率
	transfer_command(0x80);
	transfer_command(0xa8);	//duty设置
	transfer_command(0x3f);	//duty=1/64
	transfer_command(0xd3);	//显示偏移
	transfer_command(0x00);	
	transfer_command(0x40);	//起始行
	transfer_command(0x8d);	//升压允许
	transfer_command(0x14);	
	transfer_command(0x20);	//page address mode
	transfer_command(0x02);
	transfer_command(0xc8); //行扫描顺序：从上到下	//c1
	transfer_command(0xa1); //列扫描顺序：从左到右	//a0
	transfer_command(0xda); //sequential configuration
	transfer_command(0x12);
	transfer_command(0x81); //微调对比度,本指令的0x81不要改动，改下面的值
	transfer_command(0xcf); //微调对比度的值，可设置范围0x00～0xff
	transfer_command(0xd9); //Set Pre-Charge Period
	transfer_command(0xf1);
	transfer_command(0xdb); //Set VCOMH Deselect Level
	transfer_command(0x40);
	transfer_command(0xaf); //开显示
}


void lcd_address(u8 page,u8 column)
{
	column=column-1;  							//我们平常所说的第1列，在LCD驱动IC里是第0列。所以在这里减去1.
	page=page-1;
	transfer_command(0xb0+page);   			//设置页地址。每页是8行。一个画面的64行被分成8个页。我们平常所说的第1页，在LCD驱动IC里是第0页，所以在这里减去1
	transfer_command(((column>>4)&0x0f)+0x10);	//设置列地址的高4位
	transfer_command(column&0x0f);				//设置列地址的低4位
}

//全屏清屏
void clear_screen(void)
{
	unsigned char i,j;
 	for(j=0;j<8;j++)
	{
		lcd_address(1+j,1);
		for(i=0;i<128;i++)
		{
			transfer_data(0x00);
		}
	}
}
//full display test
void full_display(u8 data1,u8 data2)
{
	int i,j;
	for(i=0;i<8;i++)
	{
		lcd_address(i+1,1);
		for(j=0;j<64;j++)
		{
			transfer_data(data1);
			transfer_data(data2);    			    
		}
	}          
}
//显示128x64点阵图像
void display_128x64(u8 *dp)
{
	u16 i,j;
	for(j=0;j<8;j++)
	{
		lcd_address(j+1,1);
		for (i=0;i<128;i++)
		{	
			transfer_data(*dp);					//写数据到LCD,每写完一个8位的数据后列地址自动加1
			dp++;
		}
	}
}

//显示128x16点阵图像
void display_128x16(u8 page,u8 column,u8 *dp)
{
	u16 i,j;
	for(j=0;j<2;j++)
	{
		lcd_address(page+j,column);
		for (i=0;i<128;i++)
		{	
			transfer_data(*dp);					//写数据到LCD,每写完一个8位的数据后列地址自动加1
			dp++;
		}
	}
}

//显示32x32点阵图像、汉字、生僻字或32x32点阵的其他图标
void display_graphic_32x32(u8 page,u8 column,const u8 *dp)
{
	u8 i,j;
	for(j=0;j<4;j++)
	{
		lcd_address(page+j,column);
		for (i=0;i<32;i++)
		{	
			transfer_data(*dp);		//写数据到LCD,每写完一个8位的数据后列地址自动加1
			dp++;
		}
	}
}

//显示16x16点阵图像、汉字、生僻字或16x16点阵的其他图标
void display_graphic_16x16(u8 page,u8 column,u8 *dp)
{
	u8 i,j;
	for(j=0;j<2;j++)
	{
		lcd_address(page+j,column);
		for (i=0;i<16;i++)
		{	
			transfer_data(*dp);		//写数据到LCD,每写完一个8位的数据后列地址自动加1
			dp++;
		}
	}
}

//显示8x16点阵图像、汉字、生僻字或8x16点阵的其他图标
void display_graphic_16x8(u8 page,u8 column,u16 *dp)
{
	u8 i,j;
	for(j=0;j<1;j++)
	{
		lcd_address(page+j,column);
		for (i=0;i<16;i++)
		{	
			transfer_data(*dp);		//写数据到LCD,每写完一个8位的数据后列地址自动加1
		dp++;
		}
	}
}
//显示64x64点阵图像、汉字、生僻字或16x16点阵的其他图标
void display_graphic_64x64(u8 page,u8 column,u8 *dp)
{
	u8 i,j;
	for(j=0;j<8;j++)
	{
		lcd_address(page+j,column);
		for (i=0;i<64;i++)
		{	
			transfer_data(*dp);		//写数据到LCD,每写完一个8位的数据后列地址自动加1
			dp++;
		}
	}

}

//显示128x64点阵图像、汉字、生僻字或16x16点阵的其他图标
void display_graphic_128x64(u8 page,u8 column,u8 *dp)
{
	u8 i,j;
	for(j=0;j<8;j++)
	{
		lcd_address(page+j,column);
		for (i=0;i<128;i++)
		{	
			transfer_data(*dp);		//写数据到LCD,每写完一个8位的数据后列地址自动加1
			dp++;
		}
	}
}

//显示8x16点阵图像、ASCII, 或8x16点阵的自造字符、其他图标
void display_graphic_8x16(u8 page,u8 column,u8 *dp)
{
	u8 i,j;
	for(j=0;j<2;j++)
	{
		lcd_address(page+j,column);
		for (i=0;i<8;i++)
		{	
			transfer_data(*dp);					//写数据到LCD,每写完一个8位的数据后列地址自动加1
			dp++;
		}
	}
}

//显示8x16的点阵的字符串，括号里的参数分别为（页,列，字符串指针）
void display_string_8x16(u16 page,u16 column,u8 *text)
{
	u16 i=0,j,k,n;
	if(column>123)
	{
		column=1;
		page+=2;
	}
	while(text[i]>0x00)
	{	
		if((text[i]>=0x20)&&(text[i]<=0x7e))
		{
			j=text[i]-0x20;
			for(n=0;n<2;n++)
			{			
				lcd_address(page+n,column);
				for(k=0;k<8;k++)
				{					
					transfer_data(ascii_table_8x16[j][k+8*n]);	//写数据到LCD,每写完1字节的数据后列地址自动加1
				}
			}
			i++;
			column+=8;
		}
		else
		i++;
	}
}



//显示5x8的点阵的字符串，括号里的参数分别为（页,列，字符串指针）
void display_string_5x8(u16 page,u16 column,u8 reverse,const char *text)
{
	u16 i=0,j,k,disp_data;
	while(text[i]>0x00)
	{	
		if((text[i]>=0x20)&&(text[i]<=0x7e))
		{
			j=text[i]-0x20;
			lcd_address(page,column);
			for(k=0;k<5;k++)
			{
				if(reverse==1)
				{
					disp_data=~ascii_table_5x8[j][k];
				}
				else
				{
					disp_data=ascii_table_5x8[j][k];
				}
				
				transfer_data(disp_data);	//写数据到LCD,每写完1字节的数据后列地址自动加1
			}
			if(reverse==1)	transfer_data(0xff);	//写入一列空白列，使得5x8的字符与字符之间有一列间隔，更美观
			else	transfer_data(0x00);			//写入一列空白列，使得5x8的字符与字符之间有一列间隔，更美观
			i++;
			column+=6;
			if(column>123)
			{
				column=1;
				page++;
			}
		}
		else
		i++;
	}
}


//写入一组16x16点阵的汉字字符串（字符串表格中需含有此字）
//括号里的参数：(页，列，汉字字符串）
void display_string_16x16(u8 page,u8 column,u8 *text)
{
    u8 i,j,k;
    u16 address; 
    
    j = 0;
    while(text[j] != '\0')
    {
        i = 0;
        address = 1;
        while(Chinese_text_16x16[i] > 0x7e)	  // >0x7f即说明不是ASCII码字符
        {
            if(Chinese_text_16x16[i] == text[j])
            {
                if(Chinese_text_16x16[i + 1] == text[j + 1])
                {
                    address = i * 16;
                    break;
                }
            }
            i += 2;            
        }
        
        if(column > 113)
        {
            column = 0;
            page += 2;
        }
        
        if(address != 1)// 显示汉字                   
        {
            
			for(k=0;k<2;k++)
			{
				lcd_address(page+k,column);
	            for(i = 0; i < 16; i++)               
	            {
	                transfer_data(Chinese_code_16x16[address]);   
	                address++;
	            }
   			}
            j += 2;        
        }
        else              //显示空白字符            
        {
			for(k=0;k<2;k++)
			{
				lcd_address(page+k,column);
	            for(i = 0; i < 16; i++)               
	            {
	                transfer_data(0x00);   
	            }
   			} 
            j++;
        }
        column+=16;
    }
}


//显示16x16点阵的汉字或者ASCII码8x16点阵的字符混合字符串
//括号里的参数：(页，列，字符串）
void disp_string_8x16_16x16(u8 page,u8 column,const u8 *text)
{
    u8 temp[3];
    u8 i = 0;    
    
    while(text[i] != '\0')
    {
        if(text[i] > 0x7e)
        {
            temp[0] = text[i];
            temp[1] = text[i + 1];
            temp[2] = '\0';          //汉字为两个字节
            display_string_16x16(page,column,temp);  //显示汉字
            column += 16;
            i += 2;
        }
        else
        {
            temp[0] = text[i];    
            temp[1] = '\0';          //字母占一个字节
            display_string_8x16(page, column, temp);  //显示字母
            column += 8;
            i++;
        }
    }
}
/*显示数字串*/
void display_number_16x8(u8 page,u8 column,u8 number)
{
    unsigned char i=0,n1=0,k=0;
	u8 n[4]={0};
  n[3]= number%10;
  n[2]=(number/10)%10;
  n[1]=(number/100)%10;
  n[0]=(number/1000)%10;
	for(i=0;i<4;i++) 
	{
		n[i]=n[i]+16;
		for(n1=0;n1<2;n1++)
			{			
			lcd_address(page,column+8*i);
			for(k=0;k<8;k++)
				{					
					transfer_data(ascii_table_8x16[n[i]][k]);	//写数据到LCD,每写完1字节的数据后列地址自动加1
				}
			}
	    for(n1=0;n1<2;n1++)
			{			
			lcd_address(page+1,column+8*i);
			for(k=8;k<16;k++)
				{					
					transfer_data(ascii_table_8x16[n[i]][k]);	//写数据到LCD,每写完1字节的数据后列地址自动加1
				}
			}

	}
		
}


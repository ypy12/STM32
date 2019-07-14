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



//дָ�OLED��ʾģ��
void transfer_command(int data1)   
{
     start();
     write_w(0x78);
     write_w(0x00);
     write_w(data1);
     stop();
}

//д���ݵ�OLED��ʾģ��
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
 	
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	 //ʹ��PB�˿�ʱ��

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;	 //PB0,PB1�������  
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//�ٶ�50MHz
 	GPIO_Init(GPIOB, &GPIO_InitStructure);	  //��ʼ��GPIOB0,1
}

//OLED��ʾģ���ʼ��
void initial_olcd(void)
{
  
  	lcd_reset=0;        	//�͵�ƽ��λ
  	delay_ms(500);
  	lcd_reset=1;		   	  //��λ���
	       
	transfer_command(0xae);	//����ʾ
	transfer_command(0xd5);	//����Ƶ��
	transfer_command(0x80);
	transfer_command(0xa8);	//duty����
	transfer_command(0x3f);	//duty=1/64
	transfer_command(0xd3);	//��ʾƫ��
	transfer_command(0x00);	
	transfer_command(0x40);	//��ʼ��
	transfer_command(0x8d);	//��ѹ����
	transfer_command(0x14);	
	transfer_command(0x20);	//page address mode
	transfer_command(0x02);
	transfer_command(0xc8); //��ɨ��˳�򣺴��ϵ���	//c1
	transfer_command(0xa1); //��ɨ��˳�򣺴�����	//a0
	transfer_command(0xda); //sequential configuration
	transfer_command(0x12);
	transfer_command(0x81); //΢���Աȶ�,��ָ���0x81��Ҫ�Ķ����������ֵ
	transfer_command(0xcf); //΢���Աȶȵ�ֵ�������÷�Χ0x00��0xff
	transfer_command(0xd9); //Set Pre-Charge Period
	transfer_command(0xf1);
	transfer_command(0xdb); //Set VCOMH Deselect Level
	transfer_command(0x40);
	transfer_command(0xaf); //����ʾ
}


void lcd_address(u8 page,u8 column)
{
	column=column-1;  							//����ƽ����˵�ĵ�1�У���LCD����IC���ǵ�0�С������������ȥ1.
	page=page-1;
	transfer_command(0xb0+page);   			//����ҳ��ַ��ÿҳ��8�С�һ�������64�б��ֳ�8��ҳ������ƽ����˵�ĵ�1ҳ����LCD����IC���ǵ�0ҳ�������������ȥ1
	transfer_command(((column>>4)&0x0f)+0x10);	//�����е�ַ�ĸ�4λ
	transfer_command(column&0x0f);				//�����е�ַ�ĵ�4λ
}

//ȫ������
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
//��ʾ128x64����ͼ��
void display_128x64(u8 *dp)
{
	u16 i,j;
	for(j=0;j<8;j++)
	{
		lcd_address(j+1,1);
		for (i=0;i<128;i++)
		{	
			transfer_data(*dp);					//д���ݵ�LCD,ÿд��һ��8λ�����ݺ��е�ַ�Զ���1
			dp++;
		}
	}
}

//��ʾ128x16����ͼ��
void display_128x16(u8 page,u8 column,u8 *dp)
{
	u16 i,j;
	for(j=0;j<2;j++)
	{
		lcd_address(page+j,column);
		for (i=0;i<128;i++)
		{	
			transfer_data(*dp);					//д���ݵ�LCD,ÿд��һ��8λ�����ݺ��е�ַ�Զ���1
			dp++;
		}
	}
}

//��ʾ32x32����ͼ�񡢺��֡���Ƨ�ֻ�32x32���������ͼ��
void display_graphic_32x32(u8 page,u8 column,const u8 *dp)
{
	u8 i,j;
	for(j=0;j<4;j++)
	{
		lcd_address(page+j,column);
		for (i=0;i<32;i++)
		{	
			transfer_data(*dp);		//д���ݵ�LCD,ÿд��һ��8λ�����ݺ��е�ַ�Զ���1
			dp++;
		}
	}
}

//��ʾ16x16����ͼ�񡢺��֡���Ƨ�ֻ�16x16���������ͼ��
void display_graphic_16x16(u8 page,u8 column,u8 *dp)
{
	u8 i,j;
	for(j=0;j<2;j++)
	{
		lcd_address(page+j,column);
		for (i=0;i<16;i++)
		{	
			transfer_data(*dp);		//д���ݵ�LCD,ÿд��һ��8λ�����ݺ��е�ַ�Զ���1
			dp++;
		}
	}
}

//��ʾ8x16����ͼ�񡢺��֡���Ƨ�ֻ�8x16���������ͼ��
void display_graphic_16x8(u8 page,u8 column,u16 *dp)
{
	u8 i,j;
	for(j=0;j<1;j++)
	{
		lcd_address(page+j,column);
		for (i=0;i<16;i++)
		{	
			transfer_data(*dp);		//д���ݵ�LCD,ÿд��һ��8λ�����ݺ��е�ַ�Զ���1
		dp++;
		}
	}
}
//��ʾ64x64����ͼ�񡢺��֡���Ƨ�ֻ�16x16���������ͼ��
void display_graphic_64x64(u8 page,u8 column,u8 *dp)
{
	u8 i,j;
	for(j=0;j<8;j++)
	{
		lcd_address(page+j,column);
		for (i=0;i<64;i++)
		{	
			transfer_data(*dp);		//д���ݵ�LCD,ÿд��һ��8λ�����ݺ��е�ַ�Զ���1
			dp++;
		}
	}

}

//��ʾ128x64����ͼ�񡢺��֡���Ƨ�ֻ�16x16���������ͼ��
void display_graphic_128x64(u8 page,u8 column,u8 *dp)
{
	u8 i,j;
	for(j=0;j<8;j++)
	{
		lcd_address(page+j,column);
		for (i=0;i<128;i++)
		{	
			transfer_data(*dp);		//д���ݵ�LCD,ÿд��һ��8λ�����ݺ��е�ַ�Զ���1
			dp++;
		}
	}
}

//��ʾ8x16����ͼ��ASCII, ��8x16����������ַ�������ͼ��
void display_graphic_8x16(u8 page,u8 column,u8 *dp)
{
	u8 i,j;
	for(j=0;j<2;j++)
	{
		lcd_address(page+j,column);
		for (i=0;i<8;i++)
		{	
			transfer_data(*dp);					//д���ݵ�LCD,ÿд��һ��8λ�����ݺ��е�ַ�Զ���1
			dp++;
		}
	}
}

//��ʾ8x16�ĵ�����ַ�����������Ĳ����ֱ�Ϊ��ҳ,�У��ַ���ָ�룩
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
					transfer_data(ascii_table_8x16[j][k+8*n]);	//д���ݵ�LCD,ÿд��1�ֽڵ����ݺ��е�ַ�Զ���1
				}
			}
			i++;
			column+=8;
		}
		else
		i++;
	}
}



//��ʾ5x8�ĵ�����ַ�����������Ĳ����ֱ�Ϊ��ҳ,�У��ַ���ָ�룩
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
				
				transfer_data(disp_data);	//д���ݵ�LCD,ÿд��1�ֽڵ����ݺ��е�ַ�Զ���1
			}
			if(reverse==1)	transfer_data(0xff);	//д��һ�пհ��У�ʹ��5x8���ַ����ַ�֮����һ�м����������
			else	transfer_data(0x00);			//д��һ�пհ��У�ʹ��5x8���ַ����ַ�֮����һ�м����������
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


//д��һ��16x16����ĺ����ַ������ַ���������躬�д��֣�
//������Ĳ�����(ҳ���У������ַ�����
void display_string_16x16(u8 page,u8 column,u8 *text)
{
    u8 i,j,k;
    u16 address; 
    
    j = 0;
    while(text[j] != '\0')
    {
        i = 0;
        address = 1;
        while(Chinese_text_16x16[i] > 0x7e)	  // >0x7f��˵������ASCII���ַ�
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
        
        if(address != 1)// ��ʾ����                   
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
        else              //��ʾ�հ��ַ�            
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


//��ʾ16x16����ĺ��ֻ���ASCII��8x16������ַ�����ַ���
//������Ĳ�����(ҳ���У��ַ�����
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
            temp[2] = '\0';          //����Ϊ�����ֽ�
            display_string_16x16(page,column,temp);  //��ʾ����
            column += 16;
            i += 2;
        }
        else
        {
            temp[0] = text[i];    
            temp[1] = '\0';          //��ĸռһ���ֽ�
            display_string_8x16(page, column, temp);  //��ʾ��ĸ
            column += 8;
            i++;
        }
    }
}
/*��ʾ���ִ�*/
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
					transfer_data(ascii_table_8x16[n[i]][k]);	//д���ݵ�LCD,ÿд��1�ֽڵ����ݺ��е�ַ�Զ���1
				}
			}
	    for(n1=0;n1<2;n1++)
			{			
			lcd_address(page+1,column+8*i);
			for(k=8;k<16;k++)
				{					
					transfer_data(ascii_table_8x16[n[i]][k]);	//д���ݵ�LCD,ÿд��1�ֽڵ����ݺ��е�ַ�Զ���1
				}
			}

	}
		
}


/*****************************
函数名：car.c
描述：  go()前进	
		back()后退
		turn_left()左转
		turn_right()右转
		也可实现PWM调试功能
硬件连接：Pb12-in1,Pb13-in2,Pb14-in3,Pb15-in4
		  PB8-PWMA,PB8-PWMB
库版本：3.5
编写：颢诚科技
*****************************/
#include "car.h"

/*****************************
函数名：Car_Init
描述： 初始化电机的IO端口
输入：
输出：
调用：外部
******************************/
void Car_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	//开启GPIOC时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);//此程序时要禁用JATG，改为SWD模式。注意！！！！！！！！！！！！！！！！！

	//初始化GPIOC端口
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//设置为推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);

	   //此处是将PWM的输出口设置为高电平
//		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
//		 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 ;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//设置为推挽输出
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_Init(GPIOB,&GPIO_InitStructure);
	GPIO_ResetBits(GPIOB,GPIO_Pin_4) ;
	GPIO_ResetBits(GPIOB,GPIO_Pin_5) ;
	GPIO_ResetBits(GPIOB,GPIO_Pin_6) ;
	GPIO_ResetBits(GPIOB,GPIO_Pin_7) ;
	GPIO_ResetBits(GPIOB,GPIO_Pin_3) ;
	go();

}
/*******************************
函数名：go()
描述：前进
调用：外部

*******************************/
void go(void)
{
	in1_set;   //右轮
	in2_clr;
	in3_set;  //左轮
	in4_clr;
}

/*******************************
函数名：back()
描述：后退
调用：外部

*******************************/
void back(void)
{
	in1_clr;	 //右轮
	in2_set;
	in3_clr;	  //左轮
	in4_set;
}

/*******************************
函数名：back()
描述：后退
调用：外部

*******************************/
void car_stop(void)
{
	in1_clr;	 //右轮
	in2_clr;
	in3_clr;	  //左轮
	in4_clr;
}

/*******************************
函数名：turn_right()
描述：右转
调用：外部

*******************************/
void turn_right(void)
{
	in1_set;
	in2_clr;	    //右轮
	in3_clr;
	in4_set;//左轮
}

/*******************************
函数名：turn_left()
描述：左转
调用：外部

*******************************/
void turn_left(void)
{
	in1_clr;
	in2_set;	    //右轮
	in3_set;
	in4_clr;//左轮
}




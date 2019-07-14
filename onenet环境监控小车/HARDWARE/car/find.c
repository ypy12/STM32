/*
文件名:find.c
硬件连接：
			O1 ---- PB2
		   	O2 ---- PB3
			
描述：循迹程序。此程序只针对颢诚科技的2路循迹模块有效。
		另外用此程序时要禁用JATG，改为SWD模式。注意！！！！！！！！！！！！！！！！！
编写：颢诚科技
淘宝网店：http://shop60670850.taobao.com/
*/
#include "find.h"
//#include "PWM.h"
#include "bsp_pwm_output.h"
#include "car.h"
#include "bsp_SysTick.h"

//循迹IO初始化
//要初始化为输入模式
void Find_IO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	//开启GPIO时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);//此程序时要禁用JATG，改为SWD模式。注意！！！！！！！！！！！！！！！！！

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;//选择IO端口
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//配置为推完输入
	GPIO_Init(GPIOB, &GPIO_InitStructure);//根据GPIO_InitStructure中指定的参数初始化外设GPIOD寄存器



}


//循迹、、循迹模块 黑线输出低电平  白线高电平
//小车最左端是O1----最右端是O2
//循迹路面：白色路面黑色引导线，即寻黑线。
//黑线 传感器输出0，白线输出1
//PWM PB8 ---PWM_ch3右轮  PB9 ---PWM_ch4 左轮
void Find(void)
{
	if((Find_O1 == 1) && (Find_O2 == 1))// 前进
	{
			PWM_Init(1000,0,700,700);//PWM  频率= 72M/1000 =72KHz  右轮占空比 = 500/1000；左轮占空比 =  500/1000
			go();
	}

	//O3 寻到黑线  前进
	if((Find_O1 == 0) && (Find_O2 == 0) )
	{
			//PWM_Init(1000,0,800,800);//PWM  频率= 72M/1000 =72KHz  右轮占空比 = 500/1000；左轮占空比 =  500/1000
			stop();
	}
	//O1寻到黑线 说明小车偏向右行驶了 应向左转调整到前进方向	。即右轮加速左轮减速
	if((Find_O1 == 0) && (Find_O2 == 1))
	{
			PWM_Init(1000,0,0,700);//PWM  频率= 72M/1000 =72KHz  右轮占空比 = 500/1000；左轮占空比 =  500/1000
			go();
	}

	//O2寻到黑线 说明小车偏向左行驶了 应向右转调整到前进方向	。即左轮加速右轮减速
	if((Find_O1 == 1) && (Find_O2 == 0))
	{
			PWM_Init(1000,0,700,0);//PWM  频率= 72M/1000 =72KHz  右轮占空比 = 500/1000；左轮占空比 =  500/1000
			go();
	}
	

	
	
}













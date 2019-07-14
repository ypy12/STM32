#include "lsens.h"
#include "delay.h"

 
//初始化光敏传感器
void Lsens_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
	ADC_InitTypeDef   ADC_InitStructure;  
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC3, ENABLE);	
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF,ENABLE);//使能PORTF时钟	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;//PF8 anolog输入
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		//模拟输入引脚
	GPIO_Init(GPIOF, &GPIO_InitStructure);	
	
  RCC_ADCCLKConfig(RCC_PCLK2_Div4);                                                       
  ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;                        
  ADC_InitStructure.ADC_ScanConvMode = ENABLE;                                 
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;                        
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;        
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;                
  ADC_InitStructure.ADC_NbrOfChannel = 1;                                                
  ADC_Init(ADC3, &ADC_InitStructure);                                                 
  ADC_RegularChannelConfig(ADC3, ADC_Channel_6, 1, ADC_SampleTime_55Cycles5);          
  ADC_Cmd(ADC3, ENABLE);                                                                                
  ADC_ResetCalibration(ADC3);                                                                 
  while(ADC_GetResetCalibrationStatus(ADC3));                                        
  ADC_StartCalibration(ADC3);                                                                       
  while(ADC_GetCalibrationStatus(ADC3));                                               
  ADC_SoftwareStartConvCmd(ADC3, ENABLE);                                        
}

u16 Get_Adc3(u8 ch)   
{
  //设置指定ADC的规则组通道，一个序列，采样时间
	ADC_RegularChannelConfig(ADC3, ch, 1, ADC_SampleTime_239Cycles5 );	//ADC3,ADC通道,采样时间为239.5周期	  			    
  
	ADC_SoftwareStartConvCmd(ADC3, ENABLE);		//使能指定的ADC3的软件转换启动功能	
	 
	while(!ADC_GetFlagStatus(ADC3, ADC_FLAG_EOC ));//等待转换结束

	return ADC_GetConversionValue(ADC3);	//返回最近一次ADC3规则组的转换结果
} 
//读取Light Sens的值
//0~100:0,最暗;100,最亮 
u8 Lsens_Get_Val(void)
{
	u32 temp_val=0;
	u8 t;
	for(t=0;t<LSENS_READ_TIMES;t++)
	{
		temp_val+=Get_Adc3(LSENS_ADC_CHX);	//读取ADC值
		delay_ms(5);
	}
	temp_val/=LSENS_READ_TIMES;//得到平均值 
	if(temp_val>4000)temp_val=4000;
	return (u8)(100-4*(temp_val/40));
  
}













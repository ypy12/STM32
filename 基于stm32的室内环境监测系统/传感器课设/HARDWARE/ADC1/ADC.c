#include "stm32f10x.h"
 #include "delay.h"                              
void ADC1_Config()
{
    GPIO_InitTypeDef GPIO_InitStructure;  
    ADC_InitTypeDef   ADC_InitStructure;  
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);	
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AIN; 
    GPIO_Init(GPIOB,&GPIO_InitStructure); 
    RCC_ADCCLKConfig(RCC_PCLK2_Div4);                                                       
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;                        
    ADC_InitStructure.ADC_ScanConvMode = ENABLE;                                 
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;                        
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;        
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;                
    ADC_InitStructure.ADC_NbrOfChannel = 1;                                                
    ADC_Init(ADC1, &ADC_InitStructure);                                                 
    ADC_RegularChannelConfig(ADC1, ADC_Channel_8, 1, ADC_SampleTime_55Cycles5);          
    ADC_Cmd(ADC1, ENABLE);                                                                                
    ADC_ResetCalibration(ADC1);                                                                 
    while(ADC_GetResetCalibrationStatus(ADC1));                                        
    ADC_StartCalibration(ADC1);                                                                       
    while(ADC_GetCalibrationStatus(ADC1));                                               
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);                                        
}
u8 sound_Get_Val(void)
{
	u32 temp_val=0;
	u8 t;
	u32 sound_READ_TIMES=10;
	for(t=0;t<sound_READ_TIMES;t++)
	{
		temp_val+=ADC_GetConversionValue(ADC1);;	//读取ADC值
		delay_ms(5);
	}
	temp_val/=sound_READ_TIMES;//得到平均值 
	return (u8)(140-(140*temp_val/4095));
}

      

               
              

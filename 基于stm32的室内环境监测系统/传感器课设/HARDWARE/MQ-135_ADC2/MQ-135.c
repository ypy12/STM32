#include "stm32f10x.h"
#include "delay.h"
#include "MQ-135_ADC2.h"
void ADC2_Config()
{
    GPIO_InitTypeDef GPIO_InitStructure;  
    ADC_InitTypeDef   ADC_InitStructure;  
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC2, ENABLE);	
	  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AIN; 
    GPIO_Init(GPIOC,&GPIO_InitStructure); 
    RCC_ADCCLKConfig(RCC_PCLK2_Div4);                                                       
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;                        
    ADC_InitStructure.ADC_ScanConvMode = ENABLE;                                 
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;                        
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;        
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;                
    ADC_InitStructure.ADC_NbrOfChannel = 1;                                                
    ADC_Init(ADC2, &ADC_InitStructure);                                                 
    ADC_RegularChannelConfig(ADC2, ADC_Channel_10, 1, ADC_SampleTime_55Cycles5);          
    ADC_Cmd(ADC2, ENABLE);                                                                                
    ADC_ResetCalibration(ADC2);                                                                 
    while(ADC_GetResetCalibrationStatus(ADC2));                                        
    ADC_StartCalibration(ADC2);                                                                       
    while(ADC_GetCalibrationStatus(ADC2));                                               
    ADC_SoftwareStartConvCmd(ADC2, ENABLE);                                        


}

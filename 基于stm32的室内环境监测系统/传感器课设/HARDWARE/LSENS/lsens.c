#include "lsens.h"
#include "delay.h"

 
//��ʼ������������
void Lsens_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
	ADC_InitTypeDef   ADC_InitStructure;  
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC3, ENABLE);	
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF,ENABLE);//ʹ��PORTFʱ��	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;//PF8 anolog����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		//ģ����������
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
  //����ָ��ADC�Ĺ�����ͨ����һ�����У�����ʱ��
	ADC_RegularChannelConfig(ADC3, ch, 1, ADC_SampleTime_239Cycles5 );	//ADC3,ADCͨ��,����ʱ��Ϊ239.5����	  			    
  
	ADC_SoftwareStartConvCmd(ADC3, ENABLE);		//ʹ��ָ����ADC3�����ת����������	
	 
	while(!ADC_GetFlagStatus(ADC3, ADC_FLAG_EOC ));//�ȴ�ת������

	return ADC_GetConversionValue(ADC3);	//�������һ��ADC3�������ת�����
} 
//��ȡLight Sens��ֵ
//0~100:0,�;100,���� 
u8 Lsens_Get_Val(void)
{
	u32 temp_val=0;
	u8 t;
	for(t=0;t<LSENS_READ_TIMES;t++)
	{
		temp_val+=Get_Adc3(LSENS_ADC_CHX);	//��ȡADCֵ
		delay_ms(5);
	}
	temp_val/=LSENS_READ_TIMES;//�õ�ƽ��ֵ 
	if(temp_val>4000)temp_val=4000;
	return (u8)(100-4*(temp_val/40));
  
}













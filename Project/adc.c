#include "main.h"


void set_adc (void)
{

ADC_InitTypeDef sADCInit;

  ADCx_InitTypeDef ADCInit;
//	  ADCx_InitTypeDef sADCx;
  ADCInit.ADC_ChannelNumber = ADC_CH_ADC2;
  ADCInit.ADC_Channels = 0;
  ADCInit.ADC_ChannelSwitching = ADC_CH_SWITCHING_Disable;
  ADCInit.ADC_ClockSource = ADC_CLOCK_SOURCE_ADC;
  ADCInit.ADC_DelayGo = 0;
  ADCInit.ADC_LevelControl = ADC_LEVEL_CONTROL_Disable ;
  ADCInit.ADC_HighLevel = 0;
  ADCInit.ADC_LowLevel = 0;
  ADCInit.ADC_IntVRefSource = ADC_INT_VREF_SOURCE_INEXACT;

  ADCInit.ADC_Prescaler = 0;
  ADCInit.ADC_SamplingMode = ADC_SAMPLING_MODE_SINGLE_CONV;
  ADCInit.ADC_VRefSource = ADC_VREF_SOURCE_INTERNAL;

	ADC1_Init(&ADCInit);
	ADC_StructInit(&sADCInit);
  ADC1_ITConfig	(ADC1_IT_END_OF_CONVERSION, ENABLE);
	NVIC_EnableIRQ(ADC_IRQn);
	ADC1_Cmd(ENABLE);


}
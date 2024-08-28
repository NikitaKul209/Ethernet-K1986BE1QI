#include "main.h"
#include "MDR32F9Qx_adc.h"

void set_adc (void)
{



  ADCx_InitTypeDef ADCInit;
  ADCInit.ADC_ChannelNumber = ADC_CH_ADC0;
  ADCInit.ADC_Channels = 0;
  ADCInit.ADC_ChannelSwitching = ADC_CH_SWITCHING_Disable;
  ADCInit.ADC_ClockSource = ADC_CLOCK_SOURCE_ADC;
  ADCInit.ADC_DelayGo = 0;
  ADCInit.ADC_LevelControl = ADC_LEVEL_CONTROL_Enable ;
  ADCInit.ADC_HighLevel = 0;
  ADCInit.ADC_LowLevel = 0;
  ADCInit.ADC_IntVRefSource = ADC_INT_VREF_SOURCE_INEXACT;

  ADCInit.ADC_Prescaler = ADC_CLK_div_None;
  ADCInit.ADC_SamplingMode = ADC_SAMPLING_MODE_SINGLE_CONV;
  ADCInit.ADC_VRefSource = ADC_VREF_SOURCE_EXTERNAL;

  ADC1_Init(&ADCInit);
  ADC1_ITConfig	(ADC1_IT_END_OF_CONVERSION, ENABLE);




}
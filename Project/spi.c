

#include "main.h"



static SSP_InitTypeDef SPI1;
void set_spi(void)
{
  SSP_DeInit(MDR_SSP1);
  SSP_StructInit(&SPI1);

  /* Initialize the SSP max speed */
  SPI1.SSP_CPSDVSR = 2;
  SPI1.SSP_SCR = 1;
  /* Initialize the SPI_Mode member */
  SPI1.SSP_Mode = SSP_ModeMaster;
  /* initialize the SSP_WordLength member */
  SPI1.SSP_WordLength = SSP_WordLength16b;
  /* Initialize the SSP_SPH member */
  SPI1.SSP_SPH = SSP_SPH_1Edge;
  /* Initialize the SSP_SPO member */
  SPI1.SSP_SPO = SSP_SPO_Low;
  /* Initialize the SSP_FRF member */
  SPI1.SSP_FRF = SSP_FRF_SPI_Motorola;
  /* Initialize the hardware flow control */
//  SPI1.SSP_HardwareFlowControl = SSP_HardwareFlowControl_SSE;
  SPI1.SSP_HardwareFlowControl=SSP_HardwareFlowControl_None;
  SSP_Init(MDR_SSP1, &SPI1);

  SSP_Cmd(MDR_SSP1, ENABLE);
}


//uint16_t spi_transfer(MDR_SSP_TypeDef *SSPx, uint8_t data)
//{
//	CONV_HIGH
//__NOP();
//	for (int i = 0; i<3; i++){
//		__NOP();
//	}
//	CONV_LOW
//  SSP_SendData(SSPx, data);
//	SSP_SendData(SSPx, data);
//  while ((SSP_GetFlagStatus(SSPx, SSP_FLAG_RNE) != SET)  ||	(SSP_GetFlagStatus(SSPx, SSP_FLAG_BSY)));

//  return SSP_ReceiveData(SSPx);
//}


uint16_t spi_transfer2(MDR_SSP_TypeDef *SSPx, uint16_t data)
{
  uint16_t adc_val;
 
  CONV_HIGH    
  __NOP();__NOP();  __NOP();  __NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
  CONV_LOW
  SSP_SendData(SSPx, data);
  while ((SSP_GetFlagStatus(SSPx, SSP_FLAG_RNE) != SET)  ||	(SSP_GetFlagStatus(SSPx, SSP_FLAG_BSY)));
//  SSP_SendData(SSPx, data);
//  while ((SSP_GetFlagStatus(SSPx, SSP_FLAG_RNE) != SET)  ||	(SSP_GetFlagStatus(SSPx, SSP_FLAG_BSY)));
  adc_val= SSP_ReceiveData(SSPx);

  return adc_val;
}


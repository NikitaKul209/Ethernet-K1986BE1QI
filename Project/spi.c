

#include "main.h"



static SSP_InitTypeDef SPI1;
void set_spi(void)
{
  SSP_DeInit(MDR_SSP1);
  SSP_StructInit(&SPI1);

  /* Initialize the SSP max speed */
  SPI1.SSP_CPSDVSR = 2;
  SPI1.SSP_SCR = 0;
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
	
//	SSP_ITConfig(MDR_SSP1,SSP_IT_RX,ENABLE);
//	 NVIC_EnableIRQ(SSP1_IRQn);
	
  SSP_Init(MDR_SSP1, &SPI1);
	
  SSP_Cmd(MDR_SSP1, ENABLE);
}

uint16_t spi_transfer(MDR_SSP_TypeDef *SSPx, uint16_t data,uint16_t counter)
{
 

  CONV_HIGH    

		for (int i = 0; i<6;i++){
	__NOP();	__NOP();
	}
//	__NOP();		__NOP();		__NOP();	__NOP();
 		  CONV_LOW
	__NOP();	__NOP();	



//  SSP_SendData(SSPx, data);
  SSPx->DR = data;

			for (int i = 0; i<6;i++){
	__NOP();	__NOP();
	}
//			PORT_SetBits(MDR_PORTE,PORT_Pin_7);
//	for (int i = 0; i<8;i++){__NOP();}
// while ((SSP_GetFlagStatus(SSPx, SSP_FLAG_BSY)));
//while ( (SSPx->SR & ((uint32_t)SSP_FLAG_BSY)) );
//	 while ((SSP_GetFlagStatus(SSPx, SSP_FLAG_RNE) != SET)  ||	(SSP_GetFlagStatus(SSPx, SSP_FLAG_BSY)));

//	 adc_val= SSP_ReceiveData(SSPx);
//for (int i = 0; i<8;i++){__NOP();}

  return  SSPx->DR;;
}

void spi_write_command_reg(void){
	
CONV_HIGH    
	for (int i = 0; i<2;i++){
	__NOP();	__NOP();
	}
CONV_LOW
 MDR_SSP1->DR = 0x14E3;
		 while ((SSP_GetFlagStatus(MDR_SSP1, SSP_FLAG_RNE) != SET)  ||	(SSP_GetFlagStatus(MDR_SSP1, SSP_FLAG_BSY)));
SSP_ReceiveData(MDR_SSP1);
	
}

uint16_t spi_read_command_reg(void){

CONV_HIGH    
	for (int i = 0; i<2;i++){
	__NOP();	
	}
CONV_LOW
SSP_SendData(MDR_SSP1, 0x54ff);

	 while ((SSP_GetFlagStatus(MDR_SSP1, SSP_FLAG_RNE) != SET)  ||	(SSP_GetFlagStatus(MDR_SSP1, SSP_FLAG_BSY)));
return  MDR_SSP1->DR;; 
}

//void SSP_IRQHandler(void){
//if(SSP_GetITStatusMasked(MDR_SSP1,SSP_IT_RX)){



//}
//	

//}


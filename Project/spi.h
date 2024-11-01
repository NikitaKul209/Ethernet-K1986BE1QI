#ifndef SPI
#define SPI

#include "main.h"


//#define CONV_LOW   PORT_ResetBits(MDR_PORTE,PORT_Pin_15);
//#define CONV_HIGH  PORT_SetBits(MDR_PORTE,PORT_Pin_15);

#define CONV_HIGH MDR_PORTE->SETTX = PORT_Pin_15;
#define CONV_LOW  MDR_PORTE->CLRTX = PORT_Pin_15;
void set_spi(void);
//uint16_t spi_transfer(MDR_SSP_TypeDef *SSPx, uint16_t data);

uint16_t spi_transfer(MDR_SSP_TypeDef *SSPx, uint16_t data,uint16_t counter);
uint16_t spi_read_command_reg(void);
void spi_write_command_reg(void);
#endif
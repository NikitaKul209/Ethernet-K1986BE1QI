#include "clk.h"

void set_clk(void)
{



  RST_CLK_HSEconfig(RST_CLK_HSE_ON);
  if (RST_CLK_HSEstatus() !=  SUCCESS)
    {
      while(1) {};

    }

  RST_CLK_HSE2config(RST_CLK_HSE2_ON);
  if (RST_CLK_HSE2status() !=  SUCCESS)
    {
      while(1);
    };

  RST_CLK_CPU_PLLcmd(ENABLE);
  if	(RST_CLK_CPU_PLLstatus() != SUCCESS)
    {
      while(1);
    };
  RST_CLK_CPU_PLLconfig(RST_CLK_CPU_PLLsrcHSEdiv1, RST_CLK_CPU_PLLmul5 );
  RST_CLK_CPU_PLLuse(ENABLE);

  RST_CLK_CPUclkSelection(RST_CLK_CPUclkCPU_C3);
//RST_CLK_PCLKcmd(RST_CLK_PCLK_DMA,ENABLE);
  RST_CLK_PCLKcmd(RST_CLK_PCLK_PORTA, ENABLE);
  RST_CLK_PCLKcmd(RST_CLK_PCLK_PORTC, ENABLE);

//RST_CLK_PCLKcmd(RST_CLK_PCLK_UART1, ENABLE);
//RST_CLK_PCLKcmd(RST_CLK_PCLK_UART2, ENABLE);

  RST_CLK_PCLKcmd(RST_CLK_PCLK_TIMER1, ENABLE);
  RST_CLK_PCLKcmd(RST_CLK_PCLK_TIMER2, ENABLE);

//UART_BRGInit(MDR_UART1, UART_HCLKdiv1);
//UART_BRGInit(MDR_UART2, UART_HCLKdiv1);

  TIMER_BRGInit(MDR_TIMER1, TIMER_HCLKdiv128);
  TIMER_BRGInit(MDR_TIMER2, TIMER_HCLKdiv128);
	
		
	RST_CLK_ADCclkSelection()
  /* Enables the RST_CLK_PCLK_EEPROM */
//	RST_CLK_PCLKcmd(RST_CLK_PCLK_EEPROM, ENABLE);

  /* Sets the code latency value */

//ETH_ClockDeInit();
//ETH_PHY_ClockConfig(ETH_PHY_CLOCK_SOURCE_HSE2 ,ETH_PHY_HCLKdiv1 );
//ETH_BRGInit(ETH_HCLKdiv1);
//ETH_ClockCMD(ETH_CLK1,ENABLE);


}
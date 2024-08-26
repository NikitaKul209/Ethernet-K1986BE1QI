#include "port.h"

void set_port(void)
{


  PORT_InitTypeDef PortInit;

  PortInit.PORT_PULL_UP = PORT_PULL_UP_OFF;
  PortInit.PORT_PULL_DOWN = PORT_PULL_DOWN_OFF;
  PortInit.PORT_PD_SHM = PORT_PD_SHM_OFF;
  PortInit.PORT_PD = PORT_PD_DRIVER;
  PortInit.PORT_GFEN = PORT_GFEN_OFF;
  PortInit.PORT_SPEED = PORT_SPEED_MAXFAST;
  PortInit.PORT_MODE = PORT_MODE_DIGITAL;

  PortInit.PORT_FUNC = PORT_FUNC_PORT;
  PortInit.PORT_OE = PORT_OE_OUT;
  PortInit.PORT_Pin = PORT_Pin_9;
  PORT_Init(MDR_PORTC, &PortInit);

//	PortInit.PORT_Pin = PORT_Pin_13;
//	PORT_Init(MDR_PORTA, &PortInit);

  PortInit.PORT_Pin = PORT_Pin_12;
  PORT_Init(MDR_PORTD, &PortInit);




}
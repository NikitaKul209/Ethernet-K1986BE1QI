#ifndef ETH
#define ETH

//F8-CC-6E-01-2D-A0

#include "main.h"

#define UDP_SOURCE_PORT 8070

//extern uint8_t DA_MAC_Address[6] ;
//extern uint8_t SA_MAC_Address[6] ;
//extern uint8_t DA_IP_Address[4];
//extern uint8_t SA_IP_Address[4];
void set_ethernet(void);
void ethernet_PHY_Status(void);

#endif
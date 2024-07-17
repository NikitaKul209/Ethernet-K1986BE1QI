#ifndef ETH
#define ETH

//F8-CC-6E-01-2D-A0

#include "main.h"

extern uint8_t DA_MAC_Address[6] ;
extern uint8_t SA_MAC_Address[6] ;
extern uint8_t DA_IP_Address[6]; 
extern uint8_t SA_IP_Address[6];
void set_ethernet(void);

#endif
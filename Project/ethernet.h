#ifndef ETH
#define ETH

//F8-CC-6E-01-2D-A0

#include "main.h"

#define MYIP_1               192                 // our internet protocol (IP) address
#define MYIP_2               168
#define MYIP_3               8
#define MYIP_4               19

#define SUBMASK_1            255                 // subnet mask
#define SUBMASK_2            255
#define SUBMASK_3            255
#define SUBMASK_4            0

#define GWIP_1               192                 // standard gateway (used if remote
#define GWIP_2               168                 // IP is no part of our subnet)
#define GWIP_3               8
#define GWIP_4               1

#define MAC_5               0xAB
#define MAC_4               0x87
#define MAC_3               0x65
#define MAC_2               0x43
#define MAC_1               0x21
#define MAC_0               0x00


extern uint8_t DA_MAC_Address[6] ;
extern uint8_t SA_MAC_Address[6] ;
extern uint8_t DA_IP_Address[4];
extern uint8_t SA_IP_Address[4];
void set_ethernet(void);
void set_ethernet2(void);

#endif
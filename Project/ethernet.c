#include "ethernet.h"
#include "main.h"


uint8_t DA_MAC_Address[6] = {0x58,0xD5,0x6E,0x3E,0x11,0xC8};
uint8_t SA_MAC_Address[6] = {0x34,0x13,0x78,0x56,0xbc,0x9a};
//uint8_t SA_MAC_Address[6] = {0x,0x13,0x78,0x56,0xbc,0x9a};
uint8_t SA_IP_Address[4] = { 169,254,254,172};
uint8_t DA_IP_Address[4] = { 169,254,254,1};

#define ETH_RAM_BASE_ADDR	(unsigned int)0x38000000
// ïîëíûé ðàçìåð ÎÇÓ ethernet
#define ETH_RAM_FULL_SIZE	8192	// â áàéòàõ
void set_ethernet2(void)
{

//  ETH_DeInit(MDR_ETHERNET1);

//  ETH_InitTypeDef  EthernetInit;
//  EthernetInit.ETH_Transmitter_State = ENABLE;                                              // разрешение работы передатчика (бит EN);
//  EthernetInit.ETH_Transmitter_BE =  ETH_TRANSMITTER_BE_BIG_ENDIAN;                         // порядок следования байт в буфере (бит BE);
//  EthernetInit.ETH_Transmitter_Bits_Order = ETH_TRANSMITTER_BITS_ORDER_LSB;                 // порядок следования бит в байте (бит MSB1st);
//  EthernetInit.ETH_Transmitter_Event_Mode = ETH_TRANSMITTER_EVENT_MODE_XFIFO_ALMOST_EMPTY;  // выбор события при передаче, выводимого на вывод EVNT[1] (поле EVNT_MODE);
//  EthernetInit.ETH_Automatic_Pad_Strip = ENABLE;                                            // управление дополнением пакета до минимальной длины PAD-ами (бит PAD_EN);
//  EthernetInit.ETH_Automatic_Preamble = ENABLE;                                             // управление дополнением пакета преамбулой (бит PRE_EN), SFD добавляется в любом случае;
//  EthernetInit.ETH_Automatic_CRC_Strip = ENABLE;                                            // управление дополнением пакета, автоматически подсчитываемым полем CRC (CRC_EN);
//  EthernetInit.ETH_Automatic_IPG = ENABLE;                                                  // управление интервалом между отправлением пакетов (бит IPG_EN);
//  EthernetInit.ETH_Retry_Counter = 0x0A;                                                    // управление максимальным числом повторений (поле RtryCnt).



//  EthernetInit.ETH_PHY_Address = 0x1C;
//  /* PHY Auto-negotiation enabled */
//  EthernetInit.ETH_PHY_Mode = ETH_PHY_MODE_AutoNegotiation;
//  /* Set the working interface */
//  EthernetInit.ETH_PHY_Interface = ETH_PHY_INTERFACE_ETHERNET_802_3;

//  //	MAC General Config
//  EthernetInit.ETH_Dilimiter = 0x1000;
////EthernetInit.ETH_DBG_Mode
////EthernetInit.ETH_DBG_XF
////EthernetInit.ETH_DBG_RF
////EthernetInit.ETH_Loopback_Mode
//  EthernetInit.ETH_Receiver_RST = SET;
//  EthernetInit.ETH_Transmitter_RST = SET;


////EthernetInit.ETH_Register_CLR

//  EthernetInit.ETH_Buffer_Mode = ETH_BUFFER_MODE_LINEAR ;
////EthernetInit.ETH_Buffer_Mode =   ETH_BUFFER_MODE_AUTOMATIC_CHANGE_POINTERS ;
////EthernetInit.ETH_Buffer_Mode =   ETH_BUFFER_MODE_AUTOMATIC_CHANGE_POINTERS ;



////EthernetInit.ETH_Extension_Mode
////EthernetInit.ETH_HalfDuplex_Mode
////EthernetInit.ETH_DTRM
////EthernetInit.ETH_Pause
////EthernetInit.ETH_ColWnd

//  EthernetInit.ETH_Source_Addr_HASH_Filter = DISABLE;

//  EthernetInit.ETH_MAC_Address[0] = (SA_MAC_Address[1] | SA_MAC_Address[0]<<8) ;
//  EthernetInit.ETH_MAC_Address[1] = (SA_MAC_Address[3] | SA_MAC_Address[2]<<8);
//  EthernetInit.ETH_MAC_Address[2] = (SA_MAC_Address[5] | SA_MAC_Address[4]<<8);



//  ETH_StructInit(&EthernetInit);
//  ETH_Init(MDR_ETHERNET1,&EthernetInit);

//  ETH_MACTransmissionCmd(MDR_ETHERNET1, ENABLE);
//  ETH_MACReceptionCmd(MDR_ETHERNET1, ENABLE);

//  NVIC_ClearPendingIRQ(ETHERNET_IRQn);
//  NVIC_EnableIRQ(ETHERNET_IRQn);
//  ETH_PHYCmd(MDR_ETHERNET1, ENABLE);
//  ETH_Start(MDR_ETHERNET1);

    ETH_ClockDeInit();
    RST_CLK_HSE2config(RST_CLK_HSE2_ON);
	if(RST_CLK_HSE2status() == ERROR)
    	while(1);
	ETH_PHY_ClockConfig(ETH_PHY_CLOCK_SOURCE_HSE2, ETH_PHY_HCLKdiv1);

	ETH_BRGInit(ETH_HCLKdiv1);
	ETH_ClockCMD(ETH_CLK1, ENABLE);

	ETH_InitTypeDef ETH_InitStruct;
	ETH_DeInit(MDR_ETHERNET1);
	ETH_StructInit((ETH_InitTypeDef *) &ETH_InitStruct);
	ETH_InitStruct.ETH_PHY_Mode = ETH_PHY_MODE_AutoNegotiation;
	ETH_InitStruct.ETH_Transmitter_RST = SET;
	ETH_InitStruct.ETH_Receiver_RST = SET;
	ETH_InitStruct.ETH_Buffer_Mode = ETH_BUFFER_MODE_LINEAR; //NOTE: Only linear mode supported.
	ETH_InitStruct.ETH_Source_Addr_HASH_Filter = DISABLE;
	ETH_InitStruct.ETH_MAC_Address[2] = (SA_MAC_Address[5]<<8 | SA_MAC_Address[4]) ;
	ETH_InitStruct.ETH_MAC_Address[1] = (SA_MAC_Address[3]<<8 | SA_MAC_Address[2]);
	ETH_InitStruct.ETH_MAC_Address[0] = (SA_MAC_Address[1]<<8 | SA_MAC_Address[0]);
	ETH_InitStruct.ETH_Dilimiter = 0x1000;
	ETH_Init(MDR_ETHERNET1, (ETH_InitTypeDef *) &ETH_InitStruct);
	ETH_PHYCmd(MDR_ETHERNET1, ENABLE);
	ETH_Start(MDR_ETHERNET1);


}

void set_ethernet(void)
{
  static ETH_InitTypeDef initStruct;

  // Reset ethernet clock settings
  ETH_ClockDeInit();

//	RST_CLK_PCLKcmd(RST_CLK_PCLK_DMA, ENABLE);

  // Enable HSE2 oscillator 25 MHz
  RST_CLK_HSE2config(RST_CLK_HSE2_ON);
  if(RST_CLK_HSE2status() == ERROR)
    while(1);
  // Config PHY clock
  ETH_PHY_ClockConfig(ETH_PHY_CLOCK_SOURCE_HSE2, ETH_PHY_HCLKdiv1);
	  ETH_PHYCmd(MDR_ETHERNET1, ENABLE);
//memset((unsigned int*)ETH_RAM_BASE_ADDR, 0, ETH_RAM_FULL_SIZE * sizeof(unsigned char));
  ETH_BRGInit(ETH_HCLKdiv1);
  ETH_ClockCMD(ETH_CLK1, ENABLE);
  ETH_DeInit(MDR_ETHERNET1);

  ETH_StructInit(&initStruct );
//	initStruct.ETH_PHY_Mode                 = ETH_PHY_MODE_AutoNegotiation;
  initStruct.ETH_PHY_Mode  = ETH_PHY_MODE_100BaseT_Full_Duplex ;
  initStruct.ETH_Transmitter_RST          = RESET;
  initStruct.ETH_Receiver_RST             = RESET;
  initStruct.ETH_Buffer_Mode              = ETH_BUFFER_MODE_LINEAR;
  initStruct.ETH_Source_Addr_HASH_Filter  = DISABLE;
  initStruct.ETH_Receive_All_Packets = ENABLE;
  initStruct.ETH_Unicast_Frames_Reception = ENABLE;
  initStruct.ETH_Dilimiter = 0x1000;
  // ERRATA 0019
//  initStruct.ETH_Control_Frames_Reception = ENABLE;
  // Set the MAC address (see: tcpip.h).

//	  initStruct.ETH_Transmitter_Bits_Order = ETH_TRANSMITTER_BITS_ORDER_MSB;
  initStruct.ETH_MAC_Address[2] = (SA_MAC_Address[1] | SA_MAC_Address[0]<<8) ;
  initStruct.ETH_MAC_Address[1] = (SA_MAC_Address[3] | SA_MAC_Address[2]<<8);
  initStruct.ETH_MAC_Address[0] = (SA_MAC_Address[5] | SA_MAC_Address[4]<<8);

//	initStruct.ETH_DBG_RF = DISABLE;
//	initStruct.ETH_DBG_XF = DISABLE;
//	initStruct.ETH_Register_CLR = DISABLE;
//	initStruct.ETH_IPG = 0x0060;
//	initStruct.ETH_PSC = 0x50;
//	initStruct.ETH_BAG = 0x200;
//	initStruct.ETH_JitterWnd = 0x5;
//	MDR_ETHERNET1->ETH_R_CFG |=1<<3;
//
////
//initStruct.ETH_Transmitter_BE =  ETH_TRANSMITTER_BE_LITTLE_ENDIAN;
//initStruct.ETH_Receiver_BE =  ETH_RECEIVER_BE_BIG_ENDIAN;
//initStruct.ETH_Receiver_Bits_Order = ETH_RECEIVER_BITS_ORDER_LSB;
//initStruct.ETH_Transmitter_Bits_Order = ETH_TRANSMITTER_BITS_ORDER_LSB;

  ETH_Init(MDR_ETHERNET1, &initStruct);

  ETH_MACITConfig(MDR_ETHERNET1, ETH_MAC_IT_RF_OK, ENABLE);
  // TCP/IP library init
//	TCPLowLevelInit();

  // IRQ Handler init at RX.

//  NVIC_ClearPendingIRQ(ETHERNET_IRQn);
  ETH_Start(MDR_ETHERNET1);

  NVIC_EnableIRQ(ETHERNET_IRQn);
  delay(100000);
}
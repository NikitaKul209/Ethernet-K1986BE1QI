#include "ethernet.h"
#include "main.h"


uint8_t DA_MAC_Address[6] = {0x58,0xD5,0x6E,0x3E,0x11,0xC8};
uint8_t SA_MAC_Address[6] = {0x0,0x34,0x56,0x78,0x9a,0xbc};
uint8_t SA_IP_Address[6] = { 192,168,8,10};
uint8_t DA_IP_Address[6] = { 192,168,8,1};


void set_ethernet(){
	
//ETH_DeInit(MDR_ETHERNET1);	
//	
//ETH_InitTypeDef  EthernetInit;
//EthernetInit.ETH_Transmitter_State = ENABLE;                                              // разрешение работы передатчика (бит EN);
//EthernetInit.ETH_Transmitter_BE =  ETH_TRANSMITTER_BE_BIG_ENDIAN;                         // порядок следования байт в буфере (бит BE);
//EthernetInit.ETH_Transmitter_Bits_Order = ETH_TRANSMITTER_BITS_ORDER_LSB;                 // порядок следования бит в байте (бит MSB1st);
//EthernetInit.ETH_Transmitter_Event_Mode = ETH_TRANSMITTER_EVENT_MODE_XFIFO_ALMOST_EMPTY;  // выбор события при передаче, выводимого на вывод EVNT[1] (поле EVNT_MODE);
//EthernetInit.ETH_Automatic_Pad_Strip = ENABLE;                                            // управление дополнением пакета до минимальной длины PAD-ами (бит PAD_EN);
//EthernetInit.ETH_Automatic_Preamble = ENABLE;                                             // управление дополнением пакета преамбулой (бит PRE_EN), SFD добавляется в любом случае;
//EthernetInit.ETH_Automatic_CRC_Strip = ENABLE;                                            // управление дополнением пакета, автоматически подсчитываемым полем CRC (CRC_EN);
//EthernetInit.ETH_Automatic_IPG = ENABLE;                                                  // управление интервалом между отправлением пакетов (бит IPG_EN);
//EthernetInit.ETH_Retry_Counter = 0x0A;                                                    // управление максимальным числом повторений (поле RtryCnt).
//	
//	
//  
//EthernetInit.ETH_PHY_Address = 0x1C;
//    /* PHY Auto-negotiation enabled */
//EthernetInit.ETH_PHY_Mode = ETH_PHY_MODE_AutoNegotiation;
//    /* Set the working interface */
// EthernetInit.ETH_PHY_Interface = ETH_PHY_INTERFACE_ETHERNET_802_3;

//	//	MAC General Config	
//EthernetInit.ETH_Dilimiter = 0x1000;
////EthernetInit.ETH_DBG_Mode
////EthernetInit.ETH_DBG_XF
////EthernetInit.ETH_DBG_RF
////EthernetInit.ETH_Loopback_Mode
//EthernetInit.ETH_Receiver_RST = SET;
//EthernetInit.ETH_Transmitter_RST = SET;
////EthernetInit.ETH_Register_CLR
//EthernetInit.ETH_Buffer_Mode =  ETH_BUFFER_MODE_FIFO;
////EthernetInit.ETH_Extension_Mode	
////EthernetInit.ETH_HalfDuplex_Mode
////EthernetInit.ETH_DTRM
////EthernetInit.ETH_Pause
////EthernetInit.ETH_ColWnd

//EthernetInit.ETH_Source_Addr_HASH_Filter = DISABLE;

//EthernetInit.ETH_MAC_Address[0] = (SA_MAC_Address[1] | SA_MAC_Address[0]<<8) ;
//EthernetInit.ETH_MAC_Address[1] = (SA_MAC_Address[3] | SA_MAC_Address[2]<<8);
//EthernetInit.ETH_MAC_Address[2] = (SA_MAC_Address[5] | SA_MAC_Address[4]<<8);
//	
//ETH_Init(MDR_ETHERNET1,&EthernetInit);
//	NVIC_ClearPendingIRQ(ETHERNET_IRQn);
//	NVIC_EnableIRQ(ETHERNET_IRQn);
//	ETH_PHYCmd(MDR_ETHERNET1, ENABLE);
//ETH_Start(MDR_ETHERNET1);	






	ETH_InitTypeDef initStruct;

	// Reset ethernet clock settings
	ETH_ClockDeInit();

	RST_CLK_PCLKcmd(RST_CLK_PCLK_DMA, ENABLE);

	// Enable HSE2 oscillator 25 MHz
	RST_CLK_HSE2config(RST_CLK_HSE2_ON);
	while( RST_CLK_HSE2status()!=SUCCESS ) ;

	// Config PHY clock
	ETH_PHY_ClockConfig(ETH_PHY_CLOCK_SOURCE_HSE2, ETH_PHY_HCLKdiv1);
	ETH_BRGInit(ETH_HCLKdiv1);
	ETH_ClockCMD(ETH_CLK1, ENABLE);
	ETH_DeInit(MDR_ETHERNET1);

	ETH_StructInit( &initStruct );
	initStruct.ETH_PHY_Mode                 = ETH_PHY_MODE_AutoNegotiation;
	initStruct.ETH_Transmitter_RST          = SET;
	initStruct.ETH_Receiver_RST             = SET;
	initStruct.ETH_Buffer_Mode              = ETH_BUFFER_MODE_LINEAR;
	initStruct.ETH_Source_Addr_HASH_Filter  = DISABLE;
	// ERRATA 0019
	//initStruct.ETH_Control_Frames_Reception = ENABLE;
	// Set the MAC address (see: tcpip.h).

initStruct.ETH_MAC_Address[0] = (SA_MAC_Address[0] | SA_MAC_Address[1]<<8) ;
initStruct.ETH_MAC_Address[1] = (SA_MAC_Address[2] | SA_MAC_Address[3]<<8);
initStruct.ETH_MAC_Address[2] = (SA_MAC_Address[4] | SA_MAC_Address[5]<<8);
	ETH_Init(MDR_ETHERNET1, &initStruct);
	ETH_PHYCmd(MDR_ETHERNET1, ENABLE);



	// IRQ Handler init at RX.
	ETH_MACITConfig(MDR_ETHERNET1, ETH_MAC_IT_RF_OK, ENABLE);
	NVIC_ClearPendingIRQ(ETHERNET_IRQn);
	NVIC_EnableIRQ(ETHERNET_IRQn);
	ETH_Start(MDR_ETHERNET1);









}



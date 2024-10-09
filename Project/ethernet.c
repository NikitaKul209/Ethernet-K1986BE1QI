#include "ethernet.h"
#include "main.h"


uint8_t DA_MAC_Address[6] = {0x58,0xD5,0x6E,0x3E,0x11,0xC8};
uint8_t SA_MAC_Address[6] = {0x34,0x13,0x78,0x56,0xbc,0x9a};
uint8_t SA_IP_Address[4] = { 169,254,254,172};
uint8_t DA_IP_Address[4] = { 169,254,254,1};


void set_ethernet(void)
{

  ETH_InitTypeDef ETH_InitStruct;
  ETH_DeInit(MDR_ETHERNET1);
  ETH_StructInit((ETH_InitTypeDef *) &ETH_InitStruct);
	
	ETH_InitStruct.ETH_Error_CRC_Frames_Reception = DISABLE;

  ETH_InitStruct.ETH_Receive_All_Packets = DISABLE;
  ETH_InitStruct.ETH_Unicast_Frames_Reception = ENABLE;
	
  ETH_InitStruct.ETH_PHY_Mode = ETH_PHY_MODE_AutoNegotiation;
  ETH_InitStruct.ETH_Transmitter_RST = SET;
  ETH_InitStruct.ETH_Receiver_RST = SET;
  ETH_InitStruct.ETH_Buffer_Mode = ETH_BUFFER_MODE_LINEAR;
  ETH_InitStruct.ETH_Control_Frames_Reception = ENABLE;
  ETH_InitStruct.ETH_Source_Addr_HASH_Filter = DISABLE;
  ETH_InitStruct.ETH_MAC_Address[2] = (SA_MAC_Address[5]<<8 | SA_MAC_Address[4]) ;
  ETH_InitStruct.ETH_MAC_Address[1] = (SA_MAC_Address[3]<<8 | SA_MAC_Address[2]);
  ETH_InitStruct.ETH_MAC_Address[0] = (SA_MAC_Address[1]<<8 | SA_MAC_Address[0]);
  ETH_InitStruct.ETH_Dilimiter = 0x1000;
  ETH_Init(MDR_ETHERNET1, (ETH_InitTypeDef *) &ETH_InitStruct);
  ETH_MACITConfig(MDR_ETHERNET1, ETH_MAC_IT_RF_OK, ENABLE);
  NVIC_EnableIRQ(ETHERNET_IRQn);
  ETH_PHYCmd(MDR_ETHERNET1, ENABLE);
  ETH_Start(MDR_ETHERNET1);

}
void ethernet_PHY_Status(void)
{		
if(!(MDR_ETHERNET1->PHY_Status & ETH_PHY_STATUS_LED1)) PORT_SetBits(MDR_PORTE,PORT_Pin_6);
	else PORT_ResetBits(MDR_PORTE,PORT_Pin_6);

if(!(MDR_ETHERNET1->PHY_Status & ETH_PHY_STATUS_LED3)) PORT_SetBits(MDR_PORTE,PORT_Pin_7);
	 else PORT_ResetBits(MDR_PORTE,PORT_Pin_7);	
	
}

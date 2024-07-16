
#include "main.h"
#include "ethernet.h"

extern uint8_t DA_MAC_Address[6];
extern uint8_t SA_MAC_Address[6];

int main(void)
{
  uint32_t output_frame [1500];
  uint32_t input_frame [1500];

  output_frame[0] = DA_MAC_Address[0];
  output_frame[1] = DA_MAC_Address[1];
  output_frame[2] = DA_MAC_Address[2];
  output_frame[3] = DA_MAC_Address[3];
  output_frame[4] = DA_MAC_Address[4];
  output_frame[5] = DA_MAC_Address[5];

  output_frame[6] = SA_MAC_Address[0];
  output_frame[7] = SA_MAC_Address[1];
  output_frame[8] = SA_MAC_Address[2];
  output_frame[9] = SA_MAC_Address[3];
  output_frame[10] = SA_MAC_Address[4];
  output_frame[11] = SA_MAC_Address[5];

  set_clk();
  set_port();
  set_ethernet();
	
  while(1)
    {
//		void ETH_SendFrame(MDR_ETHERNET_TypeDef * ETHERNETx, uint32_t * ptr_OutputBuffer, uint32_t BufLen)
      ETH_SendFrame(  MDR_ETHERNET1,output_frame,18+30);

      //
      //
      //	PORT_WriteBit(MDR_PORTB,PORT_Pin_0, SET);
      //		PORT_WriteBit(MDR_PORTD,PORT_Pin_12, SET);





    }



}
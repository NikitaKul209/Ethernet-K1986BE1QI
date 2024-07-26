
#include "main.h"
#include "ethernet.h"

extern uint8_t DA_MAC_Address[6];
extern uint8_t SA_MAC_Address[6];
extern uint8_t SA_IP_Address[6];
extern uint8_t DA_IP_Address[6];
uint32_t output_frame [150] ;
//  uint32_t input_frame [150];
bool  timer_flag = false;
int main(void)
{

  output_frame[0] = 0xff;
  output_frame[1] = 0xff;
  output_frame[2] = 0xff;
  output_frame[3] = 0xff;
  output_frame[4] = 0xff;
  output_frame[5] = 0xff;
//  output_frame[0] = DA_MAC_Address[0];
//  output_frame[1] = DA_MAC_Address[1];
//  output_frame[2] = DA_MAC_Address[2];
//  output_frame[3] = DA_MAC_Address[3];
//  output_frame[4] = DA_MAC_Address[4];
//  output_frame[5] = DA_MAC_Address[5];

  output_frame[6] = SA_MAC_Address[0];
  output_frame[7] = SA_MAC_Address[1];
  output_frame[8] = SA_MAC_Address[2];
  output_frame[9] = SA_MAC_Address[3];
  output_frame[10] = SA_MAC_Address[4];
  output_frame[11] = SA_MAC_Address[5];

  output_frame[12] = 0x8;
  output_frame[13] = 0x6;

  output_frame[14] = 0x0;
  output_frame[15] = 0x1;

  output_frame[16] = 0x08;
  output_frame[17] = 0x0;

  output_frame[18] = 0x06;

  output_frame[19] = 0x4;

  output_frame[20] = 0x00;
  output_frame[21] = 0x01;

  output_frame[22] = SA_MAC_Address[0];
  output_frame[23] = SA_MAC_Address[1];
  output_frame[24] = SA_MAC_Address[2];
  output_frame[25] = SA_MAC_Address[3];
  output_frame[26] = SA_MAC_Address[4];
  output_frame[27] = SA_MAC_Address[5];

  output_frame[28] = SA_IP_Address[0];
  output_frame[29] = SA_IP_Address[1];
  output_frame[30] = SA_IP_Address[2];
  output_frame[31] = SA_IP_Address[3];



  output_frame[32] = 0x0;
  output_frame[33] = 0x0;
  output_frame[34] = 0x0;
  output_frame[35] = 0x0;
  output_frame[36] = 0x0;
  output_frame[37] = 0x0;

  output_frame[38] = DA_IP_Address[0];
  output_frame[39] = DA_IP_Address[1];
  output_frame[40] = DA_IP_Address[2];
  output_frame[41] = DA_IP_Address[3];



  set_clk();
		set_timer();
//  set_port();
  set_ethernet();
;
  NVIC_EnableIRQ(TIMER1_IRQn );
  TIMER_Cmd(MDR_TIMER1, ENABLE);
            TIMER_SetCounter(MDR_TIMER1,0x0);


  while(1)
    {

      if(timer_flag)
        {
          timer_flag = false;
          ETH_SendFrame(  MDR_ETHERNET1,output_frame,42);
					TIMER_Cmd(MDR_TIMER1, ENABLE);

        }

    }



}

void TIMER1_IRQHandler(void)
{
  if (TIMER_GetITStatus(MDR_TIMER1, TIMER_STATUS_CNT_ARR))
    {
      TIMER_ClearITPendingBit(MDR_TIMER1, TIMER_STATUS_CNT_ARR);
      TIMER_Cmd(MDR_TIMER1, DISABLE);
      TIMER_SetCounter(MDR_TIMER1,0x0);
      timer_flag = true;
    }
}
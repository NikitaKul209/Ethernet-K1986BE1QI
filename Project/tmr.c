#include "tmr.h"

static TIMER_CntInitTypeDef TimerInit;

void set_timer(void)
{
  TIMER_CntStructInit(&TimerInit);

  TimerInit.TIMER_Prescaler = 0x6;
  TimerInit.TIMER_Period = 100000;
  TimerInit.TIMER_CounterDirection =  TIMER_CntDir_Up;
  TimerInit.TIMER_EventSource = TIMER_EvSrc_TIM_CLK;

  TIMER_ITConfig(MDR_TIMER2, TIMER_STATUS_CNT_ARR, ENABLE);
  TIMER_CntInit(MDR_TIMER2, &TimerInit);

  TIMER_ClearITPendingBit(MDR_TIMER2, TIMER_STATUS_CNT_ARR);
  TIMER_SetCounter(MDR_TIMER2,0x0);
  NVIC_EnableIRQ(TIMER2_IRQn );
//    TimerInit.TIMER_Period = 1750;
//    TIMER_ITConfig(MDR_TIMER2, TIMER_STATUS_CNT_ARR, ENABLE);
//    TIMER_CntInit(MDR_TIMER2, &TimerInit);
//}


}
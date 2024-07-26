#include "tmr.h"

static TIMER_CntInitTypeDef TimerInit;

void set_timer(void)
{
    TIMER_CntStructInit(&TimerInit);

    TimerInit.TIMER_Prescaler = 0x38;
    TimerInit.TIMER_Period = 75000;
    TimerInit.TIMER_CounterDirection =  TIMER_CntDir_Up;
    TimerInit.TIMER_EventSource = TIMER_EvSrc_TIM_CLK;

    TIMER_ITConfig(MDR_TIMER1, TIMER_STATUS_CNT_ARR, ENABLE);
    TIMER_CntInit(MDR_TIMER1, &TimerInit);

//    TimerInit.TIMER_Period = 1750;
//    TIMER_ITConfig(MDR_TIMER2, TIMER_STATUS_CNT_ARR, ENABLE);
//    TIMER_CntInit(MDR_TIMER2, &TimerInit);
//}


}
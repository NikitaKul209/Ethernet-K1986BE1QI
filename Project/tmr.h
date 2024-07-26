#ifndef TIMER
#define TIMER



#define delta_t_byte 1000       // 128
#define delta_t_frame 1500     // 1024
#define delta_t_request 125000 // 0,5 ??? BKIA
#define delta_t_request_u 5000 // 50 HZ ????? ?856??_U


#define TIMER_ARR 750
#define TIMER_ARR_HALF_SEC 125000
#define TIMER_TEST 240000

#include "main.h"

void set_timer(void);


#endif

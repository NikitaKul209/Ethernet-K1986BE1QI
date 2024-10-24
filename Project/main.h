#ifndef MAIN
#define MAIN

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <locale.h>
#include "MDR32F9Qx_port.h"
#include "MDR32F9Qx_rst_clk.h"
#include "MDR32F9Qx_config.h"
#include "MDR32F9Qx_timer.h"
#include "MDR32F9Qx_uart.h"
#include "MDR32F9Qx_ssp.h"
#include "MDR32F9Qx_eth.h"
#include "MDR32F9Qx_adc.h"
#include "port.h"
#include "clk.h"
#include "ethernet.h"
#include "tmr.h"
#include "adc.h"
#include "spi.h"

void delay(uint32_t ticks);


#endif
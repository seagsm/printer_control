#ifndef BOARD_INIT_H
#define BOARD_INIT_H 1



#include "stm32f10x.h"
#include "board_system_type.h"
#include "board_state.h"
#include "board_gpio.h"
#include "board_uart.h"
#include "board_sys_tick.h"
#include "board_pwm.h"
#include "board_spi_dma.h"
#include "board_capture.h"
#include "board_encoder_emulation.h"



/* #define BOARD_INIT_TOTAL_STEPS 12U */
#define BOARD_INIT_TOTAL_STEPS 7U
BOARD_ERROR be_board_init_main_init(void);




#endif
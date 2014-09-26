#ifndef BOARD_TIMER_H
#define BOARD_TIMER_H 1

#include "stm32f10x.h"
#include "board_system_type.h"
#include "board_NVIC.h"

BOARD_ERROR be_board_timer_init(void);
static BOARD_ERROR be_board_timer2_init(void);
static BOARD_ERROR be_board_timer3_init(void);
void TIM2_IRQHandler(void);
void TIM3_IRQHandler(void);

#endif
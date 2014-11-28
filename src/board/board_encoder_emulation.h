#ifndef BOARD_ENCODER_EMULATION_H
#define BOARD_ENCODER_EMULATION_H 1


#include "stm32f10x.h"
#include "stm32f10x_tim.h"
#include "board_system_type.h"
#include "board_NVIC.h"

#define ZERO_SPEED_PERIOD   4000U 

       BOARD_ERROR board_encoder_emulation_init(void);
static BOARD_ERROR board_encoder_emulation_timer_init(void);
static        void board_encoder_emulation_set_period(uint16_t u16_period);
static        void board_encoder_emulation_proccess(void);
static        void board_encoder_emulation_start(void);
void board_encoder_emulation_stop(void);




void TIM1_UP_IRQHandler(void);






#endif
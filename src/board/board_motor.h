#ifndef BOARD_MOTOR_H
#define BOARD_MOTOR_H 1


#include "stm32f10x.h"
#include "stm32f10x_tim.h"
#include "board_system_type.h"
#include "board_NVIC.h"

#define BOARD_MOTOR_STEP_PERIOD     100U


       BOARD_ERROR board_motor_init(void);
static BOARD_ERROR board_motor_timer_init(void);

              void board_motor_step(int8_t i8_step);
     
              void TIM3_IRQHandler(void);



#endif
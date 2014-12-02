#ifndef BOARD_MOTOR_H
#define BOARD_MOTOR_H 1


#include "stm32f10x.h"
#include "stm32f10x_tim.h"
#include "board_system_type.h"
#include "board_NVIC.h"
#include "board_gpio_table.h"


#define BOARD_MOTOR_STEP_PERIOD     100U


       BOARD_ERROR board_motor_init(void);
static        void board_motor_timer_init(void);
static        void board_motor_enable(uint8_t uint8_enable);
static        void board_motor_direction(int32_t int32_direction);

              void board_motor_step(int8_t i8_step);
     
              void TIM3_IRQHandler(void);
              
static        void board_motor_timer_pulse_counter_init(void);              
              void TIM4_IRQHandler(void);


#endif
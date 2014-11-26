#ifndef BOARD_CAPTURE_H
#define BOARD_CAPTURE_H 1


#include "stm32f10x.h"
#include "stm32f10x_tim.h"
#include "board_system_type.h"
#include "board_NVIC.h"




BOARD_ERROR be_board_capture_pwm_init(void);

void board_capture_pwm_TIM_2_start(void);
void board_capture_pwm_TIM_2_stop(void);
void board_capture_pwm_TIM_4_start(void);
void board_capture_pwm_TIM_4_stop(void);



static void TIM2_Configuration(void);
static void GPIO_Configuration(void);
static void NVIC_Configuration(void);
void TIM2_IRQHandler(void);
void test(void);

PWM_CAPTURE_STATE board_capture_get_pwm_command(void);
void board_capture_set_pwm_command(PWM_CAPTURE_STATE command);

#endif
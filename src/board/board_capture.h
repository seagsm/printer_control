#ifndef BOARD_CAPTURE_H
#define BOARD_CAPTURE_H 1


#include "stm32f10x.h"
#include "stm32f10x_tim.h"
#include "board_system_type.h"
#include "board_NVIC.h"
#include "board_encoder_emulation.h"


extern uint16_t u16_board_capture_duty_value;
extern uint16_t u16_board_capture_period_value;

BOARD_ERROR be_board_capture_pwm_init(void);

void board_capture_pwm_TIM_start(TIM_TypeDef* TIMx);
void board_capture_pwm_TIM_stop(TIM_TypeDef* TIMx);

static void TIM2_Configuration(void);

static void board_capture_gpio_configuration(void);
static void board_capture_nvic_configuration(void);
static void board_capture_tim_configuration(void);
static void board_capture_tim2_configuration(void);
static void board_capture_tim4_configuration(void);
static void board_capture_dma1_ch5_init(void);
static void board_capture_dma1_ch7_init(void);

void TIM2_IRQHandler(void);
void TIM4_IRQHandler(void);
void test(void);

PWM_CAPTURE_STATE board_capture_get_pwm_command(void);
void board_capture_set_pwm_command(PWM_CAPTURE_STATE command);

uint16_t board_capture_pwm_TIM2_duty(void);
uint16_t board_capture_pwm_TIM4_duty(void);

#endif
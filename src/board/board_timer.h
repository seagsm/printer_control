#ifndef BOARD_TIMER_H
#define BOARD_TIMER_H 1

#include "stm32f10x.h"
#include "board_system_type.h"
#include "board_NVIC.h"

BOARD_ERROR be_board_timer_init(void);
static BOARD_ERROR be_board_timer_module_init(
                                        TIM_TypeDef* TIM_timer,
                                        uint8_t IRQChannelPreemptionPriority,
                                        uint8_t IRQChannelSubPriority,
                                        uint16_t TIM_Period,
                                        uint16_t TIM_Prescaler,
                                        uint16_t TIM_ClockDivision,
                                        uint16_t TIM_CounterMode
                                       );
static BOARD_ERROR be_board_timer_set_period(TIM_TypeDef* TIMx, uint16_t TIM_Period);

void v_board_timer_set_step(uint8_t u8_step_set);

void TIM2_IRQHandler(void);
void TIM3_IRQHandler(void);

#endif



#include "board_motor.h"


BOARD_ERROR board_motor_init(void)
{
    BOARD_ERROR be_result = BOARD_ERR_OK;

    be_result =  board_motor_timer_init();
    
    return(be_result);
}

void board_motor_step(int8_t i8_step)
{







}


void TIM3_IRQHandler(void)
{
    uint16_t u16_tmp = 0U;
    if(TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
    {
         TIM_ClearITPendingBit(TIM3, TIM_IT_Update);            /* Counter overflow, reset interrupt */
    }
 
}


/* Initialisation of timer for generation of pulses for motor driver. */
static BOARD_ERROR board_motor_timer_init(void)
{
    BOARD_ERROR be_result = BOARD_ERR_OK;
    
    NVIC_InitTypeDef   NVIC_InitStructure;
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

    /* Enable the TIM1 global Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel                      = (unsigned char)TIM3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority    = TIMER3_PERIOD_INTERUPT_PRIORITY_GROUP;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority           = TIMER3_PERIOD_INTERUPT_SUB_PRIORITY_GROUP;
    NVIC_InitStructure.NVIC_IRQChannelCmd                   = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    /* TIM3 clock enable*/
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
    RCC_APB1PeriphResetCmd(RCC_APB1Periph_TIM3, DISABLE);

    /* Time Base configuration */
    TIM_TimeBaseStructure.TIM_Period        = BOARD_MOTOR_STEP_PERIOD;
    TIM_TimeBaseStructure.TIM_Prescaler     = 71U;          /* Ftimer=fsys/(Prescaler+1),for Prescaler=71 ,Ftimer=1MHz */
    TIM_TimeBaseStructure.TIM_ClockDivision = 0U;
    TIM_TimeBaseStructure.TIM_CounterMode   = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);

    TIM_ClearITPendingBit(TIM3, TIM_IT_Update);             /* Clear pending interrupt. */
    TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
    return(be_result); 
}

#include "board_timer.h"


BOARD_ERROR be_board_timer_init(void)
{
    BOARD_ERROR be_result = BOARD_ERR_OK;

    be_board_timer2_init();
    be_board_timer3_init();

    return(be_result);
}
static BOARD_ERROR be_board_timer2_init(void)
{
    BOARD_ERROR be_result = BOARD_ERR_OK;

    NVIC_InitTypeDef   NVIC_InitStructure;
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

    /* Enable the TIM1 global Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel                    = (unsigned char)TIM2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority  = TIMER2_PERIOD_INTERUPT_PRIORITY_GROUP;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority         = TIMER2_PERIOD_INTERUPT_SUB_PRIORITY_GROUP;
    NVIC_InitStructure.NVIC_IRQChannelCmd                 = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    /* Init Timer1 like PWM. */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

    /* Time Base configuration */
    TIM_TimeBaseStructure.TIM_Period         = 20U;
    TIM_TimeBaseStructure.TIM_Prescaler      = 72U;          /* Ftimer=fsys/Prescaler,for Prescaler=72 ,Ftimer=1MHz */
    TIM_TimeBaseStructure.TIM_ClockDivision  = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode    = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

    TIM_ClearITPendingBit(TIM2, TIM_IT_Update);             /* Clear pending interrupt. */
    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);


    TIM_Cmd(TIM2, ENABLE);
    NVIC_EnableIRQ(TIM2_IRQn);


    return(be_result);
}


static BOARD_ERROR be_board_timer3_init(void)
{
    BOARD_ERROR be_result = BOARD_ERR_OK;

    NVIC_InitTypeDef   NVIC_InitStructure;
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

    /* Enable the TIM3 global Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel                    = (unsigned char)TIM3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority  = TIMER3_PERIOD_INTERUPT_PRIORITY_GROUP;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority         = TIMER3_PERIOD_INTERUPT_SUB_PRIORITY_GROUP;
    NVIC_InitStructure.NVIC_IRQChannelCmd                 = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    /* Init Timer3 like PWM. */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

    /* Time Base configuration */
    TIM_TimeBaseStructure.TIM_Period         = 10U;
    TIM_TimeBaseStructure.TIM_Prescaler      = 72U;          /* Ftimer=fsys/Prescaler,for Prescaler=72 ,Ftimer=1MHz */
    TIM_TimeBaseStructure.TIM_ClockDivision  = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode    = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

    TIM_ClearITPendingBit(TIM3, TIM_IT_Update);             /* Clear pending interrupt. */
    TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);


    /*TIM_Cmd(TIM2, ENABLE);*/
    NVIC_EnableIRQ(TIM3_IRQn);


    return(be_result);
}






void TIM2_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
    {
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
        GPIO_SetBits(GPIOB, GPIO_Pin_12);
        TIM_Cmd(TIM3, ENABLE);
    }
}


void TIM3_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
    {
        TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
        GPIO_ResetBits(GPIOB, GPIO_Pin_12);
        TIM_Cmd(TIM3, DISABLE);
    }
}


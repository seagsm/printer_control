
#include "board_timer.h"


static uint8_t u8_step_on;


BOARD_ERROR be_board_timer_init(void)
{
    BOARD_ERROR be_result = BOARD_ERR_OK;


    be_board_timer_module_init(
                                TIM2,
                                TIMER2_PERIOD_INTERUPT_PRIORITY_GROUP,
                                TIMER2_PERIOD_INTERUPT_SUB_PRIORITY_GROUP,
                                50U,/* 20 is 20uS period for 72 prescaler. */
                                72U,/* Ftimer=fsys/Prescaler,for Prescaler=72 ,Ftimer=1MHz */
                                TIM_CKD_DIV1,
                                TIM_CounterMode_Up
                              );

    be_board_timer_module_init(
                                TIM3,
                                TIMER3_PERIOD_INTERUPT_PRIORITY_GROUP,
                                TIMER3_PERIOD_INTERUPT_SUB_PRIORITY_GROUP,
                                10U,
                                72U,
                                TIM_CKD_DIV1,
                                TIM_CounterMode_Up
                              );

    /* Start generation. */
    TIM_Cmd(TIM2, ENABLE);

    return(be_result);
}

static BOARD_ERROR be_board_timer_module_init(
                                        TIM_TypeDef* TIM_timer,
                                        uint8_t IRQChannelPreemptionPriority,
                                        uint8_t IRQChannelSubPriority,
                                        uint16_t TIM_Period,
                                        uint16_t TIM_Prescaler,
                                        uint16_t TIM_ClockDivision,
                                        uint16_t TIM_CounterMode
                                       )
{

    BOARD_ERROR be_result = BOARD_ERR_OK;
    NVIC_InitTypeDef        NVIC_InitStructure;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    IRQn_Type               TIM_IRQn;
    uint32_t                RCC_APB1Periph = 0U;

    TIM_DeInit(TIM_timer);

    if(TIM_timer == TIM2)
    {
        TIM_IRQn = TIM2_IRQn;
        RCC_APB1Periph = RCC_APB1Periph_TIM2;
    }
    else if(TIM_timer == TIM3)
    {
       TIM_IRQn = TIM3_IRQn;
       RCC_APB1Periph = RCC_APB1Periph_TIM3;
    }
    else
    {
        be_result = BOARD_ERR_RANGE;
    }
    if(be_result == BOARD_ERR_OK)
    {

        RCC_APB1PeriphClockCmd(RCC_APB1Periph, ENABLE);

        /* Time Base configuration */
        TIM_TimeBaseStructure.TIM_Period         = TIM_Period;
        TIM_TimeBaseStructure.TIM_Prescaler      = TIM_Prescaler;          /* Ftimer=fsys/Prescaler,for Prescaler=72 ,Ftimer=1MHz */
        TIM_TimeBaseStructure.TIM_ClockDivision  = TIM_ClockDivision;
        TIM_TimeBaseStructure.TIM_CounterMode    = TIM_CounterMode;

        TIM_TimeBaseInit(TIM_timer, &TIM_TimeBaseStructure);

        TIM_ClearITPendingBit(TIM_timer, TIM_IT_Update);
        TIM_ITConfig(TIM_timer, TIM_IT_Update, ENABLE);

        /* Enable the TIMn global Interrupt */
        NVIC_InitStructure.NVIC_IRQChannel                    = (unsigned char)TIM_IRQn;
        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority  = IRQChannelPreemptionPriority;
        NVIC_InitStructure.NVIC_IRQChannelSubPriority         = IRQChannelSubPriority;
        NVIC_InitStructure.NVIC_IRQChannelCmd                 = ENABLE;
        NVIC_Init(&NVIC_InitStructure);
        NVIC_EnableIRQ(TIM_IRQn);
    }

    return(be_result);
}

static BOARD_ERROR be_board_timer_set_period(TIM_TypeDef* TIMx, uint16_t TIM_Period)
{
    BOARD_ERROR be_result = BOARD_ERR_OK;

    /* Set the Autoreload value */
    TIMx->ARR = TIM_Period ;

    return(be_result);
}

void TIM2_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
    {
        if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_10) == 0U)
        {
            if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) == 0U)
            {
                if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1) == 1U)
                {
                    TIM_Cmd(TIM3, ENABLE);
                    GPIO_SetBits(GPIOB, GPIO_Pin_11);
                }
            }
            else if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1) == 0U)
            {
                if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) == 1U)
                {
                    TIM_Cmd(TIM3, ENABLE);
                    GPIO_ResetBits(GPIOB, GPIO_Pin_11);
                }
            }
            else if(u8_step_on == 1U)
            {
                TIM_Cmd(TIM3, ENABLE);
                u8_step_on = 0U;
            }
        }
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
        GPIO_SetBits(GPIOB, GPIO_Pin_12);
    }
}

/* Generate one pulse end disable timer. */
void TIM3_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
    {
        TIM_Cmd(TIM3, DISABLE);
        TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
        GPIO_ResetBits(GPIOB, GPIO_Pin_12);

    }
}

void v_board_timer_set_step(uint8_t u8_step_set)
{
    u8_step_on = u8_step_set;
}
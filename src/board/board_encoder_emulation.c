

#include "board_encoder_emulation.h"

/**/
BOARD_ERROR board_encoder_emulation_init(void)
{
    BOARD_ERROR be_result = BOARD_ERR_OK;
    
    be_result = board_encoder_emulation_timer_init();

    /**/
    board_encoder_emulation_start();
    /**/
    
    return(be_result);
}

static void board_encoder_emulation_start(void)
{
    TIM_Cmd(TIM1, ENABLE);
    NVIC_EnableIRQ(TIM1_UP_IRQn);
}

void board_encoder_emulation_stop(void)
{
    TIM_Cmd(TIM1, DISABLE);
    NVIC_DisableIRQ(TIM1_UP_IRQn);
}                
                
static void board_encoder_emulation_set_period(uint16_t u16_period)
{
    TIM1->ARR = u16_period;
}



void TIM1_UP_IRQHandler(void)
{
    if(TIM_GetITStatus(TIM1, TIM_IT_Update) != RESET)
    {
         TIM_ClearITPendingBit(TIM1, TIM_IT_Update);            /* Counter overflow, reset interrupt */
         board_encoder_emulation_proccess();
         
    }
}

static void board_encoder_emulation_proccess(void)
{
    uint16_t u16_current_period = 0U;
    
    static uint8_t u8_flag = 0U;
        
    if(u8_flag == 0U)
    {
        GPIO_ResetBits( GPIOB, GPIO_Pin_1);
        u8_flag = 1U;
    }
    else
    {
        GPIO_SetBits( GPIOB, GPIO_Pin_1);
        u8_flag = 0U;
    }  

    u16_current_period = TIM1->ARR;
    
    if(u16_current_period < 100U)
    {
        u16_current_period = ZERO_SPEED_PERIOD;
    }
    else
    {
        u16_current_period--;
    }
    
    board_encoder_emulation_set_period(u16_current_period);
    
    
}













static BOARD_ERROR board_encoder_emulation_timer_init(void)
{
    BOARD_ERROR be_result = BOARD_ERR_OK;

    NVIC_InitTypeDef   NVIC_InitStructure;
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

    /* Enable the TIM1 global Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel                      = (unsigned char)TIM1_UP_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority    = TIMER1_PERIOD_INTERUPT_PRIORITY_GROUP;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority           = TIMER1_PERIOD_INTERUPT_SUB_PRIORITY_GROUP;
    NVIC_InitStructure.NVIC_IRQChannelCmd                   = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    /* Init Timer1 like PWM. */
    RCC_APB2PeriphClockCmd (RCC_APB2Periph_TIM1, ENABLE);

    /* Time Base configuration */
    TIM_TimeBaseStructure.TIM_Period        = ZERO_SPEED_PERIOD;
    TIM_TimeBaseStructure.TIM_Prescaler     = 71U;          /* Ftimer=fsys/(Prescaler+1),for Prescaler=71 ,Ftimer=1MHz */
    TIM_TimeBaseStructure.TIM_ClockDivision = 0U;
    TIM_TimeBaseStructure.TIM_CounterMode   = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);

    TIM_ClearITPendingBit(TIM1, TIM_IT_Update);             /* Clear pending interrupt. */
    TIM_ITConfig(TIM1, TIM_IT_Update, ENABLE);
    return(be_result);
}        








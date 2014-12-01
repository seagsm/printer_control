


#include "board_motor.h"


BOARD_ERROR board_motor_init(void)
{
    BOARD_ERROR be_result = BOARD_ERR_OK;

    board_motor_timer_init();
    board_motor_timer_pulse_counter_init();
    
    return(be_result);
}

void board_motor_step(int8_t i8_step)
{



}

void TIM3_IRQHandler(void)
{
    if(TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
    {
         TIM_ClearITPendingBit(TIM3, TIM_IT_Update);            /* Counter overflow, reset interrupt */
    }
}

/* Initialisation of timer 3 for generation of pulses for motor driver. */
#if 0
static void board_motor_timer_init(void)
{
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
}
#endif

static void board_motor_timer_init(void)
{
    uint16_t u16_pwm_period = 1000U;
    uint16_t u16_pwm_duty   = 50U;  
  
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_OCInitTypeDef TIM_OCInitStructure;
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

    /* Connect clock to port B for ch 3(PB0). */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    GPIO_InitStructure.GPIO_Mode        = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed       = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;  /*PB0 -> TIM3_CH3 */
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    
    TIM_OCInitStructure.TIM_OCMode      = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse       = u16_pwm_duty;
    TIM_OCInitStructure.TIM_OCPolarity  = TIM_OCPolarity_High;
    TIM_OC3Init(TIM3, &TIM_OCInitStructure);    
    
    /* Init Timer3 like PWM. */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
    /* Time Base configuration. */
    TIM_TimeBaseStructure.TIM_Period        = u16_pwm_period;
    TIM_TimeBaseStructure.TIM_Prescaler     = 3U;         /* Ftimer=Fsys/Prescaler,for Fsys=72MHz and Prescaler=72, Ftimer=1MHz. */
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode   = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
    
    
    /* Turn on output triger of TIM3 to OnUpdate event. */
    TIM_SelectOutputTrigger(TIM3, TIM_TRGOSource_Update);
    TIM_Cmd(TIM3, ENABLE);
}

/* Initialisation of timer 4 for calculation of numbers of pulses from PWM generated by TIM3. */
/* This timer should count 9 OnUpdate event from TIM3(PWM) and disable it. 
 * It is neccesary for generation 9 pulses for each encoder step. 
 */
static void board_motor_timer_pulse_counter_init(void)
{
    NVIC_InitTypeDef   NVIC_InitStructure;
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

    /* Enable the TIM4 global Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel                      = (unsigned char)TIM4_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority    = TIMER4_PERIOD_INTERUPT_PRIORITY_GROUP;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority           = TIMER4_PERIOD_INTERUPT_SUB_PRIORITY_GROUP;
    NVIC_InitStructure.NVIC_IRQChannelCmd                   = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    /* TIM3 clock enable*/
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
    RCC_APB1PeriphResetCmd(RCC_APB1Periph_TIM4, DISABLE);
    
    /* Time Base configuration */
    TIM_TimeBaseStructure.TIM_Period        = 9U;
    TIM_TimeBaseStructure.TIM_Prescaler     = 0U;          /* Ftimer=fsys/(Prescaler+1),for Prescaler=71 ,Ftimer=1MHz */
    TIM_TimeBaseStructure.TIM_ClockDivision = 0U;
    TIM_TimeBaseStructure.TIM_CounterMode   = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);

    /* http://robocraft.ru/blog/ARM/739.html */
    /* Connect TIM4 triger input to TIM3 triger output. */
    TIM_SelectInputTrigger(TIM4, TIM_TS_ITR2);
    
    TIM_SelectMasterSlaveMode(TIM4, TIM_MasterSlaveMode_Enable);
    TIM_SelectSlaveMode(TIM4, TIM_SlaveMode_External1);
    
    TIM_ClearITPendingBit(TIM4, TIM_IT_Update);             /* Clear pending interrupt. */
    TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);
    
    TIM_SetCounter(TIM4, 0U); /* Clear counter. */
    TIM_Cmd(TIM4, ENABLE);    /* Enable timer. */ 
}

void TIM4_IRQHandler(void)
{
    if(TIM_GetITStatus(TIM4, TIM_IT_CC1) == SET)                /* If compare capture has occured */
    {
        TIM_ClearITPendingBit(TIM4, TIM_IT_CC1);
    }

    if(TIM_GetITStatus(TIM4, TIM_IT_CC2) == SET)                /* If compare capture has occured */
    {
        TIM_ClearITPendingBit(TIM4, TIM_IT_CC2);
    }

    if(TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)
    {
         TIM_ClearITPendingBit(TIM4, TIM_IT_Update);            /* Counter overflow, reset interrupt */
         
         /*TODO: Here should be decreased encoder step counter and disabled PWM if 
         * encoder step counter reached 0.
         */ 
         TIM_Cmd(TIM3, DISABLE);
    }
}

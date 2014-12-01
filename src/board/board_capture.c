/*
    This file consist function to fulfill requerement for capture PWM from TIM2CH1
    and TIM4CH1
*/

#include "board_capture.h"

uint16_t u16_board_capture_duty_value;
uint16_t u16_board_capture_period_value;

static PWM_CAPTURE_STATE board_capture_command = PWM_CAPTURE_STOP;

static uint16_t board_capture_TIM2_PWM_duty     = 0U;   /* High Register for T2                         */
static uint16_t board_capture_TIM2_PWM_period   = 0U;   /* Low Regiser for T2 (For Input Capture)       */

#if TIMER4_CAPTURE
static uint16_t board_capture_TIM4_PWM_duty     = 0U;   /* High Register for T4                         */
static uint16_t board_capture_TIM4_PWM_period   = 0U;   /* Low Regiser for T4 (For Input Capture)       */
#endif

static uint8_t board_capture_TIM2_update        = 0U;   /* Flag to update period, set by T2 IRQ         */

#if TIMER4_CAPTURE
static uint8_t board_capture_TIM4_update        = 0U;   /* Flag to update period, set by T4 IRQ         */
#endif

/* Get TIM2 captured PWM duty. */
uint16_t board_capture_pwm_TIM2_duty(void)
{
    return(board_capture_TIM2_PWM_duty);
}

#if TIMER4_CAPTURE
/* Get TIM4 captured PWM duty. */
uint16_t board_capture_pwm_TIM4_duty(void)
{
    return(board_capture_TIM4_PWM_duty);
}
#endif


/* Get current state of PWM capture.*/
PWM_CAPTURE_STATE board_capture_get_pwm_command(void)
{
    return(board_capture_command);
}

/* Input parameter are TIM2 or TIM4. */
void board_capture_pwm_TIM_start(TIM_TypeDef* TIMx)
{
    /* Set flag of module to START state. */
    if(TIMx == TIM2)
    {
        board_capture_command = PWM_CAPTURE_CW_START;
    }
    if(TIMx == TIM4)
    {
        board_capture_command = PWM_CAPTURE_CCW_START;
    }

    /* Clear all interrupt flags. */
    TIM_ClearITPendingBit(TIMx, TIM_IT_CC1 | TIM_IT_CC2 | TIM_IT_Update);
    /* Enable interrupt from block. */
    TIM_ITConfig(TIMx, TIM_IT_CC1 | TIM_IT_CC2 | TIM_IT_Update, ENABLE);
    /* Enable counter. */
    TIM_Cmd(TIMx, ENABLE);
}

/* Input parameter are TIM2 or TIM4. */
void board_capture_pwm_TIM_stop(TIM_TypeDef* TIMx)
{
    /* Set flad of module to STOP state. */
    board_capture_command = PWM_CAPTURE_STOP;

    /* Disable interrupt from module events. */
    TIM_ITConfig(TIMx, TIM_IT_CC1 | TIM_IT_CC2 | TIM_IT_Update, DISABLE);

    /* Disable timer. */
    TIM_Cmd(TIMx, DISABLE);

    /* Clear measured parameters. */
    if(TIMx == TIM2)
    {
        board_capture_TIM2_PWM_period = 0U;
        board_capture_TIM2_PWM_duty   = 0U;
    }

#if TIMER4_CAPTURE
    if(TIMx == TIM4)
    {
        board_capture_TIM4_PWM_period = 0U;
        board_capture_TIM4_PWM_duty   = 0U;
    }
#endif

}

/* Initialisation of capture module. */
BOARD_ERROR be_board_capture_pwm_init(void)
{
    BOARD_ERROR be_result = BOARD_ERR_OK;

    board_capture_gpio_configuration(); /* Inicialisation of GPIO's for capture PWM. */

    /* board_capture_nvic_configuration(); */ /* Initialisation of IRQ for TIM2 and TIM4. */

    board_capture_tim_configuration();  /* Configuretion of capture timers. */

    board_capture_pwm_TIM_stop(TIM2);
#if TIMER4_CAPTURE
    board_capture_pwm_TIM_stop(TIM4);
#endif
/*DEBUG*/
#if 0
    board_capture_pwm_TIM_start(TIM2);
#if TIMER4_CAPTURE
    board_capture_pwm_TIM_start(TIM4);
#endif
#endif

    /* Disable overload event. */
    TIM_ITConfig(TIM2, TIM_IT_Update, DISABLE);
#if TIMER4_CAPTURE
    TIM_ITConfig(TIM4, TIM_IT_Update, DISABLE);
#endif
    return(be_result);
}


/* DEBUG */
void test(void)
{
    uint32_t u32_count = 0U;
    uint32_t u32_flag  = 0U;

    /* board_capture_pwm_TIM_start(TIM2); */
    TIM_Cmd(TIM2, ENABLE);
    /* board_capture_pwm_TIM_start(TIM4); */

    while(1)
    {
        gv_board_sys_tick_delay(50U);
        TIM_Cmd(TIM3, ENABLE);
        if(board_capture_TIM2_update == 1U)
        {
            board_capture_TIM2_update = 0U;
        }
#if TIMER4_CAPTURE
        if(board_capture_TIM4_update == 1U)
        {
            board_capture_TIM4_update = 0U;
        }
#endif
        u32_count++;
        if(u32_count > 6000000U)
        {
            if(u32_flag == 0U)
            {
                u32_flag = 1U;
               /* board_capture_pwm_TIM_2_stop(); */
            }
            else
            {
                u32_flag = 0U;
               /* board_capture_pwm_TIM_2_start(); */
            }
            u32_count = 0U;
        }
    }
}

/*
    timing is :
    T = reg_l * Tau = reg_l * (1/(Fslc / (1 + Prescaler )))=
    = |If Fslc = 72000000 and Prescaler = 4 | = reg_l * ( 1/(72000000/5) )=
    = reg_l * 1 / 14400000 = reg_l * 69.4(4)nS;
*/
void TIM2_IRQHandler(void)
{
    uint16_t u16_tmp = 0U;

    if(TIM_GetITStatus(TIM2, TIM_IT_CC1) == SET)                /* If compare capture has occured, read value of PWM PERIOD*/
    {
        TIM_ClearITPendingBit(TIM2, TIM_IT_CC1);
        board_capture_TIM2_PWM_period = TIM_GetCapture1(TIM2);  /* Get timer counts for Period */
        board_capture_TIM2_update = 1U;                         /* Set Flag to update period */

        /* Set period for encoder timer. */
        u16_tmp = board_capture_TIM2_PWM_period - board_capture_TIM2_PWM_duty; /* Calculation of reverce value of DUTY. */
        board_encoder_emulation_set_target_period(u16_tmp);     /* Set reference period for encoder emulation module. */
                                                                /* CW or CCW flaf will be read by board_capture_get_pwm_command() function. */
    }

    if(TIM_GetITStatus(TIM2, TIM_IT_CC2) == SET)                /* If compare capture has occured, read value of PWM DUTY  */
    {
        TIM_ClearITPendingBit(TIM2, TIM_IT_CC2);
        board_capture_TIM2_PWM_duty = TIM_GetCapture2(TIM2);
    }

    if(TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
    {
         TIM_ClearITPendingBit(TIM2, TIM_IT_Update);            /* Counter overflow, reset interrupt */
    }
}

#if TIMER4_CAPTURE
void TIM4_IRQHandler(void)
{
    uint16_t u16_tmp = 0U;

    if(TIM_GetITStatus(TIM4, TIM_IT_CC1) == SET)                /* If compare capture has occured */
    {
        TIM_ClearITPendingBit(TIM4, TIM_IT_CC1);
        board_capture_TIM4_PWM_period = TIM_GetCapture1(TIM4);  /* Get timer counts for Period */
        board_capture_TIM4_update = 1U;                         /* Set Flag to update period */

        /* Set period for encoder timer. */
        u16_tmp = board_capture_TIM4_PWM_period - board_capture_TIM4_PWM_duty; /* Calculation of reverce value of DUTY. */
        board_encoder_emulation_set_target_period(u16_tmp);     /* Set reference period for encoder emulation module. */
                                                                /* CW or CCW flaf will be read by board_capture_get_pwm_command() function. */
    }

    if(TIM_GetITStatus(TIM4, TIM_IT_CC2) == SET)                /* If compare capture has occured */
    {
        TIM_ClearITPendingBit(TIM4, TIM_IT_CC2);
        board_capture_TIM4_PWM_duty = TIM_GetCapture2(TIM4);

    }

    if(TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)
    {
         TIM_ClearITPendingBit(TIM4, TIM_IT_Update);            /* Counter overflow, reset interrupt */
    }

}
#endif

/* Configuration of IRQ for TIM2 and TIM4. */
static void board_capture_nvic_configuration(void)
{
        NVIC_InitTypeDef NVIC_InitStructure;

       /* Enable TIM2 IRQ */
        NVIC_InitStructure.NVIC_IRQChannel                      = (unsigned char)TIM2_IRQn;
        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority    = TIMER2_PERIOD_INTERUPT_PRIORITY_GROUP;
        NVIC_InitStructure.NVIC_IRQChannelSubPriority           = TIMER2_PERIOD_INTERUPT_SUB_PRIORITY_GROUP;
        NVIC_InitStructure.NVIC_IRQChannelCmd                   = ENABLE;
        NVIC_Init(&NVIC_InitStructure);

#if TIMER4_CAPTURE
        /* Enable TIM42 IRQ */
        NVIC_InitStructure.NVIC_IRQChannel                      = (unsigned char)TIM4_IRQn;
        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority    = TIMER4_PERIOD_INTERUPT_PRIORITY_GROUP;
        NVIC_InitStructure.NVIC_IRQChannelSubPriority           = TIMER4_PERIOD_INTERUPT_SUB_PRIORITY_GROUP;
        NVIC_InitStructure.NVIC_IRQChannelCmd                   = ENABLE;
        NVIC_Init(&NVIC_InitStructure);
#endif
}

/* Input capture GPIO initialisation. */
static void board_capture_gpio_configuration(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    /* GPIOA GPIOB and GPIOC clock enable */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB , ENABLE);

    GPIO_InitStructure.GPIO_Pin         = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode        = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Speed       = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin         = GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Mode        = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Speed       = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
}

static void board_capture_tim_configuration(void)
{
    board_capture_tim2_configuration();
#if TIMER4_CAPTURE
    board_capture_tim4_configuration();
#endif
}

/* Set Timer 2 Interrupt (Input Pulse Capture) */
static void board_capture_tim2_configuration(void)
{
    /* Time base configuration */
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_ICInitTypeDef  TIM_ICInitStructure;

    /* TIM2 clock enable*/
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    RCC_APB1PeriphResetCmd(RCC_APB1Periph_TIM2, DISABLE);

    TIM_TimeBaseStructure.TIM_Prescaler = 4U;
    TIM_TimeBaseStructure.TIM_Period = 0xFFFFU;
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

    TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;
    TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
    TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
    TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
    TIM_ICInitStructure.TIM_ICFilter = 0x00U;
    TIM_ICInit(TIM2, &TIM_ICInitStructure);

    /* Channel 2 Config*/
    TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;
    TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Falling;
    TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_IndirectTI;
    TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
    TIM_ICInitStructure.TIM_ICFilter = 0x00U;
    TIM_ICInit(TIM2, &TIM_ICInitStructure);

    TIM_ClearITPendingBit(TIM2, TIM_IT_CC1 | TIM_IT_CC2 | TIM_IT_Update);
    TIM_ITConfig(TIM2, TIM_IT_CC1, ENABLE);
    TIM_ITConfig(TIM2, TIM_IT_CC2, ENABLE);
    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);

    /*More*/
    TIM_SelectInputTrigger(TIM2,TIM_TS_TI1FP1);
    TIM_SelectSlaveMode(TIM2, TIM_SlaveMode_Reset);

    /* Initialize DMA to transfer CC1 to board_capture_period_value. */
    board_capture_dma1_ch5_init();
    /* Initialize DMA to transfer CC2 to board_capture_duty_value. */
    board_capture_dma1_ch7_init();

    /* TIM2 enable counter */
    /* TIM_Cmd(TIM2, ENABLE); */ /* Should be start by start function. */
}

#if TIMER4_CAPTURE
/* Set Timer 4 Interrupt (Input Pulse Capture) */
static void board_capture_tim4_configuration(void)
{
    /* Time base configuration */
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_ICInitTypeDef  TIM_ICInitStructure;

    /* TIM4 clock enable*/
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
    RCC_APB1PeriphResetCmd(RCC_APB1Periph_TIM4, DISABLE);

    TIM_TimeBaseStructure.TIM_Prescaler = 4U;           /* Tau = 1/(Fslc / (1 + Prescaler )) = 1 / (72000000 / (1 + 4)) = 69.4nS */
    TIM_TimeBaseStructure.TIM_Period = 0xFFFFU;
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);

    TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;
    TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
    TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
    TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
    TIM_ICInitStructure.TIM_ICFilter = 0x00U;
    TIM_ICInit(TIM4, &TIM_ICInitStructure);

    /* Channel 2 Config*/
    TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;
    TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Falling;
    TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_IndirectTI;
    TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
    TIM_ICInitStructure.TIM_ICFilter = 0x00U;
    TIM_ICInit(TIM4, &TIM_ICInitStructure);

    TIM_ClearITPendingBit(TIM4, TIM_IT_CC1 | TIM_IT_CC2 | TIM_IT_Update);
    TIM_ITConfig(TIM4, TIM_IT_CC1, ENABLE);
    TIM_ITConfig(TIM4, TIM_IT_CC2, ENABLE);
    TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);

    /*More*/
    TIM_SelectInputTrigger(TIM4,TIM_TS_TI1FP1);
    TIM_SelectSlaveMode(TIM4, TIM_SlaveMode_Reset);

    /* TIM4 enable counter */
    /* TIM_Cmd(TIM4, ENABLE); */ /* Should be start by start function. */
}
#endif

/* This function should initialiseTIM2 CH1 DMA CH5. */
static void board_capture_dma1_ch5_init(void)
{
    DMA_InitTypeDef DMA_InitStructure;

    /* DMA module clk ON. */
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

    /* DMA reset. */
    DMA_DeInit(DMA1_Channel5);

    /* Fill DMA init structure before initialisation. */
    DMA_InitStructure.DMA_PeripheralBaseAddr    = (uint32_t)(&TIM2->CCR1);
    DMA_InitStructure.DMA_MemoryBaseAddr        = (uint32_t)(&u16_board_capture_period_value);
    DMA_InitStructure.DMA_DIR                   = DMA_DIR_PeripheralSRC;
    DMA_InitStructure.DMA_BufferSize            = 1U;
    DMA_InitStructure.DMA_PeripheralInc         = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc             = DMA_MemoryInc_Disable;
    DMA_InitStructure.DMA_PeripheralDataSize    = DMA_PeripheralDataSize_HalfWord;
    DMA_InitStructure.DMA_MemoryDataSize        = DMA_MemoryDataSize_HalfWord;
    DMA_InitStructure.DMA_Mode                  = DMA_Mode_Circular;
    DMA_InitStructure.DMA_Priority              = DMA_Priority_Low;
    DMA_InitStructure.DMA_M2M                   = DMA_M2M_Disable;

    /* Initialisation of DMA UART TX. */
    DMA_Init(DMA1_Channel5, &DMA_InitStructure);

    /* TIM2 ->  DMA1 CH5 enable. */
    TIM_DMACmd(TIM2, TIM_DMA_CC1, ENABLE);

    /* Start DMA transmitting. */
    DMA_Cmd(DMA1_Channel5, ENABLE);
}

/* This function should initialiseTIM2 CH2 DMA CH7. */
static void board_capture_dma1_ch7_init(void)
{
    DMA_InitTypeDef DMA_InitStructure;

    /* DMA module clk ON. */
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

    /* DMA reset. */
    DMA_DeInit(DMA1_Channel7);

    /* Fill DMA init structure before initialisation. */
    DMA_InitStructure.DMA_PeripheralBaseAddr    = (uint32_t)(&TIM2->CCR2);
    DMA_InitStructure.DMA_MemoryBaseAddr        = (uint32_t)(&u16_board_capture_duty_value);
    DMA_InitStructure.DMA_DIR                   = DMA_DIR_PeripheralSRC;
    DMA_InitStructure.DMA_BufferSize            = 1U;
    DMA_InitStructure.DMA_PeripheralInc         = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc             = DMA_MemoryInc_Disable;
    DMA_InitStructure.DMA_PeripheralDataSize    = DMA_PeripheralDataSize_HalfWord;
    DMA_InitStructure.DMA_MemoryDataSize        = DMA_MemoryDataSize_HalfWord;
    DMA_InitStructure.DMA_Mode                  = DMA_Mode_Circular;
    DMA_InitStructure.DMA_Priority              = DMA_Priority_Low;
    DMA_InitStructure.DMA_M2M                   = DMA_M2M_Disable;

    /* Initialisation of DMA UART TX. */
    DMA_Init(DMA1_Channel7, &DMA_InitStructure);

    /* TIM2 ->  DMA1 CH7 enable. */
    TIM_DMACmd(TIM2, TIM_DMA_CC2, ENABLE);

    /* Start DMA transmitting. */
    DMA_Cmd(DMA1_Channel7, ENABLE);
}








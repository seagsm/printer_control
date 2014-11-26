
#include "board_capture.h"

static PWM_CAPTURE_STATE board_capture_command = PWM_CAPTURE_STOP;


static uint16_t reg_h           = 0U;   /* High Register for T2 */
static uint16_t reg_l           = 0U;   /* Low Regiser for T2 (For Input Capture)*/
static uint8_t T2_Overrun       = 0U;
static uint8_t update_per       = 0U;   /* Flag to update period, set by T2 Input Capture */


PWM_CAPTURE_STATE board_capture_get_pwm_command(void)
{
    return(board_capture_command);
}

void board_capture_set_pwm_command(PWM_CAPTURE_STATE command)
{
    board_capture_command = command;
}

void board_capture_pwm_TIM_2_start(void)
{

}

void board_capture_pwm_TIM_4_start(void)
{

}

void board_capture_pwm_TIM_2_stop(void)
{

}

void board_capture_pwm_TIM_4_stop(void)
{

}

BOARD_ERROR be_board_capture_pwm_init(void)
{
    BOARD_ERROR be_result = BOARD_ERR_OK;

    GPIO_Configuration();
    NVIC_Configuration();
    TIM2_Configuration();

    return(be_result);

}


void test(void)
{
     while(1)
    {
        if(update_per == 1U)
        {
            update_per = 0U;
        }
    
    }  
}




void TIM2_IRQHandler(void)
{
 
    if(TIM_GetITStatus(TIM2, TIM_IT_CC1) == SET)            /* If compare capture has occured */
    {
        TIM_ClearITPendingBit(TIM2, TIM_IT_CC1);
        reg_l = TIM_GetCapture1(TIM2);                      /* Get timer counts for Period */
        update_per = 1U;                                       /* Set Flag to update period */
    }
 
    if(TIM_GetITStatus(TIM2, TIM_IT_CC2) == SET)            /* If compare capture has occured */
    {
        TIM_ClearITPendingBit(TIM2, TIM_IT_CC2);
        reg_h= TIM_GetCapture2(TIM2);

    }
 
    if(TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
    {
         TIM_ClearITPendingBit(TIM2, TIM_IT_Update);    /* Counter overflow, reset interrupt */
         if( T2_Overrun < 127U)
         {
           T2_Overrun++;
         }
    }
 
}





static void NVIC_Configuration(void)
{
        NVIC_InitTypeDef NVIC_InitStructure;
 
       /* enable tim2 irq */
        NVIC_InitStructure.NVIC_IRQChannel = (unsigned char)TIM2_IRQn;
        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = TIMER2_PERIOD_INTERUPT_PRIORITY_GROUP;
        NVIC_InitStructure.NVIC_IRQChannelSubPriority = TIMER2_PERIOD_INTERUPT_SUB_PRIORITY_GROUP;
        NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
        NVIC_Init(&NVIC_InitStructure);
}






/* Input capture GPIO init */
static void GPIO_Configuration(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
 
    /* GPIOA GPIOB and GPIOC clock enable */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE);
        
    GPIO_InitStructure.GPIO_Pin         =  GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode        = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Speed       = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
 
}

/* Set Timer 2 Interrupt (Input Pulse Capture) */
static void TIM2_Configuration(void)
{
    /* Time base configuration */
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_ICInitTypeDef  TIM_ICInitStructure;
 
    /* TIM2 clock enable*/
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    RCC_APB1PeriphResetCmd(RCC_APB1Periph_TIM2, DISABLE);
    
    TIM_TimeBaseStructure.TIM_Prescaler = 4U;
    TIM_TimeBaseStructure.TIM_Period = 60000U;
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
 
    TIM_ITConfig(TIM2, TIM_IT_CC1, ENABLE);
    TIM_ITConfig(TIM2, TIM_IT_CC2, ENABLE);
    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
 
    /*More*/
    TIM_SelectInputTrigger(TIM2,TIM_TS_TI1FP1);
    TIM_SelectSlaveMode(TIM2, TIM_SlaveMode_Reset);
 
    /* TIM2 enable counter */
    TIM_Cmd(TIM2, ENABLE);
}








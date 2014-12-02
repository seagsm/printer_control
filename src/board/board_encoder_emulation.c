

#include "board_encoder_emulation.h"


static uint16_t u16_target_period = 0U;


/* Board encoder emulation init. */
BOARD_ERROR board_encoder_emulation_init(void)
{
    BOARD_ERROR be_result = BOARD_ERR_OK;
    
    be_result = board_encoder_emulation_timer_init();
    
    return(be_result);
}

/* Start encoder emulation timer and interrupt. */
void board_encoder_emulation_start(void)
{
    /* Set start period. */
    board_encoder_emulation_set_period(ZERO_SPEED_PERIOD);

    /* Enable counter. */
    TIM_Cmd(TIM1, ENABLE);
    /* Enable update interrupt. */
    NVIC_EnableIRQ(TIM1_UP_IRQn);
}

/* Stop encoder emulation timer and interrupt. */
void board_encoder_emulation_stop(void)
{
    TIM_Cmd(TIM1, DISABLE);
    NVIC_DisableIRQ(TIM1_UP_IRQn);
}                

/* Set target value of encoder period. */
void board_encoder_emulation_set_target_period(uint16_t u16_period)
{
    u16_target_period = u16_period;
}

/* Set current timer period. */
static void board_encoder_emulation_set_period(uint16_t u16_period)
{
    TIM1->ARR = u16_period;
}

/* Timer 1 update interrupt hundler. */
void TIM1_UP_IRQHandler(void)
{
    if(TIM_GetITStatus(TIM1, TIM_IT_Update) != RESET)
    {
         TIM_ClearITPendingBit(TIM1, TIM_IT_Update);            /* Counter overflow, reset interrupt */
         /* board_encoder_emulation_proccess(); */
         board_encoder_emulation_float_proccess();
    }
}

/* For test only. */
static void _test_board_encoder_emulation_proccess(void)
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

/* Integer calculation of PID approaching. */
static void board_encoder_emulation_proccess(void)
{
    uint16_t u16_current_period = 0U;
    
    int32_t i32_current_period  = 0;
    int32_t i32_target_period   = 0;
    int32_t i32_new_period      = 0;
    
    
    u16_current_period = TIM1->ARR;

    i32_current_period = (int32_t)u16_current_period;
    i32_target_period = (int32_t)u16_target_period;
    
    i32_new_period = i32_current_period + ((i32_target_period - i32_current_period)/PID_PROPORTIONAL);
    
    u16_current_period = (uint16_t)i32_new_period;

    board_encoder_emulation_set_period(u16_current_period);
}

/* Float calculation of PID approaching. */
static void board_encoder_emulation_float_proccess(void)
{
    uint16_t u16_current_period = 0U;
    float f32_current_period    = (float)0;
    float f32_target_period     = (float)0;
    float f32_new_period        = (float)0;
    
    PWM_CAPTURE_STATE pcs_state;
 
    /* Get current channel of PWM capture. CW or CCW or Stop. */
    pcs_state = board_capture_get_pwm_command();
     
    /* Read current encoder period. */
    u16_current_period = TIM1->ARR;

    f32_current_period = (float)u16_current_period;
 
    /* Calculation of first aproximation of DUTY -> ENCODER PERIOD */
    if(u16_board_capture_duty_value > 0U)
    {
        u16_target_period = 3000U/u16_board_capture_duty_value;
    }
    else
    {
        u16_target_period = 4000U;
    }  
    
    f32_target_period  = (float)u16_target_period;
    
    /* Calculate new approaching. */
    f32_new_period = f32_current_period + ((f32_target_period - f32_current_period)/((float)PID_PROPORTIONAL));
    
    u16_current_period = (uint16_t)f32_new_period;

    /* Set new period to encoder timer. */
    board_encoder_emulation_set_period(u16_current_period);
    
    /* Here is a place for generation of encoder signals. */
    if(pcs_state == PWM_CAPTURE_CW_START)
    {
        /* ++ */
        board_encoder_emulation_output(1);
    
    }
    else if(pcs_state == PWM_CAPTURE_CCW_START)
    {
        /* -- */
        board_encoder_emulation_output(-1);
    }
    else
    {

    }  
}

/* Initialisation of timer for encoder emulation. */
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

    /* Init Timer1 like timer. */
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



/* Input parametr is 1 or -1 . */
static void board_encoder_emulation_output(int8_t i8_printer_step)
{
    static int8_t i8_encoder_possition_counter = 0;

    /* TODO: Here shoub be called function board_motor_step(direction). */
    board_motor_step(i8_printer_step);

    /* Round encoder. */    
    i8_encoder_possition_counter = i8_encoder_possition_counter + i8_printer_step;
    if(i8_encoder_possition_counter > 3)
    {
        i8_encoder_possition_counter = 0;
    }
    else if(i8_encoder_possition_counter < 0)
    {
        i8_encoder_possition_counter = 3;
    }  
    else
    {
      
    }
    
    switch (i8_encoder_possition_counter)
    {
        case 0: 
            GPIO_SetBits(   GPIOB, GPIO_B_OUT_ENCODER_A);
            GPIO_SetBits(   GPIOB, GPIO_B_OUT_ENCODER_B);
            break;
        case 1: 
            GPIO_SetBits(   GPIOB, GPIO_B_OUT_ENCODER_A);
            GPIO_ResetBits( GPIOB, GPIO_B_OUT_ENCODER_B);
            break;
        case 2: 
            GPIO_ResetBits( GPIOB, GPIO_B_OUT_ENCODER_A);
            GPIO_ResetBits( GPIOB, GPIO_B_OUT_ENCODER_B);
            break;
        case 3: 
            GPIO_ResetBits( GPIOB, GPIO_B_OUT_ENCODER_A);
            GPIO_SetBits(   GPIOB, GPIO_B_OUT_ENCODER_B);
            break;
        default:
            break;
    }
}










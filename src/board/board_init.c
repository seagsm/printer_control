#include "board_init.h"

/*
  Board init function.
*/
BOARD_ERROR be_board_init_main_init(void)
{
    uint16_t u16_step = 0U;
    BOARD_ERROR be_result = BOARD_ERR_OK;
#if 0
    u16_board_adc_result[0] = 0U;/* It is temporery solution. */
#endif
    gv_board_sys_tick_init();
    /* Init interrupt priority group. */
    NVIC_init();
    /*TODO: It should be moved to suitable place. */
    __enable_irq();


    while (u16_step < BOARD_INIT_TOTAL_STEPS)
    {
        switch (u16_step)
		{
            case (0U):
                be_result = be_board_gpio_init();   /* Init GPIO. */
                break;
            case (1U):
               /* be_result = be_board_uart_init(); */  /* Init UART modules. */
                break;
            case (2U):
                /*  be_result = be_board_pwm_init(); */    /* Init PWM chanels. */
                /* Should be initialized by be_board_motor_init() based on TIM3 */
                be_result = board_motor_init();  /*            */
                break;
            case (3U):
                be_result = board_encoder_emulation_init();
                break;
            case (4U):
                be_result = be_board_capture_pwm_init();
                break;
            case (5U):
                be_result = board_spi_1_dma_slave_configuration();
                test();
                break;
            case (6U):
                be_result = BOARD_ERR_OK;
                break;

            default:
                be_result = BOARD_ERR_ERROR;
                break;
        }
        /* If during initialisation something going wrong. */
        if(be_result != BOARD_ERR_OK)
        {
            /* TODO: Print u16_step like number of error step. */
            break;
        }
        u16_step++;
    }

    if(be_result == BOARD_ERR_OK)
    {
        v_board_stateSetRequiredState(BOARD_SYSTEM_READY_TO_RUN);
    }
    else
    {
        v_board_stateSetRequiredState(BOARD_SYSTEM_FAULT);
    }
    return(be_result);
}
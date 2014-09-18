
/* Code style rules:                */
/* 1. Code should be an ideal.      */
/* 2. An ideal code does not exist. */
/*                 Vadym Volokitin. */

/* Inc printer stepper motor driver controller for Epson.*/

#include "main.h"


#include <stm32f10x.h>
#include <stm32f10x_gpio.h>
#include <stm32f10x_rcc.h>
#include <stm32f10x_tim.h>
#include <misc.h>

   static                 uint8_t u8_buff_value[0x06];

int main( void)
{
    BOARD_SYSTEM_STATE  bss_state;
    BOARD_ERROR         be_result = BOARD_ERR_OK;

    v_board_state_set_required_state(BOARD_SYSTEM_INIT);
    /*TODO:
                    Should be done calibration of PPM input for current minimum
                    and maximum value for each channel.
     */

    while(1U)
    {
        if(be_result == BOARD_ERR_ERROR)
        {
            v_board_state_set_required_state(BOARD_SYSTEM_FAULT);
        }
        bss_state = bss_board_state_get_required_state();
        switch (bss_state)
        {
            case BOARD_SYSTEM_INIT:
                v_board_state_update_current_state(BOARD_SYSTEM_INIT);
                be_result = be_board_init_main_init();      /* init main hardware moduls.*/
                break;
            case BOARD_SYSTEM_READY_TO_RUN:
                v_board_state_update_current_state(BOARD_SYSTEM_READY_TO_RUN);
#if 0
                be_result = be_board_system_init_unlock();  /* init all board system .*/
#endif
                /* Should be removed. Real setup inside  be_board_system_init_unlock() */
                v_board_state_set_required_state(BOARD_SYSTEM_RUN);

                break;

            case BOARD_SYSTEM_RUN:
                /* One time switch a board to RUN state. */
                if(bss_board_state_get_current_state() != BOARD_SYSTEM_RUN )
                {
                    /* Start main loop interrupt. */
                    be_api_main_loop_start();
                    v_board_state_update_current_state(BOARD_SYSTEM_RUN);
                }

                {
                    static uint16_t u16_value = 0U;
                    uint8_t u8_byte_counter = 0U;

                    SPI_I2S_SendData(SPI1, (uint16_t)u16_value);
                    u16_value++;

/*
0x30 0
0x31 1
0x32 2
0x33 3
0x34 4
0x35 5
0x36 6
0x37 7
0x38 8
0x39 9
0x41 A
0x42 B
0x43 C
0x44 D
0x45 E
0x46 F
 */
                    u8_buff_value[3] = (uint8_t)(u16_value & 0x000FU);
                    u8_buff_value[2] = (uint8_t)((u16_value & 0x00F0U)>> 4 );
                    u8_buff_value[1] = (uint8_t)((u16_value & 0x0F00U)>> 8 );
                    u8_buff_value[0] = (uint8_t)((u16_value & 0xF000U)>> 12);
                    u8_buff_value[4] = 0x0AU;
                    u8_buff_value[5] = 0x0DU;

                    for (u8_byte_counter = 0U; u8_byte_counter < 0x04U; u8_byte_counter++)
                    {
                        switch(u8_buff_value[u8_byte_counter])
                        {
                            case 0x00U:
                                u8_buff_value[u8_byte_counter] = 0x30U;
                            break;
                            case 0x01U:
                                u8_buff_value[u8_byte_counter] = 0x31U;
                            break;
                            case 0x02U:
                                u8_buff_value[u8_byte_counter] = 0x32U;
                            break;
                            case 0x03U:
                                u8_buff_value[u8_byte_counter] = 0x33U;
                            break;
                            case 0x04U:
                                u8_buff_value[u8_byte_counter] = 0x34U;
                            break;
                            case 0x05U:
                                u8_buff_value[u8_byte_counter] = 0x35U;
                            break;
                            case 0x06U:
                                u8_buff_value[u8_byte_counter] = 0x36U;
                            break;
                            case 0x07U:
                                u8_buff_value[u8_byte_counter] = 0x37U;
                            break;
                            case 0x08U:
                                u8_buff_value[u8_byte_counter] = 0x38U;
                            break;
                            case 0x09U:
                                u8_buff_value[u8_byte_counter] = 0x39U;
                            break;
                            case 0x0AU:
                                u8_buff_value[u8_byte_counter] = 0x41U;
                            break;
                            case 0x0BU:
                                u8_buff_value[u8_byte_counter] = 0x42U;
                            break;
                            case 0x0CU:
                                u8_buff_value[u8_byte_counter] = 0x43U;
                            break;
                            case 0x0DU:
                                u8_buff_value[u8_byte_counter] = 0x44U;
                            break;
                            case 0x0EU:
                                u8_buff_value[u8_byte_counter] = 0x45U;
                            break;
                            case 0x0FU:
                                u8_buff_value[u8_byte_counter] = 0x46U;
                            break;
                            default:
                                break;
                        }
                    }
                    board_dma_send_print_buffer(u8_buff_value, 0x06U);

                }
                SPI_I2S_ITConfig(SPI1, SPI_I2S_IT_TXE, ENABLE);


                gv_board_sys_tick_delay(100U);
                /* timer2_PWM_duty_CH1(bc_channel_value_structure.u16_channel_1_value); *//* send data to servo. */

#if 0
                GPIO_SetBits( GPIOA, GPIO_Pin_12);
                gv_board_sys_tick_fast_delay(50U);
                GPIO_ResetBits( GPIOA, GPIO_Pin_12);
#endif
                /* Led. */
                GPIO_SetBits( GPIOB, GPIO_Pin_1);
                gv_board_sys_tick_fast_delay(50U);
                GPIO_ResetBits( GPIOB, GPIO_Pin_1);

            /*    gv_board_dma_send_packet(); */
            /*    board_dma_send_buff(); */

                GPIO_SetBits( GPIOB, GPIO_Pin_1);
                gv_board_sys_tick_fast_delay(50U);
                GPIO_ResetBits( GPIOB, GPIO_Pin_1);
                break;
            case BOARD_SYSTEM_MOTOR_CALIBRATION:/* Calibration of motor ESD controller.*/
                v_board_state_update_current_state(BOARD_SYSTEM_MOTOR_CALIBRATION);
                be_result = be_board_system_init_motor_calibration();
                break;
            case BOARD_SYSTEM_FAULT:
                v_board_state_update_current_state(BOARD_SYSTEM_FAULT);
                GPIO_SetBits( GPIOB, GPIO_Pin_1);
                gv_board_sys_tick_delay(400U);
                GPIO_ResetBits( GPIOB, GPIO_Pin_1);
                gv_board_sys_tick_delay(400U);
                break;
            default:
                break;
        }
    }
}












#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
uint32_t assert_failed(uint8_t* file, uint32_t line)
{
    /*  User can add his own implementation to report the file name and line number,
        ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

    /* Infinite loop */
    while (1U)
    {
        return(1U);
    }
}
#endif


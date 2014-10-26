


#include "board_agp.h"

static BOARD_AGP_DATA board_agp_data_log[100];



BOARD_ERROR be_board_agp_init(void)
{
    BOARD_ERROR be_result = BOARD_ERR_OK;


    return(be_result);
}

BOARD_ERROR be_board_agp_get(uint8_t u8_dir)
{
    BOARD_ERROR be_result = BOARD_ERR_OK;
    static int32_t i32_step_counter = 0;
    static int32_t i32_minus_step_counter = 0;
    static uint8_t  u8_agp_last_state = 1U;
    static uint8_t  u8_agp_index = 0U;

    uint8_t  u8_agp_new_state;

    if(u8_dir == 0U)
    {
        i32_step_counter++;
    }
    else
    {
        i32_minus_step_counter++;
    }

    /* Checking and setting AGP sensor possition. */
    u8_agp_new_state = GPIO_ReadInputDataBit(GPIOC, GPIO_C_IN_APG_SENSOR);
    if(u8_agp_new_state == 0U)
    {
        GPIO_SetBits( GPIOB, GPIO_Pin_1);
    }
    else
    {
        GPIO_ResetBits( GPIOB, GPIO_Pin_1);
    }

    if(u8_agp_new_state != u8_agp_last_state)
    {
        board_agp_data_log[u8_agp_index].i32_step = i32_step_counter;
        board_agp_data_log[u8_agp_index].i32_minus_step = i32_minus_step_counter;
        board_agp_data_log[u8_agp_index].u8_state = u8_agp_new_state;

        i32_step_counter = 0;
        i32_minus_step_counter = 0;

        u8_agp_index++;
    }
    u8_agp_last_state = u8_agp_new_state;


    return(be_result);
}


BOARD_ERROR be_board_agp_set(uint8_t u8_dir)
{
    BOARD_ERROR be_result = BOARD_ERR_OK;
    static int32_t i32_step_counter;
    static uint8_t  u8_agp_last_state = 1U;
    static uint8_t  u8_agp_index = 0U;
    int32_t i32_agp_position[16] ={
        8838, 4678, 2581, 5735, 2684, 10155, 2638, 16950,
        10653, 5662, 2607, 5248, 2700, 9704, 9817, 17368
                                   };
    if(u8_dir == 0U)
    {
        i32_step_counter++;
    }
    else
    {
        i32_step_counter--;
    }

    if(i32_step_counter >= i32_agp_position[u8_agp_index])
    {
        i32_step_counter = 0;
        u8_agp_index++;
        if(u8_agp_index >= 16U)
        {
            u8_agp_index = 0U;
        }

        if(u8_agp_last_state == 1U)
        {
            u8_agp_last_state = 0U;
            GPIO_ResetBits(GPIOC, GPIO_C_IN_APG_SENSOR);
            GPIO_ResetBits( GPIOB, GPIO_Pin_1);
        }
        else
        {
            u8_agp_last_state = 1U;
            GPIO_SetBits(GPIOC, GPIO_C_IN_APG_SENSOR);
            GPIO_SetBits( GPIOB, GPIO_Pin_1);
        }
    }


    return(be_result);
}








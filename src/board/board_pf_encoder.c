
#include "board_pf_encoder.h"

BOARD_ERROR be_board_pf_encoder_set(uint8_t u8_dir)
{
    BOARD_ERROR be_result = BOARD_ERR_OK;

    static int8_t i8_index = 0;

    if(u8_dir == 0U)
    {
        i8_index++;
    }
    else
    {
        i8_index--;
    }

    if(i8_index > 3)
    {
        i8_index = 0;
    }
    if(i8_index < 0)
    {
        i8_index = 3;
    }
    switch(i8_index)
    {
      case 0:
        GPIO_ResetBits(GPIOB, GPIO_B_OUT_ENCODER_A );
        GPIO_ResetBits(GPIOB, GPIO_B_OUT_ENCODER_B );
        break;
      case 1:
        GPIO_SetBits(  GPIOB, GPIO_B_OUT_ENCODER_A );
        GPIO_ResetBits(GPIOB, GPIO_B_OUT_ENCODER_B );
        break;
      case 2:
        GPIO_SetBits(  GPIOB, GPIO_B_OUT_ENCODER_A );
        GPIO_SetBits(  GPIOB, GPIO_B_OUT_ENCODER_B );
        break;
      case 3:
        GPIO_ResetBits(GPIOB, GPIO_B_OUT_ENCODER_A );
        GPIO_SetBits(  GPIOB, GPIO_B_OUT_ENCODER_B );
        break;
      default:
        break;
    }

    return(be_result);
}

BOARD_ERROR be_board_pf_encoder_pulse(void)
{
    BOARD_ERROR be_result = BOARD_ERR_OK;

    static int8_t i8_index = 0;

    i8_index++;


    if(i8_index > 1)
    {
        i8_index = 0;
    }
    if(i8_index < 0)
    {
        i8_index = 1;
    }
    switch(i8_index)
    {
      case 0:
        GPIO_ResetBits(GPIOB, GPIO_B_OUT_ENCODER_A );
        GPIO_ResetBits(GPIOB, GPIO_B_OUT_ENCODER_B );
        break;
      case 1:
        GPIO_SetBits(  GPIOB, GPIO_B_OUT_ENCODER_A );
        GPIO_SetBits(  GPIOB, GPIO_B_OUT_ENCODER_B );
        break;
      default:
        break;
    }
    return(be_result);
}

BOARD_ERROR be_board_pf_encoder_level(uint8_t u8_dir)
{
    BOARD_ERROR be_result = BOARD_ERR_OK;

    switch(u8_dir)
    {
      case 0U:
        GPIO_ResetBits(GPIOB, GPIO_B_OUT_ENCODER_A );
        GPIO_ResetBits(GPIOB, GPIO_B_OUT_ENCODER_B );
        break;
      case 1U:
        GPIO_SetBits(  GPIOB, GPIO_B_OUT_ENCODER_A );
        GPIO_SetBits(  GPIOB, GPIO_B_OUT_ENCODER_B );
        break;
      default:
        break;
    }
    return(be_result);
}









#ifndef BOARD_PF_ENCODER_H
#define BOARD_PF_ENCODER_H 1

#include "stm32f10x.h"
#include "board_system_type.h"
#include "board_gpio_table.h"

BOARD_ERROR be_board_pf_encoder_set(uint8_t u8_dir);
BOARD_ERROR be_board_pf_encoder_pulse(void);
BOARD_ERROR be_board_pf_encoder_level(uint8_t u8_dir);

#endif
#ifndef BOARD_AGP_H
#define BOARD_AGP_H 1

#include "stm32f10x.h"
#include "board_system_type.h"
#include "board_gpio_table.h"

BOARD_ERROR be_board_agp_init(void);
BOARD_ERROR be_board_agp_set(uint8_t u8_dir);
BOARD_ERROR be_board_agp_get(uint8_t u8_dir);


#endif
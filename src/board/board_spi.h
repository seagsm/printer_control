#ifndef BOARD_SPI_H
#define BOARD_SPI_H 1



#include "stm32f10x.h"
#include "board_system_type.h"
#include "board_gpio.h"
#include "board_sys_tick.h"


#define BOARD_SPI_A_SCK_PIN     GPIO_Pin_5
#define BOARD_SPI_A_SCK_PORT  GPIOA

#define BOARD_SPI_A_MISO_PIN    GPIO_Pin_6
#define BOARD_SPI_A_MISO_PORT   GPIOA

#define BOARD_SPI_A_MOSI_PIN    GPIO_Pin_7
#define BOARD_SPI_A_MOSI_PORT   GPIOA




       BOARD_ERROR be_board_spi_init(void);
static BOARD_ERROR be_board_spi_a_gpio_init(void);
static BOARD_ERROR be_board_spi_a_module_init(void);


#endif
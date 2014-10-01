#ifndef BOARD_SPI_H
#define BOARD_SPI_H 1



#include "stm32f10x.h"
#include "board_system_type.h"
#include "board_gpio.h"
#include "board_sys_tick.h"
#include "board_dma.h"
#include "board_timer.h"


/* SPI_A pin definition. */
#define BOARD_SPI_A_SCK_PIN     GPIO_Pin_5
#define BOARD_SPI_A_SCK_PORT    GPIOA

#define BOARD_SPI_A_MISO_PIN    GPIO_Pin_6
#define BOARD_SPI_A_MISO_PORT   GPIOA

#define BOARD_SPI_A_MOSI_PIN    GPIO_Pin_7
#define BOARD_SPI_A_MOSI_PORT   GPIOA

/* SPI_B pin definition. */
#define BOARD_SPI_B_SCK_PIN     GPIO_Pin_13
#define BOARD_SPI_B_SCK_PORT    GPIOB

#define BOARD_SPI_B_MISO_PIN    GPIO_Pin_14
#define BOARD_SPI_B_MISO_PORT   GPIOB

#define BOARD_SPI_B_MOSI_PIN    GPIO_Pin_15
#define BOARD_SPI_B_MOSI_PORT   GPIOB







       BOARD_ERROR be_board_spi_init(void);
static BOARD_ERROR be_board_spi_a_gpio_init(void);
static BOARD_ERROR be_board_spi_a_module_init(void);

static BOARD_ERROR be_board_spi_b_gpio_init(void);
static BOARD_ERROR be_board_spi_b_module_init(void);

void   SPI1_IRQHandler(void);
void   SPI2_IRQHandler(void);



#endif
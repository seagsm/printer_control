#ifndef BOARD_SPI_DMA_H
#define BOARD_SPI_DMA_H 1


#include "stm32f10x.h"
#include "board_system_type.h"
#include "board_gpio.h"
#include "board_sys_tick.h"
#include "board_dma.h"
#include "board_timer.h"


/* SPI_A pin definition. */
#define BOARD_SPI_1_SCK_PIN     GPIO_Pin_5
#define BOARD_SPI_1_SCK_PORT    GPIOA

#define BOARD_SPI_1_MISO_PIN    GPIO_Pin_6
#define BOARD_SPI_1_MISO_PORT   GPIOA

#define BOARD_SPI_1_MOSI_PIN    GPIO_Pin_7
#define BOARD_SPI_1_MOSI_PORT   GPIOA

/* SPI_B pin definition. */
#define BOARD_SPI_2_SCK_PIN     GPIO_Pin_13
#define BOARD_SPI_2_SCK_PORT    GPIOB

#define BOARD_SPI_2_MISO_PIN    GPIO_Pin_14
#define BOARD_SPI_2_MISO_PORT   GPIOB

#define BOARD_SPI_2_MOSI_PIN    GPIO_Pin_15
#define BOARD_SPI_2_MOSI_PORT   GPIOB

BOARD_ERROR board_spi_1_dma_slave_configuration(void);

void DMA1_Channel2_IRQHandler(void);
void DMA1_Channel3_IRQHandler(void);

#endif
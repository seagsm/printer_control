#ifndef BOARD_DMA_H
#define BOARD_DMA_H 1



#include "stm32f10x.h"
#include "board_system_type.h"
#include "board_r_buff.h"
#include "board_sys_tick.h"
#include "board_sys_tick.h"


/* This is size of packet that will be send by DMA TX. */
#define DMA1_CH4_TX_SIZE 16U

/* This is size of packet that will be received by DMA RX. */
#define DMA1_CH5_RX_SIZE 128U

/*
    This is maximun size of data packet that can be sent for one time.
    I think it is good idea to have same size like DMA TX round buffer.
*/
#define USART_TX_DATA_PACKET_SIZE   TX_USART1_SIZE
#define USART_RX_DATA_PACKET_SIZE   RX_USART1_SIZE

/* extern uint8_t u8_tx_data_packet[USART_TX_DATA_PACKET_SIZE];*/


BOARD_ERROR be_board_dma1_ch4_init(void);
BOARD_ERROR be_board_dma1_ch5_init(void);

void DMA1_Channel4_IRQHandler(void);
void DMA1_Channel5_IRQHandler(void);

static uint16_t u16_board_dma_DMA1_CH5_byte_received(void);
static void sv_board_dma_send_packet(uint16_t u16_size_of_tx_data);

BOARD_ERROR be_board_dma_DMA1_CH5_buffer_copy_to_UART1_buffer(void);

#endif
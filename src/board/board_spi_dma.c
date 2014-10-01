

#include "board_spi_dma.h"

static uint16_t SPIReceivedValue[2];
static uint16_t SPITransmittedValue[2] = {0xFF00U,0x00FFU};

BOARD_ERROR board_spi_1_dma_slave_configuration(void)
{
    BOARD_ERROR be_result = BOARD_ERR_OK;

    SPI_InitTypeDef     SPI_InitStructure;  /* Variable used to setup the SPI       */
    DMA_InitTypeDef     DMA_InitStructure;  /* Variable used to setup the DMA       */
    NVIC_InitTypeDef    NVIC_InitStructure; /* Variable used to setup the Interrupt */

    /* SPI 1 GPIO config. */
    be_board_pin_init( GPIOA,      GPIO_Pin_7,     GPIO_Speed_50MHz,   GPIO_Mode_AF_PP);     /* MOSI_PIN */
    be_board_pin_init( GPIOA,      GPIO_Pin_6,     GPIO_Speed_50MHz,   GPIO_Mode_AF_PP);     /* MISO_PIN */
    be_board_pin_init( GPIOA,      GPIO_Pin_5,     GPIO_Speed_50MHz,   GPIO_Mode_AF_PP);     /* SCK_PIN */


    /*--Enable the SPI1 periph */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);

    /* Reset SPI Interface */
    SPI_I2S_DeInit(SPI1);

    /* SPI1 configuration */
    SPI_StructInit(&SPI_InitStructure);
    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
    SPI_InitStructure.SPI_Mode = SPI_Mode_Slave;
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_16b;
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
    SPI_InitStructure.SPI_CRCPolynomial = 7U;
    SPI_Init(SPI1, &SPI_InitStructure);

    /* Enable DMA1 channel IRQ Channel */
    NVIC_InitStructure.NVIC_IRQChannel = (unsigned char)DMA1_Channel2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = DMA1_Channel2_PRIORITY_GROUP;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = DMA1_Channel2_SUB_PRIORITY_GROUP;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    /*--Enable DMA1 clock--*/
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

    /*==Configure DMA1 - Channel2== (SPI -> memory)*/
    DMA_DeInit(DMA1_Channel2); /*//Set DMA registers to default values */
    DMA_StructInit(&DMA_InitStructure);
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&SPI1->DR; /*//Address of peripheral the DMA must map to */
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&SPIReceivedValue[0]; /*//Variable to which received data will be stored */
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
    DMA_InitStructure.DMA_BufferSize = 2U; /*//Buffer size */
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    DMA_Init(DMA1_Channel2, &DMA_InitStructure); /*//Initialise the DMA */

    DMA_ITConfig(DMA1_Channel2, DMA_IT_TC, ENABLE);

    DMA_Cmd(DMA1_Channel2, ENABLE); /*//Enable the DMA1 - Channel2 */

    /*//==Configure DMA1 - Channel3== (memory -> SPI)*/
    DMA_DeInit(DMA1_Channel3); /*//Set DMA registers to default values */
    DMA_StructInit(&DMA_InitStructure);
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&SPI1->DR; /*//Address of peripheral the DMA must map to */
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&SPITransmittedValue[0]; /*//Variable from which data will be transmitted */
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
    DMA_InitStructure.DMA_BufferSize = 2U; /*//Buffer size */
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    DMA_Init(DMA1_Channel3, &DMA_InitStructure); /*//Initialise the DMA */

    DMA_ITConfig(DMA1_Channel3, DMA_IT_TC, ENABLE);

    DMA_Cmd(DMA1_Channel3, ENABLE); /*//Enable the DMA1 - Channel5 */

    /* Enable SPI2 */
    SPI_Cmd(SPI1, ENABLE);

    /* Enable the SPI1 RX & TX DMA requests */
    SPI_I2S_DMACmd(SPI1, SPI_I2S_DMAReq_Rx | SPI_I2S_DMAReq_Tx, ENABLE);

    NVIC_EnableIRQ(DMA1_Channel2_IRQn);

    return(be_result);
}


/* This codes found but not used.
          case 0xBDFDU :
          case 0xBFFBU :
          case 0xBFFDU :
*/
void DMA1_Channel2_IRQHandler(void)
{
    /* Reset DMA transfer complete interrupt.*/
    DMA1->IFCR |= DMA_ISR_TCIF2;
    if( SPIReceivedValue[0] == 0x7063U)
    {
        switch(SPIReceivedValue[1])
        {
          case 0xBDFFU : /* CW */
          case 0xBFFFU :
            GPIO_SetBits(GPIOB, GPIO_Pin_11);
            v_board_timer_set_step(1U);
            break;
          case 0xBDFBU : /* CCW */
            GPIO_ResetBits(GPIOB, GPIO_Pin_11);
            v_board_timer_set_step(1U);
            break;

          default:
            break;
        }
    }
}



void DMA1_Channel3_IRQHandler(void)
{

        NVIC_DisableIRQ(DMA1_Channel3_IRQn);

}



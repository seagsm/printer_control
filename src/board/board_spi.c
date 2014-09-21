


#include "board_spi.h"

BOARD_ERROR be_board_spi_init(void)
{
    BOARD_ERROR be_result = BOARD_ERR_OK;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
    SPI_I2S_DeInit(SPI1);
    be_result |= be_board_spi_a_gpio_init();
    be_result |= be_board_spi_a_module_init();

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
    SPI_I2S_DeInit(SPI2);
    be_result |= be_board_spi_b_gpio_init();
    be_result |= be_board_spi_b_module_init();

    return(be_result);
}

/* This function init SPI A interface. Master. */
static BOARD_ERROR be_board_spi_a_gpio_init(void)
{
    BOARD_ERROR be_result = BOARD_ERR_OK;

    be_result |= be_board_pin_init( GPIOA,      GPIO_Pin_7,     GPIO_Speed_50MHz,   GPIO_Mode_AF_PP);           /* MOSI_PIN */
    be_result |= be_board_pin_init( GPIOA,      GPIO_Pin_6,     GPIO_Speed_50MHz,   GPIO_Mode_IN_FLOATING);     /* MISO_PIN */
    be_result |= be_board_pin_init( GPIOA,      GPIO_Pin_5,     GPIO_Speed_50MHz,   GPIO_Mode_AF_PP);           /* SCK_PIN */

    return(be_result);
}

/* This function init SPI A module. */
static BOARD_ERROR be_board_spi_a_module_init(void)
{
    BOARD_ERROR be_result = BOARD_ERR_OK;

    SPI_InitTypeDef     SPI_InitStructure;
    NVIC_InitTypeDef    NVIC_InitStructure;

    /* SPI1 configuration */
    SPI_InitStructure.SPI_Direction     = SPI_Direction_2Lines_FullDuplex;
    SPI_InitStructure.SPI_Mode          = SPI_Mode_Master;
    SPI_InitStructure.SPI_DataSize      = SPI_DataSize_16b;
    SPI_InitStructure.SPI_CPOL          = SPI_CPOL_Low; /* SPI_CPOL_High; */
    SPI_InitStructure.SPI_CPHA          = SPI_CPHA_1Edge; /* SPI_CPHA_2Edge; */
    SPI_InitStructure.SPI_NSS           = SPI_NSS_Soft;
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_64;
    SPI_InitStructure.SPI_FirstBit      = SPI_FirstBit_MSB;
    SPI_InitStructure.SPI_CRCPolynomial = 7U;

    SPI_Init(SPI1, &SPI_InitStructure);

   /* Setup SPI A interrupt. */
    NVIC_InitStructure.NVIC_IRQChannel              = (unsigned char)SPI1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = SPI1_IRQn_PRIORITY_GROUP;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority   = SPI1_IRQn_SUB_PRIORITY_GROUP;
    NVIC_InitStructure.NVIC_IRQChannelCmd           = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    SPI_I2S_ClearITPendingBit(SPI1, SPI_I2S_IT_TXE);
    SPI_I2S_ITConfig(SPI1, SPI_I2S_IT_TXE, ENABLE);


    /* Turn On SPI A */
    SPI_Cmd(SPI1, ENABLE);

    /* SS = 1 */
    SPI_NSSInternalSoftwareConfig(SPI1, SPI_NSSInternalSoft_Set);

    return(be_result);
}

/* This function init SPI B interface. */
static BOARD_ERROR be_board_spi_b_gpio_init(void)
{
    BOARD_ERROR be_result = BOARD_ERR_OK;

    be_result |= be_board_pin_init( GPIOB,      GPIO_Pin_15,     GPIO_Speed_50MHz,   GPIO_Mode_AF_PP);    /* MOSI_PIN */
    be_result |= be_board_pin_init( GPIOB,      GPIO_Pin_14,     GPIO_Speed_50MHz,   GPIO_Mode_AF_PP);          /* MISO_PIN */
    be_result |= be_board_pin_init( GPIOB,      GPIO_Pin_13,     GPIO_Speed_50MHz,   GPIO_Mode_IN_FLOATING);    /* SCK_PIN */

    return(be_result);
}

/* This function init SPI B module. */
static BOARD_ERROR be_board_spi_b_module_init(void)
{
    BOARD_ERROR be_result = BOARD_ERR_OK;
    SPI_InitTypeDef     SPI_InitStructure;
    NVIC_InitTypeDef    NVIC_InitStructure;

    SPI_InitStructure.SPI_Direction     = SPI_Direction_2Lines_FullDuplex;
    SPI_InitStructure.SPI_Mode          = SPI_Mode_Slave;
    SPI_InitStructure.SPI_DataSize      = SPI_DataSize_16b;
    SPI_InitStructure.SPI_CPOL          = SPI_CPOL_Low; /* SPI_CPOL_High; */ /* SCK idle high */
    SPI_InitStructure.SPI_CPHA          = SPI_CPHA_1Edge; /* SPI_CPHA_2Edge; */ /* second transition -> SCK Idle high => capture on rising edge of clock */
    SPI_InitStructure.SPI_NSS           = SPI_NSS_Hard;
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_64;
    SPI_InitStructure.SPI_FirstBit      = SPI_FirstBit_MSB;

    SPI_Init(SPI2, &SPI_InitStructure);

    /* Setup SPI B interrupt. */
    NVIC_InitStructure.NVIC_IRQChannel              = (unsigned char)SPI2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = SPI2_IRQn_PRIORITY_GROUP;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority   = SPI2_IRQn_SUB_PRIORITY_GROUP;
    NVIC_InitStructure.NVIC_IRQChannelCmd           = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    /* Init RX interrupt. */
    SPI_I2S_ClearITPendingBit(SPI2, SPI_I2S_IT_RXNE);
    SPI_I2S_ITConfig(SPI2, SPI_I2S_IT_RXNE, ENABLE);

    /* Turn On SPI B */
    SPI_Cmd(SPI2, ENABLE);

    return(be_result);
}

void   SPI1_IRQHandler(void)
{
    if(SPI_I2S_GetITStatus(SPI1, SPI_I2S_IT_TXE)== SET )
    {
        /* To do something. */
        SPI_I2S_ClearITPendingBit(SPI1, SPI_I2S_IT_TXE);
        SPI_I2S_ITConfig(SPI1, SPI_I2S_IT_TXE, DISABLE);
    }

    if(SPI_I2S_GetITStatus(SPI1, SPI_I2S_IT_RXNE)== SET )
    {
        /* To do something. */
        SPI_I2S_ClearITPendingBit(SPI1, SPI_I2S_IT_RXNE);
    }
}

void   SPI2_IRQHandler(void)
{
    uint16_t u16_temp = 0U;
    if(SPI_I2S_GetITStatus(SPI2, SPI_I2S_IT_TXE)== SET )
    {
        /* To do something. */
        SPI_I2S_ClearITPendingBit(SPI2, SPI_I2S_IT_TXE);
        SPI_I2S_ITConfig(SPI2, SPI_I2S_IT_TXE, DISABLE);

    }

    if(SPI_I2S_GetITStatus(SPI2, SPI_I2S_IT_RXNE)== SET )
    {
        /* To do something. */
        u16_temp = SPI_I2S_ReceiveData(SPI2);
        board_dma_print_uint16_t(u16_temp);
        /* board_dma_print_uint16_t(0x0D0AU); */
        SPI_I2S_ClearITPendingBit(SPI2, SPI_I2S_IT_RXNE);
       /* SPI_I2S_ITConfig(SPI2, SPI_I2S_IT_RXNE, DISABLE);*/
    }
}




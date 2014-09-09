


#include "board_spi.h"

BOARD_ERROR be_board_spi_init(void)
{
    BOARD_ERROR be_result = BOARD_ERR_OK;

    be_result |= be_board_spi_a_gpio_init();
    be_result |= be_board_spi_a_module_init();

    return(be_result);
}

/* This function init SPI A interface. */
static BOARD_ERROR be_board_spi_a_gpio_init(void)
{
    BOARD_ERROR be_result = BOARD_ERR_OK;

    be_result |= be_board_pin_init( BOARD_SPI_A_MOSI_PORT,    BOARD_SPI_A_MOSI_PIN,     GPIO_Speed_10MHz,   GPIO_Mode_AF_PP);
    be_result |= be_board_pin_init( BOARD_SPI_A_MISO_PORT,    BOARD_SPI_A_MISO_PIN,     GPIO_Speed_10MHz,   GPIO_Mode_AF_PP);
    be_result |= be_board_pin_init( BOARD_SPI_A_SCK_PORT,     BOARD_SPI_A_SCK_PIN,      GPIO_Speed_10MHz,   GPIO_Mode_AF_PP);

    return(be_result);
}

/* This function init SPI A module. */
static BOARD_ERROR be_board_spi_a_module_init(void)
{
    BOARD_ERROR be_result = BOARD_ERR_OK;

    SPI_InitTypeDef SPIConf;
    /* Just TX. */
    SPIConf.SPI_Direction = SPI_Direction_1Line_Tx;

    /* Set Master mode. */
    SPIConf.SPI_Mode = SPI_Mode_Master;
    /* Set 8 bit send. */
    SPIConf.SPI_DataSize = SPI_DataSize_8b;
    /* Set mode 00.*/
    SPIConf.SPI_CPOL = SPI_CPOL_Low;
    SPIConf.SPI_CPHA = SPI_CPHA_1Edge;
    SPIConf.SPI_NSS = SPI_NSS_Soft;
    /* Set TX speed. */
    SPIConf.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
    /* Set MSB. */
    SPIConf.SPI_FirstBit = SPI_FirstBit_MSB;
    /* Call init function. */
    SPI_Init(SPI1, &SPIConf);
    /* Turn On SPI A */
    SPI_Cmd(SPI1, ENABLE);
    /* SS = 1 */
    SPI_NSSInternalSoftwareConfig(SPI1, SPI_NSSInternalSoft_Set);

    return(be_result);
}





#include "board_spi.h"

BOARD_ERROR be_board_spi_init(void)
{
    BOARD_ERROR be_result = BOARD_ERR_OK;

    be_result |= be_board_spi_a_gpio_init();
    be_result |= be_board_spi_a_module_init();
    
    be_result |= be_board_spi_b_gpio_init();
    be_result |= be_board_spi_b_module_init();

    
    
    return(be_result);
}

/* This function init SPI A interface. Master. */
static BOARD_ERROR be_board_spi_a_gpio_init(void)
{
    BOARD_ERROR be_result = BOARD_ERR_OK;

    be_result |= be_board_pin_init( GPIOA,      GPIO_Pin_7,     GPIO_Speed_10MHz,   GPIO_Mode_AF_PP);           /* MOSI_PIN */
    be_result |= be_board_pin_init( GPIOA,      GPIO_Pin_6,     GPIO_Speed_10MHz,   GPIO_Mode_IN_FLOATING);     /* MISO_PIN */
    be_result |= be_board_pin_init( GPIOA,      GPIO_Pin_5,     GPIO_Speed_10MHz,   GPIO_Mode_AF_PP);           /* SCK_PIN */

    return(be_result);
}

/* This function init SPI A module. */
static BOARD_ERROR be_board_spi_a_module_init(void)
{
    BOARD_ERROR be_result = BOARD_ERR_OK;

    SPI_InitTypeDef SPI_InitStructure;
   
    /* SPI1 configuration */
    SPI_InitStructure.SPI_Direction     = SPI_Direction_2Lines_FullDuplex;
    SPI_InitStructure.SPI_Mode          = SPI_Mode_Master;
    SPI_InitStructure.SPI_DataSize      = SPI_DataSize_16b;
    SPI_InitStructure.SPI_CPOL          = SPI_CPOL_High;
    SPI_InitStructure.SPI_CPHA          = SPI_CPHA_2Edge;
    SPI_InitStructure.SPI_NSS           = SPI_NSS_Soft;
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;
    SPI_InitStructure.SPI_FirstBit      = SPI_FirstBit_MSB;
    SPI_InitStructure.SPI_CRCPolynomial = 7U;
    
    SPI_Init(SPI1, &SPI_InitStructure);
 
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

    be_result |= be_board_pin_init( GPIOB,      GPIO_Pin_15,     GPIO_Speed_10MHz,   GPIO_Mode_IN_FLOATING);    /* MOSI_PIN */
    be_result |= be_board_pin_init( GPIOB,      GPIO_Pin_14,     GPIO_Speed_10MHz,   GPIO_Mode_AF_PP);          /* MISO_PIN */
    be_result |= be_board_pin_init( GPIOB,      GPIO_Pin_13,     GPIO_Speed_10MHz,   GPIO_Mode_IN_FLOATING);    /* SCK_PIN */

    return(be_result);
}

/* This function init SPI b module. */
static BOARD_ERROR be_board_spi_b_module_init(void)
{
    BOARD_ERROR be_result = BOARD_ERR_OK;

    SPI_InitTypeDef SPI_InitStructure;
    
    SPI_InitStructure.SPI_Direction     = SPI_Direction_2Lines_FullDuplex;
    SPI_InitStructure.SPI_Mode          = SPI_Mode_Slave;
    SPI_InitStructure.SPI_DataSize      = SPI_DataSize_16b;
    SPI_InitStructure.SPI_CPOL          = SPI_CPOL_High;                     /* SCK idle high */
    SPI_InitStructure.SPI_CPHA          = SPI_CPHA_2Edge;                    /* second transition -> SCK Idle high => capture on rising edge of clock */
    SPI_InitStructure.SPI_NSS           = SPI_NSS_Hard;
    SPI_InitStructure.SPI_FirstBit      = SPI_FirstBit_MSB;
    
    SPI_Init(SPI2, &SPI_InitStructure);

    return(be_result);
}




void   SPI1_IRQHandler(void)
{
 /* SPI_I2S_GetITStatus(SPI_TypeDef* SPIx, uint8_t SPI_I2S_IT) */
}
void   SPI2_IRQHandler(void)
{

}




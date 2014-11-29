

#include "board_gpio.h"



/* This function should set direction and mode for all pin working like GPIO. */
BOARD_ERROR be_board_gpio_init(void)
{
    BOARD_ERROR be_result = BOARD_ERR_OK;
    
    be_result |= be_board_pin_init( GPIOB, GPIO_Pin_1, GPIO_Speed_10MHz,GPIO_Mode_Out_PP);   /* PB1 connected to Blue LED on board. This pin has not external board connection. */
    GPIO_SetBits( GPIOB, GPIO_Pin_1);   /* Turn LED ON.(Just for test.)*/

    /* Outputs GPIO. */
    be_result |= be_board_pin_init( GPIOB, GPIO_Pin_0,  GPIO_Speed_10MHz,GPIO_Mode_Out_PP);  /* PB0  */  /* Pulse. */
    be_result |= be_board_pin_init( GPIOB, GPIO_Pin_2,  GPIO_Speed_10MHz,GPIO_Mode_Out_PP);  /* PB2  */  /* Dir. */
    be_result |= be_board_pin_init( GPIOB, GPIO_Pin_10, GPIO_Speed_10MHz,GPIO_Mode_Out_PP);  /* PB10 */  /* Enable. */
    be_result |= be_board_pin_init( GPIOB, GPIO_Pin_11, GPIO_Speed_10MHz,GPIO_Mode_Out_PP);  /* PB11 */  /* Encoder A. */
    be_result |= be_board_pin_init( GPIOB, GPIO_Pin_12, GPIO_Speed_10MHz,GPIO_Mode_Out_PP);  /* PB12 */  /* Encoder B. */
    be_result |= be_board_pin_init( GPIOC, GPIO_Pin_13, GPIO_Speed_10MHz,GPIO_Mode_Out_PP);  /* PC13 */  /* AGP emulation output. */ /* Reset value have to be "1". */

    GPIO_ResetBits( GPIOB, GPIO_Pin_0 );
    GPIO_ResetBits( GPIOB, GPIO_Pin_2 );  
    GPIO_ResetBits( GPIOB, GPIO_Pin_10);    
    GPIO_ResetBits( GPIOB, GPIO_Pin_11);    
    GPIO_ResetBits( GPIOB, GPIO_Pin_12);   
    GPIO_SetBits(   GPIOC, GPIO_Pin_13);    /* Reset value of AGP emmulator have to be "1". */

    /* Inputs GPIO. */
    be_result |= be_board_pin_init( GPIOB, GPIO_Pin_14, GPIO_Speed_10MHz,GPIO_Mode_IPU);     /* PB14 */  /* ENCODER_SIDE_END_SENSOR Input. */
    be_result |= be_board_pin_init( GPIOB, GPIO_Pin_15, GPIO_Speed_10MHz,GPIO_Mode_IPU);     /* PB15 */  /* MOTOR_SIDE_END_SENSOR Input. */
    be_result |= be_board_pin_init( GPIOA, GPIO_Pin_1,  GPIO_Speed_2MHz, GPIO_Mode_IPU);     /* PA1  */  /* Button 1. Input. */
    be_result |= be_board_pin_init( GPIOA, GPIO_Pin_2,  GPIO_Speed_2MHz, GPIO_Mode_IPU);     /* PA2  */  /* Button 2. Input. */
    be_result |= be_board_pin_init( GPIOA, GPIO_Pin_3,  GPIO_Speed_2MHz, GPIO_Mode_IPU);     /* PA3  */  /* Button 3. Input. */
    be_result |= be_board_pin_init( GPIOA, GPIO_Pin_4,  GPIO_Speed_2MHz, GPIO_Mode_IPU);     /* PA4  */  /* Button 4. Input. */
    be_result |= be_board_pin_init( GPIOA, GPIO_Pin_8,  GPIO_Speed_10MHz,GPIO_Mode_IPU);     /* PA8  */  /* PE desk Input. */

    be_result |= be_board_pin_init( GPIOC, GPIO_Pin_14, GPIO_Speed_10MHz,GPIO_Mode_IPU);     /* PC14 */  /* AGP Input. */
    be_result |= be_board_pin_init( GPIOC, GPIO_Pin_15, GPIO_Speed_10MHz,GPIO_Mode_IPU);     /* PC15 */  /* Head Gear Input. */
 
    return(be_result);
}

/*
    This function is setting GPIO pin on required mode and direction.
    GPIO_Mode_AIN             — analog input;
    GPIO_Mode_IN_FLOATING     — digital input, Z-state;
    GPIO_Mode_IPD             — digital input, pull-down;
    GPIO_Mode_IPU             — digital input, pull-up;
    GPIO_Mode_Out_OD          — output open drain;
    GPIO_Mode_Out_PP          — output push-pull;
    GPIO_Mode_AF_OD           - alternate function open-drain;
    GPIO_Mode_AF_PP           - alternate function push-pull;
*/
BOARD_ERROR be_board_pin_init(
                                GPIO_TypeDef *gpio_board_port,
                                uint16_t u16_port_pin,
                                GPIOSpeed_TypeDef gpio_speed_pin_speed,
                                GPIOMode_TypeDef gpio_mode_pin_mode
                              )
{
    BOARD_ERROR be_result = BOARD_ERR_OK;

    /* Create structure for initialisation. */
    GPIO_InitTypeDef GPIO_InitStructure;

    /* Enable PORT Peripheral clock. */
    if(gpio_board_port == GPIOA)
    {
        RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA , ENABLE);
    }
    else if(gpio_board_port == GPIOB)
    {
        RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB , ENABLE);
    }
    else if(gpio_board_port == GPIOC)
    {
        RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOC , ENABLE);
    }
    else
    {
        be_result = BOARD_ERR_ERROR;
    }

    /* Configurate the GPIO pin structure. */
    GPIO_InitStructure.GPIO_Pin   = u16_port_pin;
    GPIO_InitStructure.GPIO_Speed = gpio_speed_pin_speed;
    GPIO_InitStructure.GPIO_Mode  = gpio_mode_pin_mode;

    /* Call GPIO init function. */
    GPIO_Init( gpio_board_port, &GPIO_InitStructure);
    return(be_result);
}

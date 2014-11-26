

#include "board_gpio.h"



/* This function should set direction and mode for all pin working like GPIO. */
BOARD_ERROR be_board_gpio_init(void)
{
    BOARD_ERROR be_result = BOARD_ERR_OK;

    /* PB1 connected to Blue LED on board. This pin has not external board connection. */
    be_result = be_board_pin_init( GPIOB, GPIO_Pin_1, GPIO_Speed_10MHz,GPIO_Mode_Out_PP);

    /* Turn LED ON.(Just for test.)*/
    GPIO_SetBits( GPIOB, GPIO_Pin_1);

#if 0
    /* This pins using us ordinar GPIO OUT. */

    /* Enable. Output. */
    be_result = be_board_pin_init( GPIOB, GPIO_Pin_2, GPIO_Speed_10MHz,GPIO_Mode_Out_PP);
    GPIO_ResetBits( GPIOB, GPIO_Pin_2);

    /* Dir. Output. */
    be_result = be_board_pin_init( GPIOB, GPIO_Pin_11, GPIO_Speed_10MHz,GPIO_Mode_Out_PP);
    GPIO_ResetBits( GPIOB, GPIO_Pin_11);

    /* Pulse. Output. */
    be_result = be_board_pin_init( GPIOB, GPIO_Pin_12, GPIO_Speed_10MHz,GPIO_Mode_Out_PP);
    GPIO_ResetBits( GPIOB, GPIO_Pin_10);

    /* Encoder A. Output. */
    be_result = be_board_pin_init( GPIOB, GPIO_Pin_7, GPIO_Speed_10MHz,GPIO_Mode_Out_PP); /* PB7 */
    GPIO_ResetBits( GPIOB, GPIO_Pin_7);

    /* Encoder B. Output. */
    be_result = be_board_pin_init( GPIOB, GPIO_Pin_6, GPIO_Speed_10MHz,GPIO_Mode_Out_PP); /* PB6 */
    GPIO_ResetBits( GPIOB, GPIO_Pin_6);

#if 0
    /* AGP emulation output. Reset value have to be "1". */
     be_result = be_board_pin_init( GPIOC, GPIO_Pin_14, GPIO_Speed_10MHz,GPIO_Mode_Out_PP); /* PC14 */
     GPIO_SetBits( GPIOC, GPIO_Pin_14);
#endif

#if 0

    /* GPIO_B_IN_ENCODER_SIDE_END_SENSOR Input. */
    be_result = be_board_pin_init( GPIOB, GPIO_Pin_10, GPIO_Speed_10MHz,GPIO_Mode_IPU); /* PB10 */
    /* GPIO_B_IN_MOTOR_SIDE_END_SENSOR Input. */
    be_result = be_board_pin_init( GPIOB, GPIO_Pin_0, GPIO_Speed_10MHz,GPIO_Mode_IPU); /* PB0 */
    /* PE printer Input. */
    be_result = be_board_pin_init( GPIOC, GPIO_Pin_13, GPIO_Speed_10MHz,GPIO_Mode_IPU); /* PC13 */
#endif
    
#if 0
    /* AGP Input. */
    be_result = be_board_pin_init( GPIOC, GPIO_Pin_14, GPIO_Speed_10MHz,GPIO_Mode_IPU); /* PC14 */
#endif

#if 0
    /* Head Gear Input. */
    be_result = be_board_pin_init( GPIOC, GPIO_Pin_15, GPIO_Speed_10MHz,GPIO_Mode_IPU); /* PC15 */

    /* Button 1. Input. */
    be_result = be_board_pin_init( GPIOA, GPIO_Pin_0, GPIO_Speed_2MHz,GPIO_Mode_IPU); /* PA0 */
    /* Button 2. Input. */
    be_result = be_board_pin_init( GPIOA, GPIO_Pin_1, GPIO_Speed_2MHz,GPIO_Mode_IPU); /* PA1 */
    /* Button 3. Input. */
    be_result = be_board_pin_init( GPIOA, GPIO_Pin_2, GPIO_Speed_2MHz,GPIO_Mode_IPU); /* PA2 */
    /* Button 4. Input. */
    be_result = be_board_pin_init( GPIOA, GPIO_Pin_3, GPIO_Speed_2MHz,GPIO_Mode_IPU); /* PA3 */
#endif
    
    
#endif


    /* be_result = be_board_pin_init( GPIOA, GPIO_Pin_12, GPIO_Speed_10MHz,GPIO_Mode_Out_PP); */ /* PA12 */
    /* GPIO_ResetBits( GPIOA, GPIO_Pin_12); */






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

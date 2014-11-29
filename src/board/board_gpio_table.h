#ifndef BOARD_GPIO_TABLE_H
#define BOARD_GPIO_TABLE_H 1

#include "stm32f10x.h"


#define GPIO_B_OUT_MOTOR_DIR                    GPIO_Pin_2
#define GPIO_B_OUT_MOTOR_PULSE                  GPIO_Pin_0
#define GPIO_B_OUT_MOTOR_ENABLE                 GPIO_Pin_10

#define GPIO_B_OUT_ENCODER_A                    GPIO_Pin_11
#define GPIO_B_OUT_ENCODER_B                    GPIO_Pin_12

#define GPIO_C_OUT_APG_SENSOR                   GPIO_Pin_13


#define GPIO_B_IN_ENCODER_SIDE_END_SENSOR       GPIO_Pin_14
#define GPIO_B_IN_MOTOR_SIDE_END_SENSOR         GPIO_Pin_15

#define GPIO_A_IN_PE_SENSOR                     GPIO_Pin_8
#define GPIO_C_IN_APG_SENSOR                    GPIO_Pin_14
#define GPIO_C_IN_HEAD_GEAR_SENSOR              GPIO_Pin_15

#define GPIO_A_IN_BUTTON_1                      GPIO_Pin_1
#define GPIO_A_IN_BUTTON_2                      GPIO_Pin_2
#define GPIO_A_IN_BUTTON_3                      GPIO_Pin_3
#define GPIO_A_IN_BUTTON_4                      GPIO_Pin_4







#endif
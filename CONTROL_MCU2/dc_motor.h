/*
 * Module       : DC Motor
 *
 * Description  : Motor can rotate clockwise and anticlockwise with specific speed
 *
 * File Name    : dc_motor.c
 *
 * Author       : Youssef Osman
 */
#ifndef DC_MOTOR_H_
#define DC_MOTOR_H_

/******************************      Includes       **************************/

#include"std_types.h"

/*****************************      Definitions     ****************************/


#define IN_PORT_ID PORTB_ID

#define IN1_PIN_ID PIN0_ID
#define IN2_PIN_ID PIN1_ID

/******************************     Declarations    ****************************/

typedef enum
{
	stop,clockwise,anticlockwise
}DC_MOTOR_State;

/****************************** Functions Prototypes ***************************/

void DC_MOTOR_init(void);
void DC_MOTOR_rotate(uint8 speed,DC_MOTOR_State state);

#endif /* DC_MOTOR_H_ */

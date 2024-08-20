/*
 * Module       : DC Motor
 *
 * Description  : Motor can rotate clockwise and anticlockwise with specific speed
 *
 * File Name    : dc_motor.c
 *
 * Author       : Youssef Osman
 */
/******************************      Includes       **************************/

#include "gpio.h"
#include "dc_motor.h"
#include"pwm.h"
/****************************** Functions Definitions ***************************/

void DC_MOTOR_init(void)
{
	/* setup the direction for the two motor pins through the GPIO driver as output pins*/
	GPIO_setupPinDirection(IN_PORT_ID,IN1_PIN_ID,PIN_OUTPUT);
	GPIO_setupPinDirection(IN_PORT_ID,IN2_PIN_ID,PIN_OUTPUT);
	/* Stop DC Motor as initialization*/
	GPIO_writePin(IN_PORT_ID,IN1_PIN_ID,LOGIC_LOW);
	GPIO_writePin(IN_PORT_ID,IN2_PIN_ID,LOGIC_LOW);

}
/*
 * speed is by percentage , number from 0 to 100
 * state is stop motor -> 0 or rotate clockwise -> 1 or rotate anticlockwise -> 2
 * */
void DC_MOTOR_rotate(uint8 speed,DC_MOTOR_State state)
{
	/* calculate Duty cycle value from percentage */
	uint8 duty_cycle = (uint8)(((uint32)speed*255)/(100));
	Timer0_PWM_init(duty_cycle);

	if(state == stop)
	{
		GPIO_writePin(IN_PORT_ID,IN1_PIN_ID,LOGIC_LOW);
		GPIO_writePin(IN_PORT_ID,IN2_PIN_ID,LOGIC_LOW);
	}
	else if(state == clockwise)
	{
		GPIO_writePin(IN_PORT_ID,IN1_PIN_ID,LOGIC_HIGH);
		GPIO_writePin(IN_PORT_ID,IN2_PIN_ID,LOGIC_LOW);
	}
	else if(state == anticlockwise)
	{
		GPIO_writePin(IN_PORT_ID,IN1_PIN_ID,LOGIC_LOW);
		GPIO_writePin(IN_PORT_ID,IN2_PIN_ID,LOGIC_HIGH);
	}
}

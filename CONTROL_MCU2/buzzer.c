 /******************************************************************************
 *
 * Module: BUZZER
 *
 * File Name: buzzer.c
 *
 * Description: Source file for the buzzer driver
 *
 * Author: Youssef Osman
 *
 *******************************************************************************/

/****************************** Includes *****************************/

#include "gpio.h"
#include "buzzer.h"

/************************** Functions Definitions (Public) ************************/

/*
 * Set the direction for the buzzer pin as output pin through the GPIO driver
 * Turn off the buzzer through the GPIO
 * */
void BUZZER_init(void)
{
	GPIO_setupPinDirection(BUZZER_PORT_ID,BUZZER_PIN_ID,PIN_OUTPUT);
	GPIO_writePin(BUZZER_PORT_ID,BUZZER_PIN_ID,LOGIC_LOW);
}
/*
 * Function to enable the Buzzer through the GPIO.
 * */
void BUZZER_ON(void)
{
	GPIO_writePin(BUZZER_PORT_ID,BUZZER_PIN_ID,LOGIC_HIGH);
}
/*
 * Function to disable the Buzzer through the GPIO.
 * */
void BUZZER_OFF(void)
{
	GPIO_writePin(BUZZER_PORT_ID,BUZZER_PIN_ID,LOGIC_LOW);
}


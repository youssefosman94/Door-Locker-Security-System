/******************************************************************************
 *
 * Module: TIMER1
 *
 * File Name: timer1.c
 *
 * Description: Source file for the AVR Timer1 driver, support Normal Mode And Compare Mode
 *
 * Author: Youssef Osman
 *
 *******************************************************************************/

/**************************************** Includes   *****************************/
#include<avr/io.h>
#include<avr/interrupt.h>
#include"timer1.h"

/*******************************************************************************
 *                           Global Variables                                  *
 *******************************************************************************/

/*global variable to hold the address of the call back function in the application*/
static volatile void (*g_timer1CallBack)(void) = NULL_PTR;


/*******************************************************************************
 *                       Interrupt Service Routines                            *
 *******************************************************************************/

ISR (TIMER1_COMPA_vect)
{
	if(g_timer1CallBack != NULL_PTR)
	{
		(*g_timer1CallBack)();
	}
}
ISR (TIMER1_OVF_vect)
{
	if(g_timer1CallBack != NULL_PTR)
	{
		(*g_timer1CallBack)();
	}
}

/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/
/*
 * Description :
 * initialize timer 1 registers
 * Set the required Mode.
 *  Set the required Prescaler.
 *  Set the required initial value.
 *  Set the required compare value.
 * */

 void TIMER1_init(const TIMER1_ConfigType *Config_ptr)
{
	/*
	 * COM1A1 = 0 and COM1B1 = 0 and COM1A0 = 0 and COM1B0 = 0 for OC1A/OC1B disconnected
	 * FOC1B = 1 and FOC1A = 1 for non PWM
	 * set initial value in TCNT1
	 * */
	TCCR1A = (1<<FOC1A) | (1<<FOC1B);
	TCNT1 = Config_ptr->initial_value;

	if(Config_ptr->mode == NORMAL_MODE)
	{
		/*
		 * WGM10 = 0 and WGM11 = 0 and WGM12 = 0 and WGM13 = 0 for Normal Mode
		 * Enable Overflow Interrupt
		 * */
		TIMSK |= (1<<TOIE1);

	}
	else if(Config_ptr->mode == COMPARE_MODE)
	{
		/*
		 * WGM10 = 0 and WGM11 = 0 and WGM12 = 1 and WGM13 = 0 for Compare Mode
		 * Enable Compare A Match interrupt
		 * Put compare value in compare register OCR1A
		 * */
		TCCR1B |= (1<<WGM12);
		TIMSK |= (1<<OCIE1A);
		OCR1A = Config_ptr->compare_value;

	}
	TCCR1B |= (Config_ptr->prescaler); // select timer clock

}

 /*
  * Description: Function to set the Call Back function address.
  */
void TIMER1_setCallBack(void(*a_ptr)(void))
{
	g_timer1CallBack = a_ptr;
}

/*
 * Description: Function to disable the Timer1
 */
void TIMER1_deInit(void)
{
	TCCR1A = 0;
	TCNT1  = 0;
	TCCR1B = 0;
	OCR1A  = 0;
	/*disable interrupt*/
	TIMSK &= ~(1<<OCIE1A);
	TIMSK &= ~(1<<TOIE1);
	/* Reset the global pointer value */
	g_timer1CallBack = NULL_PTR;
}
















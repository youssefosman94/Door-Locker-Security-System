/******************************************************************************
 *
 * Module: TIMER1
 *
 * File Name: timer1.h
 *
 * Description: Header file for the AVR Timer1 driver, support Normal Mode And Compare Mode
 *
 * Author: Youssef Osman
 *
 *******************************************************************************/



#ifndef TIMER1_H_
#define TIMER1_H_

#include "std_types.h"

/******************************    Types Declarations    ****************************/

typedef enum
{
	NO_CLOCK,F_CPU_CLK,F_CPU_8,F_CPU_64,F_CPU_256,F_CPU_1024,EXTERNAL_CLK_ON_T1_FALLING,EXTERNAL_CLK_ON_T1_RAISING
}TIMER1_PRESCALER;

typedef enum
{
	NORMAL_MODE,COMPARE_MODE
}TIMER1_MODE;

typedef struct
{
	uint16 initial_value;
	uint16 compare_value;
	TIMER1_MODE mode;
	TIMER1_PRESCALER prescaler;
}TIMER1_ConfigType;

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/
/*
 * Description :
 * initialize timer 1 registers
 * Set the required Mode.
 *  Set the required Prescaler.
 *  Set the required initial value.
 *  Set the required compare value.
 * */
void TIMER1_init(const TIMER1_ConfigType *Config_ptr);

/*
 * Description: Function to set the Call Back function address.
 */
void TIMER1_setCallBack(void(*a_ptr)(void));

/*
 * Description: Function to disable the Timer1
 */
void TIMER1_deInit(void);





#endif /* TIMER1_H_ */

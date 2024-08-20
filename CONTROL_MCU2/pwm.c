/*
 * Module    : TIMER0 PWM
 *
 * file name : pwm.c
 *
 * Author    : Youssef Osman
 */
#include"gpio.h"
#include"pwm.h"
#include<avr/io.h>

/*
 * this function trigger timer0 in Fast PWM mode
 * */
void Timer0_PWM_init(uint8 dutyCycle)
{
	/* setup (OC0 / PB3) as output pin*/
	GPIO_setupPinDirection(PORTB_ID,PIN3_ID,PIN_OUTPUT);

	TCNT0 = 0; /* set timer initial value zero */
	OCR0 = dutyCycle;
	/*
	 * WGM01 = 1 and WGM00 = 1 for fast PWM mode
	 * COM01 = 1 and COM00 =0  for non inverting mode
	 * CS02 =0 and CS01 = 1 and CS00 = 0 for prescaler 8
	 * */
	TCCR0 = (1<<WGM00) | (1<<WGM01) | (1<<COM01) | (1<<CS01);

}

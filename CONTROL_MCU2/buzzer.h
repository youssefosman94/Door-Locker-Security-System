 /******************************************************************************
 *
 * Module: BUZZER
 *
 * File Name: buzzer.h
 *
 * Description: Header file for the buzzer driver
 *
 * Author: Youssef Osman
 *
 *******************************************************************************/
#ifndef BUZZER_H_
#define BUZZER_H_


/******************************   Definitions   *****************************/
#define BUZZER_PORT_ID PORTC_ID
#define BUZZER_PIN_ID PIN7_ID

/************************** Functions Prototypes     *********************/
/*
 * Set the direction for the buzzer pin as output pin through the GPIO driver
 * Turn off the buzzer through the GPIO
 * */
void BUZZER_init(void);
/*
 * Function to enable the Buzzer through the GPIO.
 * */
void BUZZER_ON(void);
/*
 * Function to disable the Buzzer through the GPIO.
 * */
void BUZZER_OFF(void);

#endif /* BUZZER_H_ */

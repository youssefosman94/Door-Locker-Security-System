/*
 * Module      : Keypad
 * Description : Header File for KEYPAD Driver
 * File Name   : keypad.h
 * Author      : Youssef Osman
 */

#ifndef KEYPAD_H_
#define KEYPAD_H_

/****************************** Common Includes *****************************/
#include"std_types.h"

/******************************   Definitions   *****************************/

/* keypad configurations for number rows and columns*/
#define KEYPAD_COLS_NUM 4
#define KEYPAD_ROWS_NUM 4

/* Keypad Port Configurations */
#define ROW_PORT_ID PORTA_ID
#define COL_PORT_ID PORTA_ID

/* Keypad Pins Configurations */
#define FIRST_PIN_IN_ROWS_ID PIN0_ID
#define FIRST_PIN_IN_COLS_ID PIN4_ID

/*Button Status Configurations 'select pull up or pull down connections'*/
#define BUTTON_PRESSED LOGIC_LOW
#define BUTTON_NOT_PRESSED LOGIC_HIGH

/************************** Functions Prototypes*********************/

uint8 KEYPAD_keyPressed(void);

#endif /* KEYPAD_H_ */

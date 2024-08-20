/*
 * Module      : Keypad
 * Description : Source File for KEYPAD Driver
 * File Name   : keypad.c
 * Author      : Youssef Osman
 */


/****************************** Includes *****************************/
#include"keypad.h"
#include"gpio.h"
#include"common_macros.h"
#include<util/delay.h>

/************************** Functions Prototype (private) ************************/
#ifndef STANDARD_KEYPAD

#if (KEYPAD_COLS_NUM ==3)
static uint8 KEYPAD_4x3_adjustKeyNumber(uint8 button_number);

#elif (KEYPAD_COLS_NUM==4)

static uint8 KEYPAD_4x4_adjustKeyNumber(uint8 button_number);

#endif

#endif /* STANDARD_KEYPAD */
/************************** Functions Definitions (Public) ************************/

uint8 KEYPAD_keyPressed(void)
{
	uint8 col,row ;
	GPIO_setupPinDirection(ROW_PORT_ID,FIRST_PIN_IN_ROWS_ID+0,PIN_INPUT);
	GPIO_setupPinDirection(ROW_PORT_ID,FIRST_PIN_IN_ROWS_ID+1,PIN_INPUT);
	GPIO_setupPinDirection(ROW_PORT_ID,FIRST_PIN_IN_ROWS_ID+2,PIN_INPUT);
	GPIO_setupPinDirection(ROW_PORT_ID,FIRST_PIN_IN_ROWS_ID+3,PIN_INPUT);

	GPIO_setupPinDirection(COL_PORT_ID,FIRST_PIN_IN_COLS_ID+0,PIN_INPUT);
	GPIO_setupPinDirection(COL_PORT_ID,FIRST_PIN_IN_COLS_ID+1,PIN_INPUT);
	GPIO_setupPinDirection(COL_PORT_ID,FIRST_PIN_IN_COLS_ID+2,PIN_INPUT);
#if KEYPAD_COLS_NUM ==4
	GPIO_setupPinDirection(COL_PORT_ID,FIRST_PIN_IN_COLS_ID+3,PIN_INPUT);
#endif

	while(1)
	{
		for(row=0; row<KEYPAD_ROWS_NUM; row++) /*Loop on rows*/
			{
				/* each time this pin for this row will be output pin and another pins of another rows will be input pins
				 * for detect button pressed , write on pin button pressed status
				 */
				GPIO_setupPinDirection(ROW_PORT_ID,FIRST_PIN_IN_ROWS_ID+row,PIN_OUTPUT);
				GPIO_writePin(ROW_PORT_ID,FIRST_PIN_IN_ROWS_ID+row,BUTTON_PRESSED);

				for(col = 0; col<KEYPAD_COLS_NUM; col++)/*loop on columns*/
				{
					if(GPIO_readPin(COL_PORT_ID,FIRST_PIN_IN_COLS_ID+col) == BUTTON_PRESSED)
					{
#if(KEYPAD_COLS_NUM ==4)
#ifdef STANDARD_KEYPAD
						return ((row*KEYPAD_COLS_NUM)+col+1);
#else
						return KEYPAD_4x4_adjustKeyNumber((row*KEYPAD_COLS_NUM)+col+1);
#endif /* STANDARD_KEYPAD */
#elif(KEYPAD_COLS_NUM ==3)
#ifdef STANDARD_KEYPAD
						return ((row*KEYPAD_COLS_NUM)+col+1);
#else
						return KEYPAD_4x3_adjustKeyNumber((row*KEYPAD_COLS_NUM)+col+1);
#endif /* STANDARD_KEYPAD */
#endif

					}
				}
				GPIO_setupPinDirection(ROW_PORT_ID,FIRST_PIN_IN_ROWS_ID+row,PIN_INPUT);
				_delay_ms(5); /* for proteus*/
			}
	}
}

/************************** Functions Definitions (Private) *************************/
#ifndef STANDARD_KEYPAD

#if (KEYPAD_COLS_NUM == 3)
static uint8 KEYPAD_4x3_adjustKeyNumber(uint8 button_number)
{
	uint8 keypadButton = 0;
	switch(button_number)
	{
		case 10: keypadButton ='*'; /*ASCII Code of * */
				break;
		case 11: keypadButton =0;
				break;
		case 12: keypadButton ='#'; /*ASCII Code of # */
				break;
		default: keypadButton=button_number;
				break;
	}
	return keypadButton;



}

#elif (KEYPAD_COLS_NUM ==4)
static uint8 KEYPAD_4x4_adjustKeyNumber(uint8 button_number)
{
	uint8 keypad_button = 0;
		switch(button_number)
		{
			case 1: keypad_button = 7;
					break;
			case 2: keypad_button = 8;
					break;
			case 3: keypad_button = 9;
					break;
			case 4: keypad_button = '%'; // ASCII Code of %
					break;
			case 5: keypad_button = 4;
					break;
			case 6: keypad_button = 5;
					break;
			case 7: keypad_button = 6;
					break;
			case 8: keypad_button = '*'; /* ASCII Code of '*' */
					break;
			case 9: keypad_button = 1;
					break;
			case 10: keypad_button = 2;
					break;
			case 11: keypad_button = 3;
					break;
			case 12: keypad_button = '-'; /* ASCII Code of '-' */
					break;
			case 13: keypad_button = 13;  /* ASCII of Space */
					break;
			case 14: keypad_button = 0;
					break;
			case 15: keypad_button = '='; /* ASCII Code of '=' */
					break;
			case 16: keypad_button = '+'; /* ASCII Code of '+' */
					break;
			default: keypad_button = button_number;
					break;
		}
		return keypad_button;
	}

#endif

#endif /* STANDARD_KEYPAD */

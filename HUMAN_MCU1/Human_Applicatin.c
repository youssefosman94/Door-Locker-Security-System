/*
 * Author       : Youssef Osman
 *
 * Project Name : DOOR LOCKER SECURITY SYSTEM
 *
 * File Name    : Human_Apllication.c
 *
 * Description  : HMI_ECU is just responsible interaction with the user just take inputs through keypad and display
				  messages on the LCD.
 * */

/*******************************************************************************
 *                                Includes                                  *
 *******************************************************************************/
#include<avr/interrupt.h>
#include <util/delay.h>
#include "lcd.h"
#include "keypad.h"
#include "uart.h"
#include "timer1.h"

/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/
#define PASSWORD_LENGTH 5
#define CONTROL_MCU2_READY 0x01
#define AVAILABLE_VALUE_TO_TAKE_CONSECTIVE_PASSWORD 3

#define OPEN_TIME    15   /* 15 Seconds */
#define CLOSE_TIME   15   /* 15 Seconds */
#define HOLD_TIME    3    /* 3  Seconds */
#define ALARM_TIME   60   /* 60 Seconds */

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/
void enter_password(uint8 row,uint8 col,uint8 *pass);
void First_Display(void);
void Main_Options(void);
void inits(void);
void Open_Door(void);
void Alarm(void);
void callback(void);
/*******************************************************************************
 *                           Global Variables                                  *
 *******************************************************************************/
uint8 User_Password[PASSWORD_LENGTH];
uint8 Reset_Password_Counter = 0;
volatile uint8 g_ticks = 0;

/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/
int main(void)
{
	uint8 match_state;
	sei(); // enable interrupts
	inits();

	UART_sendByte(CONTROL_MCU2_READY);
	First_Display();

	while(1)
	{
		Main_Options();
		while(KEYPAD_keyPressed() != '+' && KEYPAD_keyPressed() != '-');

		if(KEYPAD_keyPressed() == '+')
		{

			while(1)
			{
				LCD_clearScreen();
				LCD_displayString("plz enter pass:");
				enter_password(1,0,User_Password);

				UART_sendByte(CONTROL_MCU2_READY);
				UART_sendData(User_Password,PASSWORD_LENGTH);
				UART_sendByte('+');
				while(UART_recieveByte() != CONTROL_MCU2_READY);
				match_state = UART_recieveByte();
				if(match_state == TRUE)
				{
					Reset_Password_Counter = 0;
					Open_Door();
					break;
				}
				else if(match_state == FALSE)
				{
					Reset_Password_Counter++;
					if(Reset_Password_Counter != AVAILABLE_VALUE_TO_TAKE_CONSECTIVE_PASSWORD)
					{
						LCD_clearScreen();
						LCD_displayStringRowColumn(0,0,"Incorrect Pass");
						LCD_displayStringRowColumn(1,3,"Try Again");
						_delay_ms(500);

					}
					else if(Reset_Password_Counter == AVAILABLE_VALUE_TO_TAKE_CONSECTIVE_PASSWORD)
					{
						Reset_Password_Counter = 0;
						Alarm();
						break;
					}
				}

			}

		}
		else if (KEYPAD_keyPressed() == '-')
		{
			while(1)
			{
				LCD_clearScreen();
				LCD_displayString("plz enter pass:");
				enter_password(1,0,User_Password);

				UART_sendByte(CONTROL_MCU2_READY);
				UART_sendData(User_Password,PASSWORD_LENGTH);
				UART_sendByte('-');

				while(UART_recieveByte() != CONTROL_MCU2_READY);
				match_state = UART_recieveByte();

				if(match_state == TRUE)
				{
					Reset_Password_Counter = 0;
					LCD_clearScreen();
					LCD_displayString("correct pass");
					_delay_ms(500);
					LCD_clearScreen();
					First_Display();
					break;

				}
				else if(match_state == FALSE)
				{
					Reset_Password_Counter++;
					if(Reset_Password_Counter != AVAILABLE_VALUE_TO_TAKE_CONSECTIVE_PASSWORD)
					{
						LCD_clearScreen();
						LCD_displayStringRowColumn(0,0,"Incorrect Pass");
						LCD_displayStringRowColumn(1,3,"Try Again");
						_delay_ms(500);

					}
					else if(Reset_Password_Counter == AVAILABLE_VALUE_TO_TAKE_CONSECTIVE_PASSWORD)
					{
						Reset_Password_Counter = 0;
						Alarm();
						break;
					}

				}
			}
		}

	}

}

/*
 * Description:
 * 		initialize LCD and timer and UART
 * */
void inits(void)
{
	LCD_init();

	/*
	 * initialize UART Module
	 * 8-bit Data
	 * Even Parity
	 * One Stop bit
	 * Baud rate = 9600
	 * */
	UART_ConfigType configurations = {EIGHT_BITS,ONE_STOP_BIT,EVEN_PARITY,9600};
	UART_init(&configurations);

	/*
	 * initialize timer`
	 * F_CPU = 1 MHZ
	 * F_TIMER = 1 MHZ / 1024 = 977
	 * tick = 1 / 977 = 1.024 ms
	 * to get interrupt every one second
	 * compare value = 1 s / 1.024 ms = 977
	 * */
	TIMER1_ConfigType Timer_Config_Type = { 0, 977, COMPARE_MODE, F_CPU_1024 };
	TIMER1_init(&Timer_Config_Type);
	TIMER1_setCallBack(callback);

}


/*
 * Description:
 * take password from user by keypad and display as asterisks *****
 * */
void enter_password(uint8 row,uint8 col,uint8 *pass)
{
	uint8 i;
	uint8 key;
	LCD_moveCursor(row,col);
	for(i = 0;i<PASSWORD_LENGTH;i++)
		{
			key =KEYPAD_keyPressed();
			if(key >= 0 && key <= 9)
			{
				pass[i] = key;
				LCD_displayCharacter('*');
			}
			else
			{
				i--;
			}
			_delay_ms(500);
		}

	/* Select Equal character to enter password */
	while(KEYPAD_keyPressed() != '=');

}

/*
 * Description:
 *  	Enter password
 *  	ReEnter password
 *  	Send both to MCU2
 *  	*/
void First_Display(void)
{
	uint8 entered_password[PASSWORD_LENGTH];
	uint8 re_entered_password[PASSWORD_LENGTH];
	uint8 condition = FALSE;
	while(condition == FALSE)
	{
		LCD_displayString("plz enter pass:");
		enter_password(1,0,entered_password);
		UART_sendData(entered_password,PASSWORD_LENGTH);

		LCD_clearScreen();
		LCD_displayString("Plz re-enter the");
		LCD_displayStringRowColumn(1,0,"same pass:");
		enter_password(1,10,re_entered_password);
		UART_sendData(re_entered_password,PASSWORD_LENGTH);
		condition = UART_recieveByte();
		if(condition == TRUE)
		{
			LCD_clearScreen();
			LCD_displayString("Matched Pass");
			_delay_ms(500);

		}
		else
		{
			LCD_clearScreen();
			LCD_displayStringRowColumn(0,3,"Mismatch");
			LCD_displayStringRowColumn(1,3,"Try again");
			_delay_ms(500);
			LCD_clearScreen();
		}
	}
}


void Main_Options(void)
{
	LCD_clearScreen();
	LCD_displayStringRowColumn(0,0,"+ : Open Door");
	LCD_displayStringRowColumn(1,0,"- : Change Pass");
}

/*
 *  Description:
 *  Display Open Message 15 seconds
 *  Display Hold Message 3  seconds
 *	Display Open Message 15 seconds
 **/
void Open_Door(void)
{

	g_ticks =0;

	LCD_clearScreen();
	LCD_displayStringRowColumn(0,5,"Door is");
	LCD_displayStringRowColumn(1,5,"UnLocking");
	while(g_ticks < OPEN_TIME);// wait 15 second

	LCD_clearScreen();
	LCD_displayString("Door will lock");
	while(g_ticks < (OPEN_TIME+HOLD_TIME));// wait 3 seconds

	LCD_clearScreen();
	LCD_displayString("Door is locking");
	while(g_ticks < (OPEN_TIME+HOLD_TIME+CLOSE_TIME));// wait 15 second

}


/*
 * Description:
 *  Display Error message one Minute on screen when mismatch password with password saved in EEPROM
 * */
void Alarm(void)
{

	g_ticks =0;

	LCD_clearScreen();
	LCD_displayStringRowColumn(0,3,"Warning!");
	LCD_displayStringRowColumn(1,2,"Error pass");
	while(g_ticks < ALARM_TIME); // wait 60 second

}

/*
 * Description:
 * Callback function for Timer ISR
 * */
void callback(void)
{
	g_ticks++;

}







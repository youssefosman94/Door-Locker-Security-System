/*
 * Author       : Youssef Osman
 *
 * Project Name : DOOR LOCKER SECURITY SYSTEM
 *
 * File Name    : Control_Apllication.c
 *
 * Description  : CONTROL_ECU is responsible for all the processing and decisions in the system like password
				  checking,password saving in EEPROM, open the door and activate the system alarm.
 **/

/*******************************************************************************
 *                                Includes                                  *
 *******************************************************************************/
#include "uart.h"
#include "external_eeprom.h"
#include "twi.h"
#include "dc_motor.h"
#include "buzzer.h"
#include "timer1.h"
#include <avr/interrupt.h>
#include <util/delay.h>

/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/
#define PASSWORD_LENGTH     5
#define CONTROL_MCU2_READY  0x01
#define AVAILABLE_VALUE_TO_TAKE_CONSECTIVE_PASSWORD 3

#define OPEN_TIME    15   /* 15 Seconds */
#define CLOSE_TIME   15   /* 15 Seconds */
#define HOLD_TIME    3    /* 3  Seconds */
#define ALARM_TIME   60   /* 60 Seconds */

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/
void inits(void);
void First_Display(void);
uint8 passwords_check(uint8 *entered_password,uint8 *re_entered_password);
void saving_password(uint8 *pass);
void Read_Password_From_EEPROM(uint8 *Saved_Password);
void Open_Door(void);
void Alarm(void);
void callback(void);

/*******************************************************************************
 *                           Global Variables                                  *
 *******************************************************************************/
uint8 User_Password[PASSWORD_LENGTH];
volatile uint8 Reset_Password_Counter = 0;
volatile uint8 g_ticks = 0;

/*******************************************************************************
 *                      Functions Definitions                                   *
 *******************************************************************************/
int main(void)
{
	uint8 Saved_Password[PASSWORD_LENGTH];
	uint8 selection;
	uint8 match_state;

	sei(); // enable interrupts
	inits();
	/* wait until MCU1 receive ready byte*/
	while(UART_recieveByte() != CONTROL_MCU2_READY);
	First_Display();

	while(1)
	{

		/* read password from EEPROM Memory and  Saved it in Password Array*/
		Read_Password_From_EEPROM(Saved_Password);

		while(UART_recieveByte() != CONTROL_MCU2_READY);
		UART_receiveData(User_Password,PASSWORD_LENGTH);
		selection = UART_recieveByte();

		match_state = passwords_check(Saved_Password,User_Password);
		UART_sendByte(CONTROL_MCU2_READY);
		UART_sendByte(match_state);
		if(selection == '+')
		{

			if(match_state == TRUE)
			{
				Reset_Password_Counter = 0;
				Open_Door();
			}
			else if(match_state == FALSE)
			{

				Reset_Password_Counter++;
				if(Reset_Password_Counter == AVAILABLE_VALUE_TO_TAKE_CONSECTIVE_PASSWORD)
				{
					Reset_Password_Counter = 0;
					Alarm();
				}
			}

		}
		else if(selection == '-')
		{
			if(match_state == TRUE)
			{
				Reset_Password_Counter = 0;
				First_Display();
			}
			else if(match_state == FALSE)
			{
				Reset_Password_Counter++;
				if(Reset_Password_Counter == AVAILABLE_VALUE_TO_TAKE_CONSECTIVE_PASSWORD)
				{
					Reset_Password_Counter = 0;
					Alarm();
				}
			}
		}
	}
}

/*
 * Description:
 * 		initialize timer and UART and Twi and Buzzer and DcMotor
 * */
void inits(void)
{
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
	 * initialize TWI
	 * Prescaler = 1
	 * Baud Rate = 400KHZ
	 * Device Address = 0x01
	 * */
	TWI_configType configs = {PRESCALER_1,400000,0x01};
	TWI_init(&configs);

	/*
	 * initialize timer
	 * F_CPU = 1 MHZ
	 * F_TIMER = 1 MHZ / 1024 = 977
	 * tick = 1 / 977 = 1.024 ms
	 * to get interrupt every one second
	 * compare value = 1 s / 1.024 ms = 977
	 * */
	TIMER1_ConfigType Timer_Config_Type = { 0, 977, COMPARE_MODE, F_CPU_1024 };
	TIMER1_init(&Timer_Config_Type);
	TIMER1_setCallBack(callback);


	DC_MOTOR_init();
	BUZZER_init();
}

/*
 * Description:
 *  	Receive passwords
 *  	Compare Passwords
 *  	Send state (true for match state , false for mismatch state) to MCU1
**/
void First_Display(void)
{
	uint8 entered_password[PASSWORD_LENGTH];
	uint8 re_entered_password[PASSWORD_LENGTH];
	uint8 condition = FALSE;
	while(condition == FALSE)
	{
		UART_receiveData(entered_password,PASSWORD_LENGTH);
		UART_receiveData(re_entered_password,PASSWORD_LENGTH);

		condition = passwords_check(entered_password,re_entered_password);
		UART_sendByte(condition);
		if(condition == TRUE)
		{
			saving_password(entered_password);
		}
	}
}

/*
 * Description:
 * 		Save password in address 0x00F0 in EEPROM Memory
**/
void saving_password(uint8 *pass)
{
	uint8 i ;
	uint16 memory_location = 0x00F0;
	for(i = 0; i < PASSWORD_LENGTH; i++)
	{
		EEPROM_writeByte((memory_location+i),pass[i]);
		_delay_ms(100);
	}
}

/*
 * Description:
 * 		Read password from address 0x00F0
 * 		Return it as pointer to array */
void Read_Password_From_EEPROM(uint8 *Saved_Password)
{
	uint8 i;
	uint16 memory_location = 0x00F0;
	for(i = 0; i<PASSWORD_LENGTH;i++)
	{
		EEPROM_readByte((memory_location+i),Saved_Password+i);
		_delay_ms(100);
	}
}

/*
 * Description:
 * Inputs Two Passwords as Arguments and Compare together
 * Return State if Match or Not
 * */
uint8 passwords_check(uint8 *entered_password,uint8 *re_entered_password)
{
	uint8 i;
	uint8 check_state = 0;
	for(i = 0; i<PASSWORD_LENGTH;i++)
	{
		if(entered_password[i] == re_entered_password[i])
		{
			check_state++;
		}
	}
	if(check_state == PASSWORD_LENGTH)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

/*
 *  Description:
 *  rotate motor 15 second clockwise
 *  stop motor 3 second
 *  rotate motor 15 second anticlockwise
 **/

void Open_Door(void)
{

	g_ticks = 0;

	DC_MOTOR_rotate(100,clockwise);
	while(g_ticks < OPEN_TIME);// wait 15 second

	DC_MOTOR_rotate(0,stop);
	while(g_ticks < (OPEN_TIME+HOLD_TIME));// wait 3 seconds

	DC_MOTOR_rotate(100,anticlockwise);
	while(g_ticks < (OPEN_TIME+HOLD_TIME+CLOSE_TIME));// wait 15 second

	DC_MOTOR_rotate(0,stop);

}

/*
 * Description:
 *  Active Buzzer one Minute when mismatch password with password saved in EEPROM
 * */
void Alarm(void)
{

	g_ticks =0;

	BUZZER_ON();
	while(g_ticks < ALARM_TIME); // wait 60 second

	BUZZER_OFF();

}

/*
 * Description:
 * Callback function for Timer ISR
 * */
void callback(void)
{
	g_ticks++;
}

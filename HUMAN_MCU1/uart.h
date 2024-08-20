 /******************************************************************************
 *
 * Module: UART
 *
 * File Name: uart.h
 *
 * Description: Header file for the UART AVR driver
 *
 * Author: Youssef Osman
 *
 *******************************************************************************/

#ifndef UART_H_
#define UART_H_

#include "std_types.h"

/******************************   Types  Declarations    ****************************/

typedef enum
{
	DISABLE_PARITY,EVEN_PARITY=2,ODD_PARITY
}PARITY_MODE;
typedef enum
{
	FIVE_BITS,SIX_BITS,SEVEN_BITS,EIGHT_BITS
}NUMBER_OF_DATA_BITS;
typedef enum
{
	ONE_STOP_BIT,TWO_STOP_BIT
}NUMbER_OF_STOP_BIT;

typedef struct
{
	NUMBER_OF_DATA_BITS data;
	NUMbER_OF_STOP_BIT stopBit;
	PARITY_MODE parity;
	uint32 baud_rate;
}UART_ConfigType;


/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

/*
 * Description :
 * Functional responsible for Initialize the UART device by:
 * 1. Setup the Frame format like number of data bits, parity bit type and number of stop bits.
 * 2. Enable the UART.
 * 3. Setup the UART baud rate.
 */
void UART_init(const UART_ConfigType *config_ptr);
/*
 * Description :
 * Functional responsible for send byte to another UART device.
 */
void UART_sendByte(const uint8 data);

/*
 * Description :
 * Functional responsible for receive byte from another UART device.
 */
uint8 UART_recieveByte(void);

/*
 * Description :
 * Send the required string through UART to the other UART device.
 */
void UART_sendString(const uint8 *Str);

/*
 * Description :
 * Receive the required string until the '#' symbol through UART from the other UART device.
 */
void UART_receiveString(uint8 *Str); // Receive until #

void UART_sendData(uint8 *array,uint8 arrLength);

void UART_receiveData(uint8 *data,uint8 length);

#endif /* UART_H_ */

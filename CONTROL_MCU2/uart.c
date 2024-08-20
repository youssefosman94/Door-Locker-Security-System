 /******************************************************************************
 *
 * Module: UART
 *
 * File Name: uart.c
 *
 * Description: Source file for the UART AVR driver
 *
 * Author: Youssef Osman
 *
 *******************************************************************************/

#include "uart.h"
#include "avr/io.h" /* To use the UART Registers */
#include "common_macros.h" /* To use the macros like SET_BIT */



/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/


/*
 * Description :
 * Functional responsible for Initialize the UART device by:
 * 1. Setup the Frame format like number of data bits, parity bit type and number of stop bits.
 * 2. Enable the UART.
 * 3. Setup the UART baud rate.
 */
void UART_init(const UART_ConfigType *config_ptr)
{
	uint16 ubrr_value = 0;

	/* U2X = 1 for double transmission speed */
	UCSRA = (1<<U2X);

	/************************** UCSRB Description **************************
	 * RXCIE = 0 disable USART RX Complete Interrupt Enable
	 * TXCIE = 0 Disable USART Tx Complete Interrupt Enable
	 * UDRIE = 0 Disable USART Data Register Empty Interrupt Enable
	 * RXEN  = 1 Receiver Enable
	 * RXEN  = 1 Transmitter Enable
	 * don't use RXB8 and TXB8 because we don't support 9-bit data , so that UCSZ2 = 0
	 ***********************************************************************/ 
	UCSRB = (1<<RXEN) | (1<<TXEN) ;
	
	/************************** UCSRC Description **************************
	 * URSEL   = 1 The URSEL must be one when writing the UCSRC
	 * UMSEL   = 0 Asynchronous Operation
	 * UPM1 and UPM0 select Parity mode
	 * UCSZ1 and UCSZ0 data size
	 * USBS select number of stop bits
	 * UCPOL   = 0 Used with the Synchronous operation only
	 ***********************************************************************/ 	
	UCSRC = (1<<URSEL) | ((config_ptr->parity)<<UPM0) | ((config_ptr->stopBit)<<USBS) | ((config_ptr->data)<<UCSZ0);

	/* calculate the UBRR register value*/
	ubrr_value = ((F_CPU / ((config_ptr->baud_rate)* 8UL) ) - 1);
	/* First 8 bits from the BAUD_PRESCALE inside UBRRL and last 4 bits in UBRRH */
	/* URSEL must be zero when writing the UBRRH */
	UBRRL = (uint8) ubrr_value;
	UBRRH = (ubrr_value>>8);
}

/*
 * Description :
 * Functional responsible for send byte to another UART device.
 */
void UART_sendByte(const uint8 data)
{
	/*
	 * UDRE flag is set when the Tx buffer (UDR) is empty and ready for
	 * transmitting a new byte so wait until this flag is set to one
	 */
	while(BIT_IS_CLEAR(UCSRA,UDRE)){}

	/*
	 * Put the required data in the UDR register and it also clear the UDRE flag as
	 * the UDR register is not empty now
	 */
	UDR = data;
}

/*
 * Description :
 * Functional responsible for receive byte from another UART device.
 */
uint8 UART_recieveByte(void)
{
	/* RXC flag is set when the UART receive data so wait until this flag is set to one */
	while(BIT_IS_CLEAR(UCSRA,RXC)){}

	/*
	 * check on frame error and parity error before read data
	 * Read the received data from the Rx buffer (UDR)
	 * The RXC flag will be cleared after read the data
	 */
	if(BIT_IS_CLEAR(UCSRA,FE) && BIT_IS_CLEAR(UCSRA,PE))
	{
		return UDR;
	}
	else
	{
		return 0;
	}

}

/*
 * Description :
 * Send the required string through UART to the other UART device.
 */
void UART_sendString(const uint8 *Str)
{
	uint8 i = 0;

	/* Send the whole string */
	while(Str[i] != '\0')
	{
		UART_sendByte(Str[i]);
		i++;
	}
}

/*
 * Description :
 * Receive the required string until the '#' symbol through UART from the other UART device.
 */
void UART_receiveString(uint8 *Str)
{
	uint8 i = 0;

	/* Receive the first byte */
	Str[i] = UART_recieveByte();

	/* set our own Null # */
	/* Receive the whole string until the '#' */
	while(Str[i] != '#')
	{
		i++;
		Str[i] = UART_recieveByte();
	}

	/* After receiving the whole string plus the '#', replace the '#' with '\0' */
	Str[i] = '\0';
}
/*
 * Description :
 * send the required data
 * */
void UART_sendData(uint8 *array,uint8 arrLength)
{
	uint8 i;
	for(i =0;i<arrLength;i++)
	{
		UART_sendByte(array[i]);

	}

}
/*
 * Description :
 * receive the required data
 * */
void UART_receiveData(uint8 *data,uint8 length)
{
	uint8 i;
	for(i = 0;i<length;i++)
	{
		data[i] = UART_recieveByte();
	}
}


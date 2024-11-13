#include "UART.h"

void USART_Init( unsigned int ubrr )
{
	UBRR0H = (unsigned char)(ubrr>>8); // Set baud rate high
	UBRR0L = (unsigned char)ubrr; // BR low
	UCSR0B = (1<<RXEN0)|(1<<TXEN0); // Enable receiver and transmitter
	UCSR0C = (1<<URSEL0)|(1<<USBS0)|(3<<UCSZ00); //Set frame format: 8data, 2stop bit
}


void USART_Transmit( unsigned char data )
{
	while ( !( UCSR0A & (1<<UDRE0)) ); //Wait for empty transmit buffer
	UDR0 = data; // Put data into buffer, sends the data
}


unsigned char USART_Receive( void )
{
	while ( !(UCSR0A & (1<<RXC0)) );
	return UDR0;
}


int uart_putchar(char c, FILE *stream)
{
	if (c == '\n') {
		USART_Transmit('\r');  // Send carriage return before newline
	}
	USART_Transmit(c);  // Transmit the character
	return 0;
}


void init_printf(void)
{
	static FILE uart_stdout = FDEV_SETUP_STREAM(uart_putchar, NULL, _FDEV_SETUP_WRITE);
	stdout = &uart_stdout;  // Set stdout to use uart_putchar
}

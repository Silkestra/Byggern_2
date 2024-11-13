#pragma once
#include <avr/io.h>
#include <stdio.h> 

#define F_CPU 4915200UL
#define FOSC 4915200UL
#define BAUD 9600
#define MYUBRR FOSC/16/BAUD-1

void USART_Init( unsigned int ubrr );
void USART_Transmit( unsigned char data );
unsigned char USART_Receive( void );
int uart_putchar(char c, FILE *stream);
void init_printf(void);

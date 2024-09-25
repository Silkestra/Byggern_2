/*
 * byggernV5.c
 *
 * Created: 11.09.2024 13:56:45
 * Author : augusal
 */ 

#include <avr/io.h>
#include "UART.h"
#include "xmem.h"
#include <util/delay.h>
#include "io.h"

void pwm_init_og(void) {
		DDRD |=(1<<PD4);
		TCCR3A = 0x00;
		TCCR3B = 0x00;
		TCCR3A = (1<<COM3A1) | (1<<COM3B1) | (0<<WGM31) | (1<<WGM30);
		TCCR3B = (0<<WGM33) | (1<<WGM32) |(1<<CS30);
		OCR3B = 0;
		};
		
void pwm_init(void) {
		DDRD |=(1<<DDD4);
		TCCR3A = (0<<COM3A0) | (1<<COM3A1) | (0<<WGM31) | (1<<WGM30);
		TCCR3B = (0<<ICES3) | (1<<WGM32) |(1<<CS30);
		OCR3B = 0;
	};
	
int main(void)
{
	pwm_init();
	USART_Init(MYUBRR);
	init_printf();
	xmem_init();
	//printf("Hei");
	oled_init();
	//xmem_write(0xb2, 0, oled_command);
	//xmem_write(0x20, 0, oled_command);
	//xmem_write(0x10, 0, oled_command);
	IO io;
    /* Replace with your application code */
	for(int seg=0; seg<8*128+1; seg++){
		//xmem_write(0b00000000,0,oled_data);
	}
	
	OLED_reset();
	
	OLED_pos(0, 0);
	for(int row = 0; row < 8; row++){
		for(int column = 0; column < 128; column++){
			OLED_pos(row,column);
			if(row == 0 || row == 7){
				OLED_write_data('_');
			}
			if(column == 0 || column == 120){
				OLED_write_data('|');
			}
		}
	}
	
	OLED_pos(1, 50);
	OLED_write_data('H');
	OLED_write_data('O');
	OLED_write_data('M');
	OLED_write_data('E');
	
	//const unsigned char A[] = {0b01111100,0b01111110,0b00010011,0b00010011,0b01111110,0b01111100,0b00000000,0b00000000};
    while (1) 
    {
		
		for(int j=0; j<8; j++){
			//xmem_write(A[j], 0, oled_data);
			_delay_ms(60);
		}
		
		/*volatile char *adc=(char *) 0x1400;
		adc[0]=16;
		_delay_ms(10);
		uint16_t value=adc[0];
		printf("Adc-verdi: %u\n\n",value);
		_delay_ms(10);*/
		
		//setStates(&io);
		//getStates(&io);
		//SRAM_test();
		
    }
}


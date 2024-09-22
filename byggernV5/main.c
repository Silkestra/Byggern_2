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

    void SRAM_test(void)
    {
	    volatile char *ext_ram = (char *) 0; // Start address for the SRAM 0x1800
	    uint16_t ext_ram_size = 0x1FFF;
	    uint16_t write_errors = 0;
	    uint16_t retrieval_errors = 0;
	    printf("Starting SRAM test...\n");
	    // rand() stores some internal state, so calling this function in a loop will
	    // yield different seeds each time (unless srand() is called before this function)
	    uint16_t seed = rand();
	    // Write phase: Immediately check that the correct value was stored
	    srand(seed);
	    for (uint16_t i = 0; i < ext_ram_size; i += 4) {
		    uint8_t some_value = rand();
		    ext_ram[i] = some_value;
		    printf("%d",i);
	    }
	    // Retrieval phase: Check that no values were changed during or after the write phase
	    // reset the PRNG to the state it had before the write phase
	    printf("SRAM test completed with \n%4d errors in write phase and \n%4d errors in retrieval phase\n\n", write_errors, retrieval_errors);
    }

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
	printf("Hei");
	IO io;
    /* Replace with your application code */
    while (1) 
    {
		/*volatile char *adc=(char *) 0x1400;
		adc[0]=16;
		_delay_ms(10);
		uint16_t value=adc[0];
		printf("Adc-verdi: %u\n\n",value);
		_delay_ms(10);*/
		
		setStates(&io);
		getStates(&io);
		_delay_ms(200);
    }
}


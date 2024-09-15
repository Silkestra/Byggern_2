/*
 * byggernV5.c
 *
 * Created: 11.09.2024 13:56:45
 * Author : augusal
 */ 

#include <avr/io.h>
#include "UART.h"
#include "xmem.h"
#include <stdlib.h>
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

int main(void)
{
	USART_Init(MYUBRR);
	init_printf();
	xmem_init();
	SRAM_test();
    /* Replace with your application code */
    while (0) 
    {
    }
}


#include "io.h"


void setStates(IO* io){
	volatile char *adc=(char *) 0x1400;
	adc[0]=16;
	_delay_ms(10);
	uint16_t value=adc[0];
	printf("Adc-verdi: %u\n\n",value);
	_delay_ms(10);
	io->joy_x = adc[0];
	
}
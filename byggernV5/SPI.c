#include "SPI.h"


void SPI_SS_low(void) {
	PORTB &= ~(1 << PB4);  // Pull SS/CS (PB4) low to select the slave
}

void SPI_SS_high(void) {
	PORTB |= (1 << PB4);   // Pull SS/CS (PB4) high to deselect the slave
}


//SPI functions
void SPI_master_init(void){
	DDRB = (1<<PB5)|(1<<PB7)|(1<<PB4); //Set MOSI and SCK output, all others input
	SPI_SS_high();
	SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR0); //Enable SPI, Master, set clock rate fck/16
}

uint8_t SPI_MasterTransmit(char cData){
	SPDR = cData; //Start transmission
	while(!(SPSR & (1<<SPIF))){
		} //Wait for transmission complete
	return SPDR;
}

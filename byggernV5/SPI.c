#include "SPI.h"





void SPI_master_init(void){
	/* Set MOSI and SCK output, all others input */
	DDRB = (1<<PB5)|(1<<PB7);
	/* Enable SPI, Master, set clock rate fck/16 */
	SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR0);
}

void SPI_MasterTransmit(char cData){
	/* Start transmission */
	SPDR = cData;
	printf()
	/* Wait for transmission complete */
	while(!(SPSR & (1<<SPIF)));
}
//Can controller config 

void can_cntrl_config(void){
	
}


//CAN Controller functions 

uint16_t can_cntrl_read(void){
	SPI_MasterTransmit(0x03);
}

void can_cntrl_write(int8_t address, int8_t data){
	SPI_MasterTransmit(0x03);
	SPI_MasterTransmit(address);
	SPI_MasterTransmit(data);
}

void can_cntrl_reset(void){
	SPI_MasterTransmit(0xC0);	
}
void can_cntrl_RTS(void){
	SPI_MasterTransmit(0xC0);
}
void can_cntrl_reset(void){
	SPI_MasterTransmit(0xC0);
}
void can_cntrl_bit_mod(void){
	SPI_MasterTransmit(0x05);
}

bool can_cntrl_read_status(void){
	SPI_MasterTransmit(0xA0)
}
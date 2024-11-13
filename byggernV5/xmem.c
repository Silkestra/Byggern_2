#include "xmem.h"
#include <avr/io.h>

void xmem_init ( void ){
	MCUCR |= (1 << SRE ); // enable XMEM
	
	SFIOR &= ~(1 << XMM0 | 1 << XMM1 ); // mask bits / reduce bus width
	SFIOR |= (1 << XMM2 );
}

void xmem_write ( uint8_t data , uint16_t addr, uint16_t base_address){
	volatile char * ext_mem = ( char *) base_address ;
	ext_mem [ addr ]= data ;
}

uint8_t xmem_read ( uint16_t addr, uint16_t base_address){
	volatile char * ext_mem = ( char *) base_address;
	uint8_t ret_val = ext_mem [ addr ];
	return ret_val ;
}
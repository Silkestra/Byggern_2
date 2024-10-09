#include "CAN.h"

#define CNF1 0x2A
#define CNF2 0x29
#define CNF3 0x28
#define CANCTRL 0x0F //0xXF, X satt til 0

//CAN Controller functions
uint16_t can_cntrl_read(uint8_t address){
	uint8_t output;
	SPI_MasterTransmit(0x03);
	SPI_MasterTransmit(address);
	output = SPI_MasterTransmit(0xFF);
	return output;
}

void can_cntrl_write(int8_t address, int8_t data){
	SPI_MasterTransmit(0x02);
	SPI_MasterTransmit(address);
	SPI_MasterTransmit(data);
}

void can_cntrl_reset(void){
	SPI_MasterTransmit(0xC0);
}
void can_cntrl_RTS(int8_t transmit_buffers){
	SPI_MasterTransmit(0x80 | transmit_buffers);
}

void can_cntrl_bit_mod(int8_t address, int8_t data, int8_t mask){
	SPI_MasterTransmit(0x05);
	SPI_MasterTransmit(address);
	SPI_MasterTransmit(mask);
	SPI_MasterTransmit(data);
}

int8_t can_cntrl_read_status(void){
	SPI_MasterTransmit(0xA0);
	return SPI_MasterTransmit(0xFF); //Output
}

void can_cntrl_config(void){
	can_cntrl_reset();
	
	float T_Q = 0.00000025; //Satt en random verdi, ikke sikkert det fungerer (studass)
	long F_OSC = 16 * 10^6;
	int BRP = (T_Q * F_OSC / 2) - 1;
	
	uint8_t cnf1_value = (1 << 6) | BRP;
	uint8_t cnf2_value = (1 << 7) | (1 << 3) | 1;
	uint8_t cnf3_value = 2;
	
	can_cntrl_write(CNF1, cnf1_value);
	can_cntrl_write(CNF2, cnf2_value);
	can_cntrl_write(CNF3, cnf3_value);
	
	//Set operation mode
	int8_t canctrl_value = can_cntrl_read(CANCTRL);
	canctrl_value &= ~(0xE0);
	canctrl_value |= (0x02 << 5);
	
	can_cntrl_write(CANCTRL, canctrl_value);
}
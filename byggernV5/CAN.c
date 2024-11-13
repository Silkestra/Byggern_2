#include "CAN.h"
#include <util/delay.h>
#include "stdlib.h"


//CAN Controller functions
uint16_t can_cntrl_read(uint8_t address){
	SPI_SS_low();
	uint8_t output;
	SPI_MasterTransmit(0x03);
	SPI_MasterTransmit(address);
	output = SPI_MasterTransmit(0x0);
	SPI_SS_high();
	return output;
}

void can_cntrl_write(int8_t address, int8_t data){
	
	SPI_SS_low();
	
	SPI_MasterTransmit(0x02);
	
	SPI_MasterTransmit(address);
	
	SPI_MasterTransmit(data);
	
	SPI_SS_high();
	
}

void can_cntrl_reset(void){
	SPI_SS_low();
	SPI_MasterTransmit(0xC0);
	SPI_SS_high();
}

void can_cntrl_RTS(int8_t transmit_buffers){
	SPI_SS_low();
	SPI_MasterTransmit(0x81 | transmit_buffers);
	SPI_SS_high();
}

void can_cntrl_bit_mod(int8_t address, int8_t data, int8_t mask){
	SPI_SS_low();
	SPI_MasterTransmit(0x05);
	SPI_MasterTransmit(address);
	SPI_MasterTransmit(mask);
	SPI_MasterTransmit(data);
	SPI_SS_high();
}

int8_t can_cntrl_read_status(void){
	SPI_SS_low();
	SPI_MasterTransmit(0xA0);
	int8_t result = SPI_MasterTransmit(0x0); //Output
	SPI_SS_high();
	return result;
}

/*void can_cntrl_int_config(void){
	//DDRE = (1<<PE0); //interrupt pin enable 
	int8_t current_value = can_cntrl_read(MCP_CANINTE);
	current_value |= 0x1F
	can_cntrl_write(MCP_CANINTE, current_value);

}
void ISR(void){
	
}
*/


void can_cntrl_config(void){
	can_cntrl_reset();
	_delay_ms(1); //delay viktig 
	SPI_SS_low();
	
	float T_Q = 0.0000005; //Satt en random verdi, ikke sikkert det fungerer (studass)
	long F_OSC = 16 * 1000000;
	int BRP = (T_Q * F_OSC / 2) - 1;
	
	
	uint8_t cnf1_value = (2 << 6) | 3;
	uint8_t cnf2_value = (1 << 7) | (6 << 3) | 0 << 6;
	uint8_t cnf3_value = 5;
	
	can_cntrl_write(CNF1, cnf1_value);
	can_cntrl_write(CNF2, cnf2_value);
	can_cntrl_write(CNF3, cnf3_value);
	
	//Set operation mode
	int8_t canctrl_value = can_cntrl_read(CANCTRL);
	//printf("%d", canctrl_value);
	canctrl_value &= ~(0xE0);
	canctrl_value |= (0x00 << 5);
	//printf("%d", canctrl_value);
	
	can_cntrl_write(CANCTRL, canctrl_value);
	
	/*can_cntrl_write(RXB0CTRL, 0x60); // Set RXB0 to accept all messages
	
	// Set masks to accept all bits
	can_cntrl_write(RXM0SIDH, 0x00);
	can_cntrl_write(RXM0SIDL, 0x00);
	can_cntrl_write(RXM1SIDH, 0x00);
	can_cntrl_write(RXM1SIDL, 0x00);
	
	// Configure filters to accept all messages
	can_cntrl_write(RXF0SIDH, 0x00);
	can_cntrl_write(RXF0SIDL, 0x00);
	can_cntrl_write(RXF1SIDH, 0x00);
	can_cntrl_write(RXF1SIDL, 0x00);*/
	
	SPI_SS_high();
}

void can_message_send (can_message * msg ) {
	can_cntrl_write(TXB0SIDH, msg->id >> 3);
	can_cntrl_write(TXB0SIDL, (msg->id & 0x07) << 5);
	can_cntrl_write(TXB0DLC, msg->data_length);
	
	for (int i = 0; i < msg->data_length; i++){
		can_cntrl_write(TXB0D0+i, msg->data[i]);
	}
	can_cntrl_RTS(0x01);
	
}


can_message can_message_read (int buffer) {
	can_message msg; 
	uint8_t sidh;
	uint8_t sidl;
	uint8_t dlc;
	uint8_t dOO;
	if(buffer==0){
		sidh=RXB0SIDH;
		sidl=RXB0SIDL;
		dlc=RXB0DLC;
		dOO=RXB0D0;
	}
	if(buffer==1){
		sidh=RXB1SIDH;
		sidl=RXB1SIDL;
		dlc=RXB1DLC;
		dOO=RXB1D0;
	}
	
    uint8_t high = can_cntrl_read(sidh);
    uint8_t low = can_cntrl_read(sidl);
   
    msg.id = (high << 3) | (low >> 5);
	msg.data_length = can_cntrl_read(dlc);
	
	for (int i = 0; i < msg.data_length; i++){
		msg.data[i] = can_cntrl_read(dOO + i);
	}
	 if (buffer == 0) {
		    // Clear interrupt flag for buffer 0
		  can_cntrl_write(0x2C, can_cntrl_read(0x2C) & ~0x01);
		    } else if (buffer == 1) {
		    // Clear interrupt flag for buffer 1
		    can_cntrl_write(0x2C, can_cntrl_read(0x2C) & ~0x02); //canintf=0x2C
	    }
	return msg;
}
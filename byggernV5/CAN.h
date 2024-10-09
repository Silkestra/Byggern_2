#include "MCP2515.h"
#include "SPI.h"
#define CNF1 0x2A
#define CNF2 0x29
#define CNF3 0x28
#define CANCTRL 0x0F //0xXF, X satt til 0


typedef struct {
	unsigned int id;
	char data_length;
	char data [8];
} can_message;


//CAN Controller functions
void can_cntrl_config(void);
uint16_t can_cntrl_read(uint8_t address);
void can_cntrl_write(int8_t address, int8_t data);
void can_cntrl_reset(void);
void can_cntrl_RTS(int8_t transmit_buffers);
void can_cntrl_bit_mod(int8_t address, int8_t data, int8_t mask);
int8_t can_cntrl_read_status(void);
void can_message_send (can_message * msg);
can_message can_message_read (void);
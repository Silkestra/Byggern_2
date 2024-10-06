#include <avr/io.h>

void SPI_SS_low(void);

void SPI_SS_high(void);

//SPI functions
void SPI_master_init(void);
uint8_t SPI_MasterTransmit(char cData);

//CAN Controller functions
void can_cntrl_config(void);
uint16_t can_cntrl_read(uint8_t address);
void can_cntrl_write(int8_t address, int8_t data);
void can_cntrl_reset(void);
void can_cntrl_RTS(int8_t transmit_buffers);
void can_cntrl_bit_mod(int8_t address, int8_t data, int8_t mask);
int8_t can_cntrl_read_status(void);
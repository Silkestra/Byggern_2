#pragma once
#include <stdint.h>
#define BASE_ADDRESS 0x1000
void xmem_init ( void );
void xmem_write ( uint8_t data , uint16_t addr, uint16_t base_address);
uint8_t xmem_read ( uint16_t addr, uint16_t base_address);

/*
 * byggernV5.c
 *
 * Created: 11.09.2024 13:56:45
 * Author : augusal
 */ 

#include <avr/io.h>
#include "UART.h"
#include "xmem.h"
#include <util/delay.h>
#include "io.h"
#include "SPI.h"
#include "CAN.h"
//#include "menu.h"

void pwm_init_og(void) {
	DDRD |=(1<<PD4);
	TCCR3A = 0x00;
	TCCR3B = 0x00;
	TCCR3A = (1<<COM3A1) | (1<<COM3B1) | (0<<WGM31) | (1<<WGM30);
	TCCR3B = (0<<WGM33) | (1<<WGM32) |(1<<CS30);
	OCR3B = 0;
};
		
void pwm_init(void) {
	DDRD |=(1<<DDD4);
	TCCR3A = (0<<COM3A0) | (1<<COM3A1) | (0<<WGM31) | (1<<WGM30);
	TCCR3B = (0<<ICES3) | (1<<WGM32) |(1<<CS30);
	OCR3B = 0;
};



int main(void)
{
	/* Replace with your application code */
	pwm_init();
	USART_Init(MYUBRR);
	init_printf();
	xmem_init();
	oled_init();
	IO io;
	io_init(&io);
	OLED_reset();
	
	
	//const char* home_men[] = {"HOME", "Menu 1", "Menu 2", "Menu 3", "Menu 4", "Menu 5"};
	//int16_t menu_length = sizeof(home_men) / sizeof(home_men[0]);
	//OLED_home(home_menu, menu_length);
	//OLED_print_menu(home_men, menu_length);
	
	//current_menu = &home_menu;
	//OLED_print_menu((char** ) current_menu->items, current_menu->num_items);
	
	//const char* menu1[] = {"Menu 1", "HOME", "Test 1", "Test 2"};
	//int16_t menu1_length = sizeof(menu1) / sizeof(menu1[0]);
	
	menu_init(&io);
	SPI_master_init();
	can_cntrl_config();
	
	can_message msg = {0x7ff,0x02,{0xCC, 0xBB}};
	
	//Write
	/*
	can_cntrl_write(TXB0SIDH, 0xFF); // SIDH (higher 8 bits of the ID)
	can_cntrl_write(TXB0SIDL, 0x07); // SIDL (lower 3 bits and extended flags)

	// Set Data Length Code (DLC) to specify the number of data bytes, e.g., 8 bytes
	can_cntrl_write(TXB0DLC, 0x02);
	
	can_cntrl_write(TXB0D0, 0x51);
	can_cntrl_write(TXB0D1, 0xCF);
	can_cntrl_RTS(0x01);

	//READ
	//if (can_cntrl_read_status() & (1<<6)){
		uint8_t sid_high = can_cntrl_read(RXB0SIDH);
		uint8_t sid_low = can_cntrl_read(RXB0SIDL);
		uint16_t messageID = (sid_high << 3) | (sid_low);
		uint8_t dlc = can_cntrl_read(RXB0DLC) & 0x0F; // Mask to get only the lower 4 bits (DLC)
		
		uint8_t out1 = can_cntrl_read(RXB0D0);
		uint8_t out2 = can_cntrl_read(RXB0D1);
		*/
	can_message_send(&msg);
	can_message out = can_message_read();
	
		printf("D0: %02x\n", out.data[0]);
		printf("D1: %02x\n", out.data[1]);
		printf("Dlc: %02x\n", out.data_length);
		printf("id: %02x\n", out.id);
	//}
	

	
	
    while (1) 
    {
		
		
		//Write 
		
		/*volatile char *adc=(char *) 0x1400;
		adc[0]=16;
		_delay_ms(10);
		uint16_t value=adc[0];
		printf("Adc-verdi: %u\n\n",value);
		_delay_ms(10);*/
		
		//set_states(&io);
		//get_states(&io);
		//SRAM_test();
		
    }
}


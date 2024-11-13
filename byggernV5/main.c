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
	pwm_init();
	USART_Init(MYUBRR);

	init_printf();
	xmem_init();
	oled_init();
	IO io;
	io_init(&io);
	OLED_reset();
	
	
	menu_init(&io);
	SPI_master_init();
	can_cntrl_config();
		
	can_message msg = {0x7ff, 0x02, {0xa0, 0x54}};
	can_message joy_msg={};
	can_message received={0x000,0x01,{0x0}};
	
	can_cntrl_write(0x2C, can_cntrl_read(0x2C) & ~0x01);
	
    while (1) 
    {
		set_states(&io);
		get_states(&io);
		
		received = can_message_read(0);
		printf("Received id: %x\n", received.id);
		if(received.id == 0x001){
			printf("Game over");
			io.game_active = false;
			io.current_node = io.death_node;
			OLED_print_menu_2(io.current_node);
		} 
		printf("game active %d\n", io.game_active);
		
		if(io.game_active){
			joy_msg=send_joy_pos(&io);
			can_message_send(&joy_msg);
		}
    }
}


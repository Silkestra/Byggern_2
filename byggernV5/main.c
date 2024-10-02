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
	IO io = {0, 0, 0, 0, NEUTRAL, 0, 2, 0};
	OLED_reset();
	//OLED_print("Snabel");
	
	//const char* home_menu[] = {"HOME", "Menu 1", "Menu 2", "Menu 3", "Menu 4", "Menu 5"};
	int16_t menu_length = sizeof(home_menu) / sizeof(home_menu[0]);
	//OLED_home(home_menu, menu_length);
	OLED_print_menu(home_menu, menu_length);
	
	//const char* menu1[] = {"Menu 1", "HOME", "Test 1", "Test 2"};
	int16_t menu1_length = sizeof(menu1) / sizeof(menu1[0]);
	
	
	
    while (1) 
    {
		/*volatile char *adc=(char *) 0x1400;
		adc[0]=16;
		_delay_ms(10);
		uint16_t value=adc[0];
		printf("Adc-verdi: %u\n\n",value);
		_delay_ms(10);*/
		
		set_states(&io);
		get_states(&io);
		//SRAM_test();
		
    }
}


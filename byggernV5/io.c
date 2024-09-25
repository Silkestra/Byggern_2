#include "io.h"
#define adc_adr (0x1400)
#define BASE_ADDRESS 0x1000

int16_t convert(int16_t adr){
	int16_t gain = 1;
	int16_t offset = -9352;
	int16_t value = adr*gain+offset;
	int16_t normalized = value;
	
	if(value > 3){
		normalized = value * 100 / 119;
	} else if (value < -3){
		normalized = value * 100 / 136;
	}
	
	return normalized;
}

void setStates(IO* io){
	xmem_write(16, adc_adr, BASE_ADDRESS);
	
	io->joy_x = convert(xmem_read(adc_adr, BASE_ADDRESS));
	io->joy_y = convert(xmem_read(adc_adr, BASE_ADDRESS));
	io->slider_l = xmem_read(adc_adr,BASE_ADDRESS)-9216;
	io->slider_r = xmem_read(adc_adr, BASE_ADDRESS)-9216;
}

void getStates(IO* io){
	printf("Joystick x: %d %% \n", io->joy_x);
	printf("Joystick y: %d %% \n", io->joy_y);
	printf("Slider left: %d %% \n", io->slider_l);
	printf("Slider right: %d %% \n", io->slider_r);
	printf("\n");
}

int16_t get_joy_x(IO* io){
	return io->joy_x;
}

int16_t get_joy_y(IO* io){
	return io->joy_y;
}

int16_t get_slider_l(IO* io){
	return io->slider_l;
}

int16_t get_slider_r(IO* io){
	return io->slider_r;
}

void oled_init(void) {
	 xmem_write(0xae, 0, oled_command); // display off
	 xmem_write(0xa1, 0, oled_command); //segment remap
	 xmem_write(0xda, 0, oled_command); //common pads hardware: alternative
	 xmem_write(0x12, 0, oled_command);
	 xmem_write(0xc8, 0, oled_command); //common output scan direction:com63~com0
	 xmem_write(0xa8, 0, oled_command); //multiplex ration mode:63
	 xmem_write(0x3f, 0, oled_command);
	 xmem_write(0xd5, 0, oled_command); //display divide ratio/osc. freq. mode
	 xmem_write(0x80, 0, oled_command);
	 xmem_write(0x81, 0, oled_command); //contrast control
	 xmem_write(0x50, 0, oled_command);
	 xmem_write(0xd9, 0, oled_command); //set pre-charge period
	 xmem_write(0x21, 0, oled_command);
	 xmem_write(0x20, 0, oled_command); //Set Memory Addressing Mode
	 xmem_write(0x02, 0, oled_command);
	 xmem_write(0xdb, 0, oled_command); //VCOM deselect level mode
	 xmem_write(0x30, 0, oled_command);
	 xmem_write(0xad, 0, oled_command); //master configuration
	 xmem_write(0x00, 0, oled_command);
	 xmem_write(0xa4, 0, oled_command); //out follows RAM content
	 xmem_write(0xa6, 0, oled_command); //set normal display
	 xmem_write(0xaf, 0, oled_command); // display on
}

void OLED_goto_row(int8_t row){
	xmem_write(0xB0 + row, 0, oled_command);
	printf("%d\n", 0xB0+row);
	printf("%d\n", 0xB0 + row + 0xB5);
}

void OLED_goto_column(int8_t column){
	xmem_write(0x00 | (column & 0x0F), 0, oled_command);
	xmem_write(0x10 | (column >> 4), 0, oled_command);
}

void OLED_pos(int8_t row, int8_t column){
	OLED_goto_row(row);
	OLED_goto_column(column);
}

void OLED_clear_row(int8_t row){
	OLED_pos(row, 0);
	
	for(int i = 0; i < 128; i++){
		xmem_write(0b00000000,0,oled_data);
	}
}

void OLED_reset(void){
	OLED_pos(0, 0);
	for(int i=0; i<8; i++){
		OLED_clear_row(i);
		OLED_goto_row(i);
	}
}

void OLED_write_data(char ch){
	for(int j=0; j<8; j++){
		xmem_write(pgm_read_byte(&(font8[ch-32][j])), 0, oled_data);
	}
}
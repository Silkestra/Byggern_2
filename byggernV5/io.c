#include "io.h"
#define adc_adr (0x1400)

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
	xmem_write(16, adc_adr);
	
	io->joy_x = convert(xmem_read(adc_adr));
	io->joy_y = convert(xmem_read(adc_adr));
	io->slider_l = xmem_read(adc_adr)-9216;
	io->slider_r = xmem_read(adc_adr)-9216;
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
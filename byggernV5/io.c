#include "io.h"
#define adc_adr (0x1400)
#define BASE_ADDRESS 0x1000

int16_t convert(int16_t value_read){
	//int16_t offset = -9352;
	int16_t offset = -9310;
	int16_t value = value_read+offset;
	int16_t normalized = value;
	
	if(value > 3){
		//normalized = value * 100 / 119;
	} else if (value < -3){
		normalized = value * 100 / 136;
		//normalized = value * 100 / 124;
	}
	
	return normalized;
}

void find_joy_dir(IO* io){
	if((io->joy_x) > 60){
		if(io->joy_dir!=UP){
			io->changed_dir = 1;
		} else{
			io->changed_dir = 0;
		}
		io->joy_dir = UP;
		
	} else if((io->joy_x) < (-60)){
		if(io->joy_dir!=DOWN){
			io->changed_dir = 1;
		} else {
			io->changed_dir = 0;
		}
		io->joy_dir = DOWN;
	} else if((io->joy_y) > 60){
		if(io->joy_dir!=RIGHT){
			io->changed_dir = 1;
		} else {
			io->changed_dir = 0;
		}
		io->joy_dir = RIGHT;
	} else if((io->joy_y) < (-60)){
		if(io->joy_dir!=LEFT){
			io->changed_dir = 1;
		} else {
			io->changed_dir = 0;
		}
		io->joy_dir = LEFT;
	} else{
		if(io->joy_dir!=NEUTRAL){
			io->changed_dir=1;
		} else {
			io->changed_dir = 0;
		}
		io->joy_dir = NEUTRAL;
	}
}

void set_states(IO* io){
	xmem_write(16, adc_adr, BASE_ADDRESS);
	
	io->joy_x = convert(xmem_read(adc_adr, BASE_ADDRESS));
	io->joy_y = convert(xmem_read(adc_adr, BASE_ADDRESS));
	//io->slider_l = xmem_read(adc_adr,BASE_ADDRESS)-9216;
	//io->slider_r = xmem_read(adc_adr, BASE_ADDRESS)-9216;
	io->slider_l = xmem_read(adc_adr,BASE_ADDRESS);
	io->slider_r = xmem_read(adc_adr,BASE_ADDRESS);
	find_joy_dir(io);
	io->button_state = read_button(io);
	set_menu_ptr(io);
}

void get_states(IO* io){
	printf("Menu pointer: %d \n", io->menu_ptr);
	printf("Button: %d \n", io->button_state);
	printf("Joystick direction: %d \n", io->joy_dir);
	printf("Joystick x: %d %% \n", io->joy_x);
	printf("Joystick y: %d %% \n", io->joy_y);
	printf("Slider left: %d \n", io->slider_l);
	printf("Slider right: %d \n", io->slider_r);
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

bool read_button(IO* io){
	PORTB = (1 << PB2);
	DDRB = (0 << DDB2);
	SFIOR = (0 << PUD);
	//printf("Button: %d \n", button);
	bool button_state = false;
	
	if(PINB & (1 << PB2)){
		button_state = false;
	} else {
		button_state = true;
	}
	
	return button_state;
}

void set_menu_ptr(IO* io){
	if(io->joy_dir == DOWN && io->changed_dir == 1 && io->menu_ptr<7){
		OLED_pos(io->menu_ptr, 110);
		OLED_print("  ");
		
		io->menu_ptr += 1;
		
		if(io->menu_ptr == 1){
			io->menu_ptr += 1;
		}
		
	} else if(io->joy_dir == UP && io->changed_dir == 1 && io->menu_ptr>0){ //io->menu_ptr <= 0
		OLED_pos(io->menu_ptr, 110);
		OLED_print("  ");
		
		io->menu_ptr -= 1;
		
		if(io->menu_ptr == 1){
			io->menu_ptr -= 1;
		}
	}

	OLED_pos(io->menu_ptr, 110);
	OLED_print("<-");
}

void jump_to_menu(IO* io){
	if(io->button_state == true){
		OLED_reset();
		OLED_print_menu()
	}
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
	//printf("%d\n", 0xB0+row);
	//printf("%d\n", 0xB0 + row + 0xB5);
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

void OLED_print(char* ch){
	xmem_write(0x20, 0, oled_command); //Set Memory Addressing Mode
	xmem_write(0x02, 0, oled_command);
	
	while (*ch != ('\0')){
		OLED_write_data(*ch);
		ch += 1;
	}
}

int16_t array_size(const char* arr){
	return sizeof(arr) / sizeof(arr[0]);
}

void OLED_print_menu(char* menu_list[], int16_t length){
	OLED_reset();
	
	for(int i = 0; i < length; i++){
		if(i == 0){
			OLED_pos(0, 50);
			OLED_print(menu_list[i]);
			} else {
			OLED_pos(i+1, 10);
			OLED_print(menu_list[i]);
		}
	}
}

void OLED_home(char* menu_list[], int16_t length){
	OLED_print_menu(menu_list, length);
}
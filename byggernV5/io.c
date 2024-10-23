#include "io.h"
#define adc_adr (0x1400)
#define BASE_ADDRESS 0x1000

void io_init(IO* io){
	io->joy_x = 0;
	io->joy_y = 0;
	io->slider_l = 0;
	io->slider_r = 0;
	io->joy_dir = NEUTRAL;
	io->button_state = 0;
	io->menu_ptr = 2;
	io->changed_dir = false;
	io->current_node = NULL;
	io->button_used = false;
}

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
	button_clicked(io);
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
	DDRB &= ~(1 << DDB2);
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

void set_menu_ptr(IO *io){
	if(io->joy_dir == DOWN && io->changed_dir == 1 && io->menu_ptr<7){
		OLED_pos(io->menu_ptr, 110);
		OLED_print("  ");
		
		io->menu_ptr += 1;
		io->current_node = io->current_node->next_sibling;
		
		
	} else if(io->joy_dir == UP && io->changed_dir == 1 && io->menu_ptr>2){ //io->menu_ptr <= 0
		OLED_pos(io->menu_ptr, 110);
		OLED_print("  ");
		
		io->menu_ptr -= 1;
		io->current_node = io->current_node->previous_sibling;
	}

	OLED_pos(io->menu_ptr, 110);
	OLED_print("<-");
}


//begrense hvor pil går

void jump_to_menu(IO* io){
	if(io->button_state == true){
		OLED_reset();
		//OLED_print_menu();
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

menu_item *create_menu_item (const char *name, menu_item *parent){
	menu_item *new_item = (menu_item*) malloc(sizeof(menu_item));
	new_item->menu_name = strdup(name);
	new_item->parent = parent;
	new_item->first_child = NULL;
	new_item->previous_sibling=NULL;
	new_item->next_sibling = NULL;
	return new_item;
}

void add_submenu(menu_item *parent, const char *name){
	menu_item *new_item=create_menu_item(name,parent);
	if (parent->first_child==NULL){
		parent->first_child = new_item;
		} else {
		menu_item *current= parent->first_child;
		
		while(current->next_sibling!=NULL){
			current = current->next_sibling;
		}
		current->next_sibling=new_item;
		new_item->previous_sibling=current;
	}
}

void add_submenu_from_list(menu_item *parent, const char** menu_list, int8_t length){
	for(int i=0; i<length; i++){
		/*printf("%s: %s\n", parent->menu_name, menu_list[i]);*/
		add_submenu(parent, menu_list[i]);
	}
}


void OLED_print_menu_2(menu_item* parent){
	OLED_reset();
	OLED_pos(0, 50);
	OLED_print(parent->menu_name);
	menu_item *current=parent->first_child;
	int i=1;
	while(current!=NULL){
		OLED_pos(i+1, 10);
		OLED_print(current->menu_name);
		current=current->next_sibling;
		i++;
	}
}

/*menu_item *page_to_sibling(IO* io){
	int page_number = io->menu_ptr;
	for(int i=0; i<page_number; i++){
		
	} 
}*/

void menu_init(IO *io){
	menu_item *menu = create_menu_item("Home", NULL); 
	
	const char* home_menu[] = {"Menu 1", "Menu 2", "Menu 3", "Menu 4", "Menu 5"};
	add_submenu_from_list(menu, home_menu, 5);
	io->current_node = menu->first_child;
	const char* home_menu1[] = {"Menu 1", "Menu 2", "Menu 3", "Menu 4", "Back"};
	add_submenu_from_list(menu->first_child, home_menu1, 5);
	OLED_print_menu_2(menu);
	
}

void button_clicked(IO* io){
	if(io->button_used == false){
		io->button_used = true;
		return;
	}
	
	if(io->button_state){
		if(strcmp(io->current_node->menu_name, "Back") == 0){
			io->current_node = io->current_node->parent;
			io->current_node = io->current_node->parent;
		}
		
		OLED_print_menu_2(io->current_node);
		io->current_node = io->current_node->first_child;
		io->menu_ptr = 2;
	}
}


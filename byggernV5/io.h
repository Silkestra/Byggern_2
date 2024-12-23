#pragma once
#include "stdlib.h"
#include <stdint.h>
#include <stdio.h>
#include "fonts.h"
#include "xmem.h"
#include "CAN.h"
#include <util/delay.h>
#include <stdbool.h>
#define oled_command (0x1000)
#define oled_data (0x1200)

typedef enum {
	LEFT,
	RIGHT,
	UP,
	DOWN,
	NEUTRAL
} Joy_direction;

typedef struct {
	const char** home_menu;
	const char** menu1;
	const char** menu2;
	const char** menu3;
	const char** menu4;
} MENU_lists;

typedef struct{
	struct menu_item *parent; 
	struct menu_item *first_child;
	struct menu_item *previous_sibling;
	struct menu_item *next_sibling; 
	char *menu_name; 
	int8_t children_count; 
	} menu_item;

typedef struct {
	volatile int16_t joy_x;
	volatile int16_t joy_y;
	volatile int16_t slider_l;
	volatile int16_t slider_r;
	Joy_direction joy_dir;
	bool button_state;
	int8_t menu_ptr;
	bool changed_dir;
	menu_item* current_node;
	bool button_used;
	int8_t offset_x;
	int8_t offset_y;
	bool game_active;
	menu_item* death_node;
	menu_item* main_menu_node;
} IO;
	
menu_item *create_menu_item (const char *name, menu_item *parent);
void add_submenu(menu_item *parent, const char *name);
void add_submenu_from_list(menu_item *parent, const char** menu_list, int8_t length);

//--------------------
void io_init(IO* io);
void find_joy_dir(IO* io);
void set_states(IO* io);
void get_states(IO* io);
int16_t convert_x(IO* io, int16_t value_read);
int16_t convert_y(IO* io, int16_t value_read);
int16_t get_joy_x(IO* io);
int16_t get_joy_y(IO* io);
int16_t get_slider_l(IO* io);
int16_t get_slider_r(IO* io);
bool read_button(IO* io);
void set_menu_ptr(IO* io);
void jump_to_menu(IO* io);
can_message send_joy_pos(IO* io);
void joy_init(IO* io);
void play_game(IO* io);


// --------------------OLED-----------------------------

void oled_init(void);
void OLED_reset();
void OLED_goto_row(int8_t row);
void OLED_goto_column(int8_t column);
void OLED_clear_row(int8_t row);
void OLED_pos(int8_t row, int8_t column);
void OLED_write_data(char ch); //volatile
void OLED_print(char* ch);
int16_t array_size(const char* arr);
void OLED_home(char* menu_list[], int16_t length);
void OLED_print_menu(char* menu_list[], int16_t length);

//MENU

void menu_init(IO *io);
void button_clicked(IO* io);
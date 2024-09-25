#pragma once
#include "stdlib.h"
#include <stdint.h>
#include <stdio.h>
#include "fonts.h"
#include "xmem.h"
#define oled_command (0x1000)
#define oled_data (0x1200)

typedef struct {
	volatile int16_t joy_x;
	volatile int16_t joy_y;
	volatile int16_t slider_l;
	volatile int16_t slider_r;
} IO;

void setStates(IO* io);
void getStates(IO* io);
int16_t convert(int16_t);
int16_t get_joy_x(IO* io);
int16_t get_joy_y(IO* io);
int16_t get_slider_l(IO* io);
int16_t get_slider_r(IO* io);

void oled_init(void);
void OLED_reset();
//void OLED_home();
void OLED_goto_row(int8_t row);
void OLED_goto_column(int8_t column);
void OLED_clear_row(int8_t row);
void OLED_pos(int8_t row, int8_t column);
void OLED_write_data(char ch); //volatile
//void OLED_print(char*);
//void OLED_set_brightness(lvl);
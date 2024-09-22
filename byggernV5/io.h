#pragma once
#include "stdlib.h"
#include <stdint.h>
#include <stdio.h>

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
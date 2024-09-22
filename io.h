#include <util/delay.h>
#include <stdlib.h>

typedef struct {
	uint16_t joy_x;
	uint16_t joy_y;
	uint16_t slider_r;
	uint16_t slider_l;
	
} io;

void setStates(io* io);
void getState(io* state);
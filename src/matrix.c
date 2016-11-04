#include "matrix.h"

uint8_t reverse_bits(uint8_t byte);
void init_matrix(void)
{
	GND_DDR = 0xff;
	LED_DDR = 0xff;
}

void write_column(uint8_t data, uint8_t column)
{
	//pulling a GND pin low and keeping all the others pulled high
	//this will cause that specific column of LEDs to turn on if their respective pins are pulled high
	//the image is drawn one column at a time, except it happens faster than what the human eye can keep up with
	GND_PORT = ~(1 << column);
	LED_PORT = data;
}

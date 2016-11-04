#ifndef MATRIX_H
#define MATRUX_H
#include <avr/io.h>
#include <stdint.h>

#define GND_PORT	PORTD
#define LED_PORT	PORTB
#define GND_DDR		DDRD
#define LED_DDR		DDRB

void init_matrix(void);
void write_column(uint8_t data, uint8_t column);

#endif

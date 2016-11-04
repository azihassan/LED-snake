#ifndef DPAD_H
#define DPAD_H
#include <stdbool.h>
#include <avr/io.h>

//to do : add interrupt handling
#define DPAD		PINC
#define DPAD_DDR	DDRC
#define UP		1 << PC0
#define DOWN		1 << PC3
#define LEFT		1 << PC1
#define RIGHT		1 << PC2

void init_dpad(void);
bool button_pressed(uint8_t button); //usage : button_pressed(LEFT). It is possible to check for two or more buttons by ORing them together even though it doesn't make sense for a snake game, eg. buttton_pressed(UP | LEFT)
bool any_pressed(void); //returns true if the player pressed any of buttons defined above

#endif

#ifndef SNAKE_H
#define SNAKE_H
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

struct Position
{
	int8_t x; //3 bits would be enough to represent a width of 8 but oh well
	int8_t y;
};

enum Cell
{
	EMPTY, SNAKE_LEFT, SNAKE_RIGHT, SNAKE_UP, SNAKE_DOWN, FOOD, BONUS
};

enum State
{
	GAME, MENU
};

struct Snake //89 bytes
{
	struct Position body[64]; //ideally it should be world.width * world.height. maybe malloc() it inside init_game() ?
	struct Position head, tail;
	uint8_t belly; //score
	uint8_t length;
};

struct World
{
	enum Cell grid[64]; //again, this should ideally be dynamically allocated depending on the size of the LED matrix
	uint8_t height;
	uint8_t width;
	enum State state;
};


void init_game(void);
void handle_keypress(const struct Position* _direction);
void tick(void);

enum State get_state(void);
void set_state(enum State state);
const struct World * get_world(void); //public because it doesn't mutate the grid (or does it ?), useful for drawing
enum Cell cell_at(const struct Position *position); //ditto
const struct Snake * get_snake(void);
void print_info();
#endif

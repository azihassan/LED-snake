#define F_CPU 1000000UL
#include <util/delay.h>
#include "snake.h"
#include "matrix.h"
#include "dpad.h"

void render(int repetitions);

int main(void)
{
	init_game();
	set_state(GAME);
	struct Position direction = {.x = 0, .y = 1};
	init_matrix();
	init_dpad();
	uint8_t tick_interval = 10;
	for(;;)
	{
		if(get_state() == GAME)
		{
			/* this should be put in a PCINT interrupt vector*/
			if(button_pressed(UP))
			{
				direction.x = 0;
				direction.y = -1;
			}
			else if(button_pressed(DOWN))
			{
				direction.x = 0;
				direction.y = 1;
			}
			else if(button_pressed(LEFT))
			{
				direction.x = -1;
				direction.y = 0;
			}
			else if(button_pressed(RIGHT))
			{
				direction.x = 1;
				direction.y = 0;
			}
			handle_keypress(&direction);
			/* pin change interrupt end */

			//this should be put in a timer interrupt
			render(5);

			_delay_ms(8);
			if(tick_interval-- == 0)
			{
				tick_interval = 10;
				tick();
			}
		}
		else if(get_state() == MENU)
		{
			write_column(0b11111111, 0);
			write_column(0b11111111, 1);
			write_column(0b11111111, 2);
			write_column(0b11111111, 3);
			write_column(0b11111111, 4);
			write_column(0b11111111, 5);
			write_column(0b11111111, 6);
			write_column(0b11111111, 7);
			if(any_pressed())
			{
				set_state(GAME);
				init_game();
			}
		}
	}
}

void render(int repetitions)
{
	while(repetitions--) //it only displays correctly when multiplexing repeatedly
	{
		const struct World *world = get_world();
		uint8_t col, row;
		for(col = 0; col < world->width; col++)
		{
			uint8_t data = 0;
			for(row = 0; row < world->height; row++)
			{
				struct Position cell = {.x = col, .y = row};
				if(cell_at(&cell) != EMPTY)
					data |= 1 << row;
			}
			write_column(data, col);
		}
	}
}

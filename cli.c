#include <stdio.h>
#include "snake.h"

void render();
int main(void)
{
	system("cls");
	init_game();
	set_state(GAME);
	struct Position direction = {.x = -1, .y = 0};
	for(;;)
	{
		if(get_state() == GAME)
		{
			handle_keypress(&direction);
			render();
			tick();
			char c = fgetc(stdin);
			char eol = fgetc(stdin);
			switch(c)
			{
				case 'j': direction.y = 0; direction.x = -1; break;
				case 'l': direction.y = 0; direction.x = 1; break;
				case 'k': direction.y = 1; direction.x = 0; break;
				case 'i': direction.y = -1; direction.x = 0; break;
				default: break;
			}
			system("cls");
		}
	}
	return 0;
}

void render()
{
	const struct World *world = get_world();
	uint8_t col, row;
	print_info();
	for(row = 0; row < world->height; row++)
	{
		for(col = 0; col < world->width; col++)
		{
			const struct Position cell = {.x = col, .y = row};
			const struct Snake *snake = get_snake();
			char c;
			switch(cell_at(&cell))
			{
				case EMPTY: c = ' '; break;
				case FOOD: c = 'f'; break;
				case BONUS: c = 'F'; break;
				case SNAKE_UP: c = '^'; break;
				case SNAKE_DOWN: c = 'v'; break;
				case SNAKE_LEFT: c = '<'; break;
				case SNAKE_RIGHT: c = '>'; break;
				default: c = 'o'; break;
			}
			if(snake->head.x == cell.x && snake->head.y == cell.y)
				printf("H");
			else if(snake->tail.x == cell.x && snake->tail.y == cell.y)
				printf("T");
			else
				printf("%c", c);
		}
		printf("\n");
	}
}

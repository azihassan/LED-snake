#include "snake.h"

//these variables and functions are private to the current module.
//exposing them to the outside world doesn't make sense
//it is only possible to interact with them through the functions defined in snake.h
static struct Snake snake;
static struct World world;
static struct Position food, direction;

static void place_food(void);
static void eat_food(const struct Position *head, enum Cell food_type);
static void move_head(const struct Position *head);
static void move_tail(void);

//helpers
static void handle_wrapping(struct Position *position); //handles screen wrapping
static void set_cell(const struct Position *position, enum Cell value);
static enum Cell position_to_direction(const struct Position *position); //converts a {-1|0|1, -1|0|1} struct to a SNAKE_direction enum
static struct Position direction_to_position(enum Cell direction); //modifies the given position so that it points to a specific direction

const struct Snake * get_snake(void)
{
	return &snake;
}
void init_game(void)
{
	srand(18979);//to do : write it to EEPROM and next time the program is ran, read it and increment it
	direction.x = 1;
	direction.y = 0;
	world.height = 8;
	world.width = 8;
	snake.belly = 0;

	uint8_t c;
	for(c = 0; c < world.width * world.height; c++)
		world.grid[c] = EMPTY;
	struct Position pos = {world.width / 2, world.height / 2};
	set_cell(&pos, SNAKE_LEFT);
	snake.head = pos;

	pos = (struct Position) {world.width / 2 + 1, world.height / 2};
	set_cell(&pos, SNAKE_LEFT);

	pos = (struct Position) {world.width / 2 + 2, world.height / 2};
	set_cell(&pos, SNAKE_LEFT);
	snake.tail = pos;

	place_food();
	handle_keypress(&direction);
}

void handle_keypress(const struct Position* _direction)
{
	//ignore directions that don't make sense
	if(_direction->x == _direction->y) //{1, 1}, {-1, 1}, {0, 0}
		return;
	if(_direction->x != 0 && _direction->y != 0) //{-1, 1}, {1, -1}
		return;
	//ignore conflicting directions like making the snake go up when it's going down
	//or making it go left when it's moving right or vice versa
	if(_direction->x + direction.x == 0)
		return;
	if(_direction->y + direction.y == 0)
		return;
	direction = *_direction;
}

static void eat_food(const struct Position *head, enum Cell food_type)
{
	move_head(head);
	snake.belly += food_type == FOOD ? 1 : 5;
	place_food();
}

void tick(void)
{
	set_cell(&snake.head, position_to_direction(&direction));
	struct Position head = {
		.x = direction.x + snake.head.x,
		.y = direction.y + snake.head.y
	};
	handle_wrapping(&head);
	enum Cell cell = cell_at(&head);
	switch(cell)
	{
		case FOOD:
		case BONUS:
			eat_food(&head, cell);
		break;
		case EMPTY:
			move_head(&head);
			move_tail();
		break;
		default:
			if(head.x == snake.tail.x && head.y == snake.tail.y)
			{
				printf("Inside tick()'s default label");
				move_head(&head);
				move_tail();
			}
			else
			{
				world.state = MENU;
			}
		break;
	}
}

static void place_food(void)
{
	//to do : the following algorithm will be slow when the snake occupies a large portion of the grid
	//it might even freeze the program
	do
	{
		food.x = rand() % world.width;
		food.y = rand() % world.height;
	}
	while(cell_at(&food) != EMPTY);
	set_cell(&food, FOOD);
}

static void move_head(const struct Position *head)
{
	if(head->x == snake.tail.x && head->y == snake.tail.y)
	{
		struct Position new_tail = direction_to_position(cell_at(&snake.tail));
		struct Position tail = {.x = snake.tail.x + new_tail.x, .y = snake.tail.y + new_tail.y};
		handle_wrapping(&tail);
		snake.head = snake.tail;
		return;
	}
	enum Cell head_direction = position_to_direction(&direction);
	snake.head = *head;
	set_cell(&snake.head, head_direction);
}

static void move_tail(void)
{
	struct Position tail = snake.tail;
	enum Cell tail_direction = cell_at(&snake.tail);
	switch(tail_direction)
	{
		case SNAKE_UP	: tail.y--; break;
		case SNAKE_DOWN	: tail.y++; break;
		case SNAKE_LEFT	: tail.x--; break;
		case SNAKE_RIGHT: tail.x++; break;
		default: printf("Impossible : %u\n", tail_direction); break;
	}

	handle_wrapping(&tail);
	//for when the tail and the head are in the same position
	//eg. a horizontal snake that occupies 8 squares:
	if(snake.head.x == snake.tail.x && snake.head.y == snake.tail.y)
		set_cell(&snake.tail, tail_direction);
	else
		set_cell(&snake.tail, EMPTY);
	snake.tail = tail;
}

enum Cell cell_at(const struct Position *position) //probably belongs in a utils.h file
{
	return world.grid[position->y * world.width + position->x];
}

static void set_cell(const struct Position *position, enum Cell value) //ditto
{
	world.grid[position->y * world.width + position->x] = value;
}

enum State get_state(void)
{
	return world.state;
}

void set_state(enum State state)
{
	world.state = state;
}

const struct World * get_world(void)
{
	return &world;
}

static void handle_wrapping(struct Position *position)
{
	if(position->x < 0)
		position->x = world.height - 1;
	else if(position->x >= world.height)
		position->x = 0;
	if(position->y < 0)
		position->y = world.height - 1;
	else if(position->y >= world.height)
		position->y = 0;
}

void print_info()
{
	printf("Head : %d, %d\n", snake.head.x, snake.head.y);
	printf("Tail : %d, %d\n", snake.tail.x, snake.tail.y);
}

static enum Cell position_to_direction(const struct Position *position)
{
	if(position->x == -1)
		return SNAKE_LEFT;
	if(position->x == 1)
		return SNAKE_RIGHT;
	if(position->y == -1)
		return SNAKE_UP;
	return SNAKE_DOWN;
}

static struct Position direction_to_position(enum Cell direction)
{
	struct Position position = {0, 0};
	switch(direction)
	{
		case SNAKE_UP	: position.y--; break;
		case SNAKE_DOWN	: position.y++; break;
		case SNAKE_LEFT	: position.x--; break;
		case SNAKE_RIGHT: position.x++; break;
		default: printf("Impossible : %u\n", direction); break;
	}
	return position;
}

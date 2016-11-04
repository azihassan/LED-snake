#include "dpad.h"

void init_dpad(void)
{
	DPAD_DDR &= ~(UP | DOWN | LEFT | RIGHT);
}

bool button_pressed(uint8_t button)
{
	return DPAD & button;
}

bool any_pressed(void)
{
	return DPAD & (UP | DOWN | LEFT | RIGHT);
}

# Snake on an Atmega328p MCU

It's a work in progress, but it's 90% finished.

Written in C and compiled with avr-gcc.

![Demonstration under Proteus](http://imgur.com/GYSCUwc.gif "Demonstration under Proteus")

It's also possible to run a CLI version by building with `make build_cli` and running the resulting binary file.

A few notes about the code :

The `Snake` structure only keeps track of the position of the snake's head and tail. The other body parts are stored in the `grid` array of the `World` structure in the form of directions (`SNAKE_UP`, `SNAKE_DOWN`, `SNAKE_LEFT` and `SNAKE_RIGHT`).

Whenever the player presses a button, the main loop creates a {x, y} structure and passes it to the `handle_keypress()` function to validate and then update the internal `_direction` structure. Periodic calls to the `tick()` function move update the grid that is then displayed on the LED matrix with the help of the functions declared in matrix.h.

The tail moves by reading the value of the current tail from the grid and then, depending on the direction it points to, changing its x and y coordinates accordingly. The cell located at the old `snake.tail` position is then updated with a value of `EMPTY`. The movement of the head is easier, it is as simple as adding the `_direction` struct to the `snake.head` struct and putting its direction in the grid.

Note that I went out of my way to make snake.h and snake.c independent from the rest of the headers. As a consequence, it is possible to use it with other forms of IO. This was especially useful when I ran into bugs whose source I couldn't determine. All I had to do was include snake.h in cli.c and compile it with the `make build_cli` command.

Debugging embedded software is something I have very little experience with. My first reflex had been to write debug information to the serial interface to see what it is exactly that's causing the program to malfunction. Unfortunately, the USART pins were already in use, so I had to resort to following the aforementioned approach.

Random thoughts about loose coupling :

I wrote this project with loose coupling in mind from start to finish. Because of this, I ran into some interesting situations that required me to pause and think for a while about the best course of action. One such case is when I wanted to display the grid. Writing to the pins of the MCU in the `snake` module - whether it be directly or through the `matrix` module - would make it dependant on this specific microcontroller, which is something that I had tried to avoid. Furthermore, was rendering even the responsibility of a module that is only supposed to handle the logic of the game ? I eventually made the decision to go against the principle of encapsulation by exposing an internal component - in this case, the `World` structure - to the outside world. In order to control the damage, I only returned a const pointer so that the developer is neither able to modify the structure nor the underlining array. You'll notice that even the `set_cell()` function is static, which makes it visible only to the functions of the `snake` module.

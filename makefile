#we makefile now
#no seriously I just read about makefiles like 5 minutes ago
GCC = gcc
AVR_GCC = C:\WinAVR-20100110\bin\avr-gcc
AVR_SIZE = C:\WinAVR-20100110\bin\avr-size 
CFLAGS = -Wall -Os
MCU = atmega328p
INCLUDE = include
SOURCES = main.c src\dpad.c src\matrix.c src\snake.c
SOURCES_CLI = cli.c src\snake.c
EXE = main
HEX = main.c

build: compile eeprom size

compile: $(SOURCES)
	cls #makefiles on windows ? blasphemy !
	echo Compiling for the $(MCU) microcontroller
	$(AVR_GCC) -I$(INCLUDE) -mmcu=$(MCU) $(CFLAGS) $(SOURCES) -o $(EXE) 

#since snake.c doesn't depend on MCU-related functions, it is possible to use it with another front-end
#front-end being of course a fancy word for "input and output". in this case, respectively, keyboard and CLI.
build_cli: $(SOURCES)
	cls
	echo Compiling for the command line
	$(GCC) -I$(INCLUDE) $(CFLAGS) $(SOURCES_CLI) -o $(EXE) 

eeprom: $(EXE) $(HEX)
	C:\WinAVR-20100110\bin\avr-objcopy -O ihex -R .eeprom main main.hex

size: $(EXE)
	$(AVR_SIZE) -C --mcu=$(MCU) $(EXE)

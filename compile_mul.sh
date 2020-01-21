avr-gcc -g -Os -mmcu=atmega328p -c $1.c
avr-gcc -g -Os -mmcu=atmega328p -c $2.c
avr-gcc -g -mmcu=atmega328p $1.o $2.o -o $1.elf
avr-objcopy -j .text -j .data -O ihex $1.elf $1.hex
avr-size --format=avr --mcu=atmega328p $1.elf
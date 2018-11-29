avr-gcc -g -Os -mmcu=$2 -c $1.c
avr-gcc -g -mmcu=$2 -o $1.elf $1.o
avr-objcopy -j .text -j .data -O ihex $1.elf $1.hex
avr-size --format=avr --mcu=$2 $1.elf
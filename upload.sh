avrdude -c arduino -p atmega328p -P $1 -b 115200 -U flash:w:$2.hex

echo "removing...."
echo $2.o
rm $2.o 
echo $2.elf
rm $2.elf 
echo $2.hex
rm $2.hex
avrdude -c arduino -p $1 -P $2 -b 115200 -U flash:w:$3.hex

echo "removing...."
echo $3.o
rm $3.o 
echo $3.elf
rm $3.elf 
echo $3.hex
rm $3.hex
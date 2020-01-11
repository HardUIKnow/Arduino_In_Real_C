/*
Building an application for using i2c component
This is a starting point build for example
*/
// define microcontroller used

#ifndef __AVR_ATmega328P__
#define __AVR_ATmega328P__
#endif

#include<avr/io.h>
void i2c_start();
void i2c_connect(uint8_t adress);
void i2c_transmit(uint8_t data);
void i2c_stop();

int main(int argc, char const *argv[])
{
    i2c_start();
    i2c_connect(0x27);
    i2c_transmit(0x40);
    i2c_stop();

    while(1){
        
    }
    return 0;
}

void i2c_start(){
    TWCR = (1 << TWINT)|(1 << TWSTA)|(1 << TWEN);
    while(!(TWCR & (1 << TWINT)));
    if((TWSR & 0xF8) != 0x08);
        //ERROR();
}

void i2c_connect(uint8_t adress){
    TWDR = (uint8_t) adress;
    TWCR = (1 << TWINT) | (1 << TWEN);
    while(!(TWCR & (1 << TWINT)));
    if((TWSR & 0xF8) != 0x18);
        //ERROR();
}

void i2c_transmit(uint8_t data){
    TWDR = data;
    TWCR = (1 << TWINT) | (1 << TWEN);
    while(!(TWCR & (1 << TWINT)));
    if((TWSR & 0xF8) != 0x28);
        //ERROR();
}

void i2c_stop(){
    TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);
}

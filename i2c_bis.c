/*
Building an application for using i2c component
This is a starting point build for example
*/
// define microcontroller used

#ifndef __AVR_ATmega328P__
#define __AVR_ATmega328P__
#endif



#define F_CPU 16000000UL
#define BAUDRATE 9600
#define BAUD_PRESCALLER (((F_CPU / (BAUDRATE * 16UL))) - 1)
#include<avr/io.h>
#include<util/delay.h>
#include "twi.h"
#define I2C_SDA_PORT	  D	    /* The SDA port. Use capital letter (A,B,C,D... etc.) */
#define SDA_PIN 	  4	    /* The SDA port pin */

#define I2C_SCL_PORT	  D	    /* The SCL port. Use capital letter (A,B,C,D... etc.) */
#define SCL_PIN 	  5	    /* The SCL port pin */
/*
   The I2C_DELAY_TIME normally is 5 microseconds for a 100 Khz I2C bus ( (1/100000)/2 ).
   but due to bus capacitance and device responce time in my application i use 50 microseconds.
   The I2C_TIMEOUT_TIME is set to whatever is the maximum time you think your device will take
   to perform the requested task. After that time the i2c_transmit function will return
   a "I2C_ERROR_DEVICE_NOT_RESPONDING" or "I2C_ERROR_DEVICE_BUSY" error code. 
*/ 
#define I2C_DELAY_TIME	  50	   /* in microseconds (max over 1 second) */
#define I2C_TIMEOUT_TIME  1000	   /* in microseconds (max over 1 second) */
/* Register name forming */
#define I2C_SDA_OUT_REG   CONCAT1(PORT, I2C_SDA_PORT)
#define I2C_SCL_OUT_REG   CONCAT1(PORT, I2C_SCL_PORT)
#define I2C_SDA_DDR_REG   CONCAT1(DDR, I2C_SDA_PORT)
#define I2C_SCL_DDR_REG   CONCAT1(DDR, I2C_SCL_PORT)
#define I2C_SDA_PIN_REG   CONCAT1(PIN, I2C_SDA_PORT)
#define I2C_SCL_PIN_REG   CONCAT1(PIN, I2C_SCL_PORT)

/* Conversion of microseconds to the right value for the delay function */
#define I2C_DELAY	  ( (I2C_DELAY_TIME*(F_CPU/60000))/100 )  
#define I2C_TIMEOUT	  ( (I2C_TIMEOUT_TIME*(F_CPU/60000))/100 ) 

/* Pin states */
#define SCL_1() 	  cbi(DDRD, PD5)
#define SCL_0() 	  sbi(DDRD, PD5)
#define SDA_1() 	  cbi(DDRD, PD4)
#define SDA_0() 	  sbi(DDRD, PD4)

#define RELEASE_I2C_BUS() { SCL_1(); SDA_1(); }

//Declaration of our functions
void USART_init(void);
unsigned char USART_receive(void);
void USART_send( unsigned char data);
void USART_putstring(char* StringPtr);
uint8_t srf_08_writeByte(uint16_t u8addr, uint8_t *u8data);

char String[]="Hello world!!";    //String[] is in fact an array but when we put the text between the " " symbols the compiler threats it as a String and automatically puts the null termination character in the end of the text
volatile char receiveByte;


int main(int argc, char const *argv[])
{
    USART_init();
    RELEASE_I2C_BUS();
	delay(I2C_TIMEOUT);
	TWIStart();
	delay(I2C_TIMEOUT);
	TWIStop();
	delay(I2C_TIMEOUT);
    TWIInit();
    _delay_ms(20);
    if(srf_08_writeByte(0x70, 0x51) != 0){
        USART_putstring("Sensor reached and ranged set !!!\n");
    }

    while(1){
        
    }
    return 0;
}

uint8_t srf_08_writeByte(uint16_t u8addr, uint8_t *u8data)
{
    //uint8_t databyte;
    TWIStart();
    _delay_ms(20);
    if (TWIGetStatus() != 0x08)
        USART_putstring("Start not ok\n");
    //send the address
        _delay_ms(20);

    TWIWrite((uint8_t)(u8addr));
    if (TWIGetStatus() != 0x28)
        USART_putstring("Adress not ok\n");
    //write byte to eeprom
        _delay_ms(20);

    TWIWrite(u8data);

    if (TWIGetStatus() != 0x28)
        USART_putstring("Data not ok\n");
    TWIStop();
    return 1;
}

void USART_init(void){

	UBRR0H = (uint8_t)(BAUD_PRESCALLER>>8);
	UBRR0L = (uint8_t)(BAUD_PRESCALLER);
	UCSR0B = (1<<RXEN0)|(1<<TXEN0);
        UCSR0B |= (1<<RXCIE0);
	UCSR0C = (3<<UCSZ00);
}

unsigned char USART_receive(void){

	while(!(UCSR0A & (1<<RXC0)));
	return UDR0;

}

void USART_send( unsigned char data){

	while(!(UCSR0A & (1<<UDRE0)));
	UDR0 = data;

}

void USART_putstring(char* StringPtr){

	while(*StringPtr != 0x00){
		USART_send(*StringPtr);
	StringPtr++;}

}
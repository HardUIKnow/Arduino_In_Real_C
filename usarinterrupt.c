/*
  Serial input output with interrupt routine
  
  This example code is in the public domain.
 */
 #ifndef __AVR_ATmega328P__
#define __AVR_ATmega328P__
#endif

#include <avr/io.h>
#include <avr/interrupt.h>

#define F_CPU 16000000UL
#define BAUDRATE 9600
#define BAUD_PRESCALLER (((F_CPU / (BAUDRATE * 16UL))) - 1)

//Declaration of our functions
void USART_init(void);
unsigned char USART_receive(void);
void USART_send( unsigned char data);
void USART_putstring(char* StringPtr);

char String[]="Hello world!!";    //String[] is in fact an array but when we put the text between the " " symbols the compiler threats it as a String and automatically puts the null termination character in the end of the text
volatile char receiveByte;
volatile int flag = 0;
int main(void){
	USART_init();        //Call the USART initialization code
	USART_putstring(String);
sei();
	while(1){        //Infinite loop
/*
		receiveByte = USART_receive();
		USART_send(receiveByte);
		USART_send('\n');
*/
          if(flag == 1){
            USART_putstring("received....");
            flag =0;  
        }
	}
	return 0;
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

ISR(USART_RX_vect){
    receiveByte = UDR0 ;
  	USART_send(receiveByte);
  	//check if there is a limiter
  	if(UDR0 == '\r'){
  		flag = 1;
  	}
}
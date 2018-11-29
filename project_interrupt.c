/* Arduino Serial Input / output
* exemple in real C.
* In this file you can view how to get started with
* USART Init
* USART Received/Send data
* USART PutString
* How to use it : A led is connected to (arduino pin 13) PINB5
* Sending 'H' via serial will pull on the led
* Sending anything else will pull off the led
*/

// Include
#include <avr/io.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#define F_CPU 16000000UL // Clock Speed
#include <util/delay.h>

                                         /* Serial definition */
// BAUDRATE and prescaler definition
#define BAUDRATE 9600										//Baudrate to 9600
#define BAUD_PRESCALLER (((F_CPU / (BAUDRATE * 16UL))) - 1)	//Baudrate configuration
#define target_time  0.25                                   // time definition
#define timer_resolution  0.000064                          // timer resolution

//Declaration of our functions
void USART_init(void);
unsigned char USART_receive(void);
void USART_send( unsigned char data);
void USART_putstring(char* StringPtr);
void adc_init(void);            //Function to initialize/configure the ADC
uint16_t read_adc(uint8_t channel);    //Function to read an arbitrary analogic channel/pin

                                        /* ADC definition */
uint16_t adc_value;            //Variable used to store the value read from the ADC
char buffer[5];                //Output of the itoa function
uint8_t channel=0;                //Variable for the for() loop

                                        /* other */
char String[]="Connected !!";    //String[] is in fact an array but when we put the text between the " " symbols the compiler threats it as a String and automatically puts the null termination character in the end of the text
char receiveByte;
volatile int flag = 0;

                                        /* main function */
int main(void){
// initialize Timer1
cli();          // disable global interrupts
TCCR1A = 0;     // set entire TCCR1A register to 0
TCCR1B = 0;     // same for TCCR1B

// set compare match register to desired timer count:
OCR1A = ((target_time) / (timer_resolution))-1;

// turn on CTC mode:
TCCR1B |= (1 << WGM12);

// Set CS10 and CS12 bits for 1024 prescaler:
TCCR1B |= (1 << CS10);
TCCR1B |= (1 << CS12);

// enable timer compare interrupt:
TIMSK1 |= (1 << OCIE1A);
adc_init();        //Setup the ADC
USART_init();        				//Call the USART initialization code
USART_putstring(String);			//Sending the "hello world" String
USART_send('\r');
USART_send('\n');
DDRB |= (1<<PINB1);				//Pin 5 of Port B pushed to OUTPUT
// enable global interrupts:
sei();
	while(1){        					//Infinite loop

        receiveByte = USART_receive();		//Check received Byte
	        USART_send(receiveByte);			//Echo the received byte
	        if(receiveByte == 'H'){			//Checking if received byte is an 'H'
                PORTB |= (1<<PINB1);		//if so pull led on
            } else if(receiveByte == 'L'){
                PORTB &= ~(1<<PINB1);		//else pull it off
            }      
    }
return 0;
}

void USART_init(void){
 
 UBRR0H = (uint8_t)(BAUD_PRESCALLER>>8);
 UBRR0L = (uint8_t)(BAUD_PRESCALLER);
 UCSR0B = (1<<RXEN0)|(1<<TXEN0);
 UCSR0C = (3<<UCSZ00);
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

unsigned char USART_receive(void){

while(!(UCSR0A & (1<<RXC0)));
    return UDR0;
}

void adc_init(void){
 ADCSRA |= ((1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0));    //16Mhz/128 = 125Khz the ADC reference clock
 ADMUX |= (1<<REFS0);                //Voltage reference from Avcc (5v)
 ADCSRA |= (1<<ADEN);                //Turn on ADC
 //ADCSRA |= (1<<ADSC);                //Do an initial conversion because this one is the slowest and to ensure that everything is up and running
}
 
uint16_t read_adc(uint8_t channel){
 ADMUX &= 0xF0;                    //Clear the older channel that was read
 ADMUX |= channel;                //Defines the new ADC channel to be read
 ADCSRA |= (1<<ADSC);                //Starts a new conversion
 while(ADCSRA & (1<<ADSC));            //Wait until the conversion is done
 return ADCW;                    //Returns the ADC value of the chosen channel
}
/*
ISR(USART_RX_vect){
    cli();
    receiveByte = UDR0 ;
    USART_send(receiveByte);
    if(receiveByte == 'H'){			//Checking if received byte is an 'H'
        PORTB |= (1<<PINB5);		//if so pull led on
    } else {
        PORTB &= ~(1<<PINB5);		//else pull it off
    }
  	//check if there is a limiter
  	if(UDR0 == '\r'){
  		flag = 1;
  	}
      sei();
}
*/
ISR(TIMER1_COMPA_vect)
{
ADCSRA |= (1<<ADSC);                //Do an initial conversion because this one is the slowest and to ensure that everything is up and running
USART_putstring("Receiving on channel 0: ");        //Send the converted value to the terminal        
adc_value = read_adc(channel);        //Read one ADC channel
itoa(adc_value, buffer, 10);        //Convert the read value to an ascii string
USART_putstring(buffer);        //Send the converted value to the terminal
USART_send('\r');
USART_send('\n');        //Send the converted value to the terminal
ADCSRA |= (0<<ADSC);                //Do an initial conversion because this one is the slowest and to ensure that everything is up and running
}
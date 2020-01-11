/* Arduino Serial Input / output
* exemple in real C.
* In this file you can view how to get started with
* USART Init
* USART Received/Send data
* USART PutString
* ADC is set on channel 0
* PWM is set via timer 0 on port D pin B6
* A led is connected on PINB6 under the effect of PWM
* How to use it : A led is connected to (arduino pin 13) PINB5
* Sending 'H' via serial will pull on the led
* Sending 'L' will pull off the led
*/

// Include
#ifndef __AVR_ATmega328P__
#define __AVR_ATmega328P__
#endif


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
long map(long x, long in_min, long in_max, long out_min, long out_max);
void USART_init(void);
unsigned char USART_receive(void);
void USART_send( unsigned char data);
void USART_putstring(char* StringPtr);
void adc_init(void);            //Function to initialize/configure the ADC
uint16_t read_adc(uint8_t channel);    //Function to read an arbitrary analogic channel/pin

                                        /* ADC definition */
uint16_t adc_value;            //Variable used to store the value read from the ADC
unsigned char buffer[5];                //Output of the itoa function
uint8_t channel=0;                //Variable for the for() loop
long time_check=0;
                                        /* other */
unsigned char String[]="Connected !!";    //String[] is in fact an array but when we put the text between the " " symbols the compiler threats it as a String and automatically puts the null termination character in the end of the text
unsigned char receiveByte;
volatile uint16_t flag = 0;

                                        /* main function */
int main(void){
// initialize Timer1
cli();          // disable global interrupts
TCCR1A = 0;     // set entire TCCR1A register to 0
TCCR1B = 0;     // same for TCCR1B

// **** pwm set up ****
TCCR0A |= (1 << COM0A1);     // set none-inverting mode
TCCR0A |= (1 << WGM01) | (1 << WGM00);     // set fast PWM Mode
TCCR0B |= (1 << CS01);    // set prescaler to 8 and starts PWM

OCR0A = 128;  // set PWM for 50% duty cycle default value

// **** adc set up ****
// turn on CTC mode:
TCCR1B |= (1 << WGM12);

// Set CS10 and CS12 bits for 1024 prescaler:
TCCR1B |= (1 << CS10);
TCCR1B |= (1 << CS12);

OCR1A = ((target_time) / (timer_resolution))-1; // set compare match register to desired timer count:

// enable timer compare interrupt:
TIMSK1 |= (1 << OCIE1A);
adc_init();        //Setup the ADC
USART_init();        				//Call the USART initialization code
USART_putstring(String);			//Sending the "hello world" String
USART_send('\r');
USART_send('\n');

DDRB |= _BV(5) ;				//Pin 5 of Port B pushed to OUTPUT
DDRD |= _BV(6) ;                //pin 3 of port B pushed to output
// enable global interrupts:
sei();
	while(1){        					//Infinite loop

        receiveByte = USART_receive();		//Check received Byte
	        USART_send(receiveByte);			//Echo the received byte
	        if(receiveByte == 'H'){			//Checking if received byte is an 'H'
                PORTB |= _BV(5);		//if so pull led on
            } else if(receiveByte == 'L'){
                PORTB &= ~(_BV(5));		//else pull it off
            }
            time_check++;      
    }
return 0;
}

long map(long x, long in_min, long in_max, long out_min, long out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
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
 ADCSRA |= ( _BV(ADPS2) | _BV(ADPS1) | _BV(ADPS0) );    //16Mhz/128 = 125Khz the ADC reference clock
 ADMUX |= _BV(REFS0);                //Voltage reference from Avcc (5v)
 ADCSRA |= _BV(ADEN);                //Turn on ADC
 //ADCSRA |= (1<<ADSC);                //Do an initial conversion because this one is the slowest and to ensure that everything is up and running
}
 
uint16_t read_adc(uint8_t channel){
 ADMUX &= 0xF0;                    //Clear the older channel that was read
 ADMUX |= channel;                //Defines the new ADC channel to be read
 ADCSRA |= _BV(ADSC);                //Starts a new conversion
 while(ADCSRA & _BV(ADSC));            //Wait until the conversion is done
 return ADCW;                    //Returns the ADC value of the chosen channel
}

ISR(TIMER1_COMPA_vect)
{
ADCSRA |= _BV(ADSC);                //Do an initial conversion because this one is the slowest and to ensure that everything is up and running
USART_putstring("Receiving on channel 0: ");        //Send the converted value to the terminal        
adc_value = read_adc(channel);        //Read one ADC channel
itoa(adc_value, buffer, 10);        //Convert the read value to an ascii string
OCR0A = map(adc_value, 0, 1023, 255, 0);
USART_putstring(buffer);        //Send the converted value to the terminal
USART_putstring("\t pwm value: ");        //Send the converted value to the terminal
itoa(OCR0A, buffer, 10);        //Convert the read value to an ascii string
USART_putstring(buffer);        //Send the converted value to the terminal
USART_putstring("\t timecheck value: ");        //Send the converted value to the terminal
itoa(time_check, buffer, 10);        //Convert the read value to an ascii string
USART_putstring(buffer);        //Send the converted value to the terminal
USART_send('\r');
USART_send('\n');        //Send the converted value to the terminal
ADCSRA |= (0<<ADSC);                //Do an initial conversion because this one is the slowest and to ensure that everything is up and running
time_check = 0;
}
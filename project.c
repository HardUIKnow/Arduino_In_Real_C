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
#define F_CPU 16000000UL // Clock Speed
#include <util/delay.h>

                                         /* Serial definition */
// BAUDRATE and prescaler definition
#define BAUDRATE 9600										//Baudrate to 9600
#define BAUD_PRESCALLER (((F_CPU / (BAUDRATE * 16UL))) - 1)	//Baudrate configuration
//Declaration of our functions
void USART_init(void);
unsigned char USART_receive(void);
void USART_send( unsigned char data);
void USART_putstring(char* StringPtr);

                                        /* ADC definition */
uint16_t adc_value;            //Variable used to store the value read from the ADC
char buffer[5];                //Output of the itoa function
void adc_init(void);            //Function to initialize/configure the ADC
uint16_t read_adc(uint8_t channel);    //Function to read an arbitrary analogic channel/pin
uint8_t channel=0;                //Variable for the for() loop

                                        /* other */
char String[]="Connected !!";    //String[] is in fact an array but when we put the text between the " " symbols the compiler threats it as a String and automatically puts the null termination character in the end of the text
char receiveByte;

                                        /* main function */
int main(void){
    // set up the ADC, remove bits set by Arduino library
    //ADCSRA &= ~PS_128; 
  
    // you can choose a prescaler from above.
    // PS_16, PS_32, PS_64 or PS_128
    // ADCSRA |= PS_64;    // set our own prescaler to 64

    // Set up ADMUX - ADC multiplexer selection register
    // Set ADC channel 0 - ADMUX bits 4..0 cleared - default after reset
    // Clear ADLAR bit in AMUX to have right adjusted result -default after reset
    // Voltage Reference Selection - AVCC with external capacitor at AREF pin - ADMUX REFS0 bit 6 set REFS1 bit 7 clear

    // Set up ADCSRB - ADC Control and Status Register B
    // Set ADC Auto Trigger Source to Free-Running Mode - ADTS2 ADTS1 ADTS0 bits 2..0 clear - default after reset
    // Set ADC channel 0 - ADMUX bit 5 cleared - default after reset

    // Set up ADCSRA - ADC control and status register A
    // Set ADC prescaler to 128 - 125KHz sample rate @ 16MHz - ADCSRA ADPS2 ADPS1 ADPS0 bits 2..0 set
    // Set ADC ADC Auto Trigger Enable - ADCSRA ADATE bit 5 set
    // Enable ADC - ADCSRA ADEN bit 7 set
    // Start taking continuous samples by writing logical one to ADCSRA ADSC bit 6

    // Wait for the first conversion to complete.
    // When the conversion completes the ADC Data Registers are updated
    // and the ADCSRA ADIF bit is set and will remain set until we clear it
    // or a reset occurs.
        
    // Since we are in Free-Running Mode new conversions will start immediately after the completion
    // of the current one and will repeat as long as we leave ADSC high.

adc_init();        //Setup the ADC
USART_init();        				//Call the USART initialization code
USART_putstring(String);			//Sending the "hello world" String
USART_send('\r');
USART_send('\n');
DDRB |= (1<<PINB5);				//Pin 5 of Port B pushed to OUTPUT
	while(1){        					//Infinite loop
        USART_putstring("Receiving on channel 0: ");        //Send the converted value to the terminal        
        adc_value = read_adc(channel);        //Read one ADC channel
        itoa(adc_value, buffer, 10);        //Convert the read value to an ascii string
        USART_putstring(buffer);        //Send the converted value to the terminal
        USART_send('\r');
        USART_send('\n');        //Send the converted value to the terminal

	    
            receiveByte = USART_receive();		//Check received Byte
	        USART_send(receiveByte);			//Echo the received byte
	        if(receiveByte == 'H'){			//Checking if received byte is an 'H'
                PORTB |= (1<<PINB5);		//if so pull led on
            } else {
                PORTB &= ~(1<<PINB5);		//else pull it off
            }
              
        _delay_ms(50);
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
 ADCSRA |= (1<<ADSC);                //Do an initial conversion because this one is the slowest and to ensure that everything is up and running
}
 
uint16_t read_adc(uint8_t channel){
 ADMUX &= 0xF0;                    //Clear the older channel that was read
 ADMUX |= channel;                //Defines the new ADC channel to be read
 ADCSRA |= (1<<ADSC);                //Starts a new conversion
 while(ADCSRA & (1<<ADSC));            //Wait until the conversion is done
 return ADCW;                    //Returns the ADC value of the chosen channel
}
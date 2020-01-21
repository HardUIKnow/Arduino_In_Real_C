/*
Building an application for using i2c component
This is a starting point build for example
*/
// define microcontroller used



#include "i2csoft.h"
#define Srf_SLA_W	0xE0
#define Srf_SLA_R	0xE1
#define FALSE	0
#define TRUE	1

//Declaration of our functions
void USART_init(void);
unsigned char USART_receive(void);
void USART_send( unsigned char data);
void USART_putstring(char* StringPtr);
void Srf_Init(void);
int srf_Read(uint8_t address,uint8_t *data);
int Srf_Write(uint8_t address,uint8_t data);

char String[]="Hello world!!";    //String[] is in fact an array but when we put the text between the " " symbols the compiler threats it as a String and automatically puts the null termination character in the end of the text
volatile char receiveByte;

int main(int argc, char const *argv[])
{
    USART_init();
	Srf_Init();
	if(Srf_Write(0xE0, 0) != 0){
		USART_putstring("Sensor connected....\n");
	}else{
		USART_putstring("Sensor not found....\n");
	} 
	if(Srf_Write(0xE0, 0x51) != 0){
		USART_putstring("Ranged set.....\n");
	}else{
		USART_putstring("Sensor not found....\n");
	}
	SoftI2CStop();


    while(1){
		if(Srf_Write(0xE0, 0x2) != 0){
		USART_putstring("Ranged set.....\n");
	}else{
		USART_putstring("Sensor not found....\n");
	}
	SoftI2CStop();
		int receive = srf_Read(0xE0, 0x2);
		USART_putstring("range = " );
		USART_putstring(receive);
		USART_putstring("\r\n");
		
        _delay_ms(500);
    }
    return 0;
}

void Srf_Init(void)
{
   SoftI2CInit();

}


int srf_Read(uint8_t address,uint8_t *data)
{
   uint8_t res;   //result

   //Start

   SoftI2CStart();

   //SLA+W (for dummy write to set register pointer)
   res=SoftI2CWriteByte(Srf_SLA_W); //DS1307 address + W

   //Error
   if(!res) return FALSE;

   //Now send the address of required register

   res=SoftI2CWriteByte(address);

   //Error
   if(!res) return FALSE;

   //Repeat Start
   SoftI2CStart();

   //SLA + R
   res=SoftI2CWriteByte(Srf_SLA_R); //DS1307 Address + R

   //Error
   if(!res) return FALSE;

   //Now read the value with NACK
   *data=SoftI2CReadByte(0);

   //Error

   if(!res) return FALSE;

   //STOP
   SoftI2CStop();

   return *data;
}

int Srf_Write(uint8_t address,uint8_t data)
{
   uint8_t res;   //result

   //Start
   SoftI2CStart();

   //SLA+W
   res=SoftI2CWriteByte(Srf_SLA_W); //DS1307 address + W

   //Error
   if(!res) 
   USART_putstring("init failed !!!\n");

   //Now send the address of required register
   res=SoftI2CWriteByte(address);

   //Error
   if(!res) 
   USART_putstring("Can't reach address !!!!\n");

   //Now write the value

   res=SoftI2CWriteByte(data);

   //Error
   if(!res) 
   USART_putstring("writing data failed !!!!\n");

   //STOP
   SoftI2CStop();

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
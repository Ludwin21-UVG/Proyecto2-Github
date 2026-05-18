/* UART_Proyecto2.c
 * Created: 23/04/2026 19:22:16
 *  Author: Ludwin Gutiérrez */ 

#include "UART_Proyecto2.h"

volatile uint8_t LecturaSerial = 0;

volatile char bufferUART[8];
volatile uint8_t idx = 0;
volatile uint8_t datoListo = 0;
volatile uint8_t servoListo = 0;

void initUART(){
	DDRD   &= ~(1<<DDD0);						 //D0->Rx
	DDRD   |=  (1<<DDD1);						 //D1->Tx
	UCSR0A  = 0;								 //Normal Speed
	UCSR0B  = (1<<RXCIE0)|(1<<RXEN0)|(1<<TXEN0); //Habilitar interrupción RX; Habilitar Rx y Tx
	UCSR0C  = (1<<UCSZ01)|(1<<UCSZ00);			 //Modo Async; Sin paridad; 1 Stop Bit; 8 Data Bits
	UBRR0   = 12;								 //4800 baul
}

//Enviar caracter de ATMega328P a laptop por UART
void writeChar(char c){
	while(!(UCSR0A &(1<<UDRE0))){}				//Esperar buffer vacío
	UDR0 = c;
}
//Enviar una cadena de ATMega328P a laptop por UART
void writeString(char* string){
	for(uint8_t i = 0; *(string + i) != '\0'; i++){
		writeChar(*(string + i));
	}
}

//Enviar numeros
void writeInt(uint8_t val){
	char buf[4];
	buf[0] = '0' + ( val / 100);
	buf[1] = '0' + ((val % 100) / 10);
	buf[2] = '0' + ( val % 10);
	buf[3] = '\0';
	writeString(buf);
}

ISR(USART_RX_vect){
    uint8_t c = UDR0;
    LecturaSerial = c;
    if(c == '\n'){
	    bufferUART[idx] = '\0';
	    datoListo = 1;
	    idx = 0;
	    } 
	else {
	    if(idx < 7){
		    bufferUART[idx++] = c;
	    }
    }
 }
/* UART_Proyecto2.h
 * Created: 23/04/2026 19:21:58
 * Author: Ludwin Gutiérrez  */ 

#ifndef UART_PROYECTO2_H_
#define UART_PROYECTO2_H_

#include <avr/io.h>
#include <avr/interrupt.h>

extern volatile uint8_t LecturaSerial;	//Declaración externa de variables y buffer

extern volatile char bufferUART[8];
extern volatile uint8_t idx;
extern volatile uint8_t datoListo;
extern volatile uint8_t servoListo;

void initUART();						//Prototipos de funciones
void writeChar(char c);
void writeString(char* string);
void writeInt(uint8_t val);


#endif /* UART_PROYECTO2_H_ */
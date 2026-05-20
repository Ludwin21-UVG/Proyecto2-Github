/* Brazo Robotico.c
 * Created: 20/04/2026 07:13:23
 * Author : Ludwin Gutiérrez */ 
/****************************************/
// Encabezado (Libraries)
#define F_CPU 1000000
#include <avr/io.h>
#include <stdint.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "PWM_Config/PWM_Proyecto2.h"		//Librería control PWM con Timers 0, 1 y 2
#include "ADC_config/ADC_Proyecto2.h"		//Librería ADC 1,2,3,4,5,6,7
#include "UART_config/UART_Proyecto2.h"		//Librería UART a 4800bauls
#include "EEPROM_Config/EEPROM_Proyecto2.h"

uint8_t estado = 0;							//0:nada, 1:manual, 2:EEPROM, 3: UARTr

uint8_t reproducirEEPROM  = 0;				//Bandera para reproducir
uint8_t eeprom_cantidad   = 0;				//cuantos cuartetos guardados
uint8_t eeprom_write_idx  = 0;				//donde escribir el próximo (0-3)
uint8_t eeprom_read_idx   = 0;				//cual reproducir el próximo (0-3)

/****************************************/
// Function prototypes
void setup();								//Setup
void initPinchage();						//Inicializar pinchange
uint16_t mapServo1(uint8_t ADC_to_PWM);		//Funciones mapeo de servos
uint16_t mapServo2(uint8_t ADC_to_PWM);
uint16_t mapServo3(uint8_t ADC_to_PWM);
uint16_t mapServo4(uint8_t ADC_to_PWM);

void ProcesarUART();						//Función para procesar datos de python
void ModoManual();							//Modos de funcionamiento
void ModoEEPROM();
void ModoUART();
/****************************************/
// Main Function
int main(void){
	cli();									//Deshabilitar interrupciones globales
	setup();
	sei();									//Habilitar interrupciones globales
    while (1){
		ProcesarUART();						//Ejecutar función que procesa UART
		ModoManual();						//Ejecutar funciones de modos
		ModoEEPROM();
		ModoUART();
	}	
}
/****************************************/
// NON-Interrupt subroutines
void setup(){
	//Configurar prescaler = 16 -> 16MHz/16 = 1MHz
	CLKPR =(1 << CLKPCE);
	CLKPR =(1 << CLKPS2);
	//Inicializar UART
	initUART();										 //4800 Bauls, No parity, 1 stop bit
	//Inicializar ADC
	initADC(ADC_REF_AVCC, ADC_LEFT, ADC_PRESCALER_8);
	MaskADC(0b11110000);							 //ADC4, ADC5, ADC6 y ADC7
	ADCSRA |= (1<<ADSC);							 //iniciar lecturas ADC
	//Inicializar Timer1: PWM PB1 y PB2
	initTimer1(PWM_FAST,PWM_PRESCALER_8, 2499);		 //20ms / 50Hz
	initPWM1A (PWM_NO_INVERTIDO);
	initPWM1B (PWM_NO_INVERTIDO);
	//Inicializar Timer2: PWM PB3 y PD3				//16.67ms / 60Hz
	initTimer2(PWM_FAST, PWM_PRESCALER_64);
	initPWM2A (PWM_NO_INVERTIDO);
	initPWM2B (PWM_NO_INVERTIDO);
	//Configurar Output
	DDRD  |=   (1<<DDD6) | (1<<DDD5) | (1<<DDD4);  //Salidas LED´s
	PORTD &= ~((1<<DDD6) | (1<<DDD5) | (1<<DDD4)); //Apagar
	//Configurar Input
	DDRC  &= ~(1<<DDC0);	//Botón en PC0
	PORTC |=  (1<<PORTC0);	//Activar Pull-up
	//Configurar Interrupción Pinchage
	initPinchage();
	//EEPROM inicial
    eeprom_cantidad  = readEEPROM(0x00);		   //Leer datos guardados en EEPROM
    eeprom_write_idx = readEEPROM(0x01);		   //Cantidad, índice de escritura e índice de lectura
    eeprom_read_idx  = readEEPROM(0x02);
    //Por seguridad, reiniciar variables si contienen valores inválidos
    if(eeprom_cantidad > 4)  eeprom_cantidad  = 0;
    if(eeprom_write_idx > 3) eeprom_write_idx = 0;
    if(eeprom_read_idx > 3)  eeprom_read_idx  = 0;
}

void initPinchage(){//Habilitar PC0
	PCICR  |= (1<<PCIE1);
	PCMSK1 |= (1<<PCINT8);
}

uint16_t mapServo1(uint8_t ADC_to_PWM){
	return  75 + ((uint32_t)ADC_to_PWM * 225 / 255);						 //0-180 grados
}

uint16_t mapServo2(uint8_t ADC_to_PWM){
	return  120 + ((uint32_t)ADC_to_PWM * 180 / 255);						 //95-180 grados
}

uint16_t mapServo3(uint8_t ADC_to_PWM){
	return ((uint32_t)(214 + ((uint32_t)ADC_to_PWM * 41) / 255) * 77) / 255; //150-180 grados
}

uint16_t mapServo4(uint8_t ADC_to_PWM){
	return ((uint32_t)(74 + ((uint32_t)ADC_to_PWM * 53) / 255) * 75) / 255;	 //52-85 grados
}


void ProcesarUART(){
	if(!datoListo) return;						   //Verificar si hay algún dato recibido
	datoListo = 0;								   //Limpiar bandera

	//Botones: B1:000, B2:000, B2:001, B3:000
	if(bufferUART[0] == 'B'){					  //Verificar si el comando es de botones
		uint8_t btn = bufferUART[1] - '0';		  //Obtener número de botón recibido
		uint8_t val = atoi((char*)&bufferUART[3]);//Obtener valor enviado desde UART

		if(btn == 1){							  // Modo Manual
			estado = 1;
			PORTD |=  (1<<PORTD4);				  //Led indicador físico de modo
			PORTD &= ~(1<<PORTD5);				  //Demás leds apagados
			PORTD &= ~(1<<PORTD6);
		}
		else if(btn == 2 && val == 0){			  //Modo EEPROM
			estado = 2;
			PORTD |=  (1<<PORTD5);				  //Led indicador físico de modo
			PORTD &= ~(1<<PORTD4);				  //Demás leds apagados
			PORTD &= ~(1<<PORTD6);
		}
		else if(btn == 2 && val == 1){			  //Si el comando del botón es igual a 2 y su valor es 1
			if(estado == 2){					  //Solo se cumple si anteriormente 
				reproducirEEPROM = 1;
			}
		}
		else if(btn == 3){						  // Modo UART
			estado = 3;
			PORTD |=  (1<<PORTD6);				  //Led indicador físico de modo
			PORTD &= ~(1<<PORTD4);				  //Demás leds apagados
			PORTD &= ~(1<<PORTD5);
		}
	}
	// Servos
	else if(bufferUART[0] == 'S'){				  //Verificar si el comando corresponde a servos
		servoListo = 1;							  //Activar bandera
	}
}

void ModoManual(){
	if(estado == 1){
		updateDutyCiclePWM1A(mapServo1(LecturaADC4));// Servo1 PB1
	    updateDutyCiclePWM1B(mapServo2(LecturaADC5));// Servo2 PB2
		updateDutyCiclePWM2A(mapServo3(LecturaADC6));// Servo3 PB3
		updateDutyCiclePWM2B(mapServo4(LecturaADC7));// Servo4 PD3
		_delay_ms(10);
	}
}

void ModoEEPROM(){
	if(estado == 2 && reproducirEEPROM){			 //Ejecutar solo en modo EEPROM y cuando se solicite reproducción
		reproducirEEPROM = 0;						 //Limpiar bandera
		if(eeprom_cantidad == 0) return;			 //Verificar si existen posiciones guardadas

		uint16_t base = 0x10 + (eeprom_read_idx * 4);//Calcular dirección base en EEPROM
		uint8_t adc1 = readEEPROM(base + 0);		 //Leer valores ADC guardados para cada servo
		uint8_t adc2 = readEEPROM(base + 1);
		uint8_t adc3 = readEEPROM(base + 2);
		uint8_t adc4 = readEEPROM(base + 3);

		updateDutyCiclePWM1A(mapServo1(adc1));		 //Actualizar posiciones PWM de los 4 servos
		updateDutyCiclePWM1B(mapServo2(adc2));
		updateDutyCiclePWM2A(mapServo3(adc3));
		updateDutyCiclePWM2B(mapServo4(adc4));

		writeString("P:");						     //Enviar por UART la posición reproducida
		writeInt(eeprom_read_idx + 1);
		writeString("\n");

		eeprom_read_idx = (eeprom_read_idx + 1) % eeprom_cantidad; //Actualizar índice de lectura en forma circular
		writeEEPROM(0x02, eeprom_read_idx);						   //Guardar nuevo índice de lectura en EEPROM
	}
}

void ModoUART(){
	if(estado == 3 && servoListo){									//Ejecutar solo en modo UART y cuando bandera esté lista
		servoListo = 0;												//Limpiar bandera
		uint8_t servo  = bufferUART[1] - '0';						//Obtener número de servo recibido
		uint8_t angulo = atoi((char*)&bufferUART[3]);				//Obtener ángulo recibido
		uint8_t adc;

		if(servo == 1){
			adc = (uint8_t)((uint32_t)angulo * 255 / 180);			//Convertir ángulo a valor ADC equivalente
			updateDutyCiclePWM1A(mapServo1(adc));
		}
		else if(servo == 2){
			adc = (uint8_t)((uint32_t)(angulo - 95) * 255 / 85);
			updateDutyCiclePWM1B(mapServo2(adc));
		}
		else if(servo == 3){
			adc = (uint8_t)((uint32_t)(angulo - 150) * 255 / 30);
			updateDutyCiclePWM2A(mapServo3(adc));
		}
		else if(servo == 4){
			adc = (uint8_t)((uint32_t)(angulo - 52) * 255 / 33);
			updateDutyCiclePWM2B(mapServo4(adc));
		}
	}
}

/****************************************/
// Interrupt routines
ISR(PCINT1_vect){										//Interrupción al presionar el botón físico
	if(!(PINC & (1<<PINC0)) && estado != 2){			//Verificar si el botón fue presionado y que no esté en el modo EEPROM
		uint16_t base = 0x10 + (eeprom_write_idx * 4);	//Calcular dirección base para almacenar datos
		writeEEPROM(base + 0, LecturaADC4);				//Guardar lecturas ADC de los 4 servos en EEPROM
		writeEEPROM(base + 1, LecturaADC5);
		writeEEPROM(base + 2, LecturaADC6);
		writeEEPROM(base + 3, LecturaADC7);

		eeprom_write_idx = (eeprom_write_idx + 1) % 4;  //Actualizar índice de escritura usando FIFO circular
		if(eeprom_cantidad < 4) eeprom_cantidad++;		//Incrementar cantidad de posiciones guardadas

		writeEEPROM(0x00, eeprom_cantidad);				//Guardar cantidad e índice en EEPROM
		writeEEPROM(0x01, eeprom_write_idx);

		writeString("C:");
		writeInt(eeprom_cantidad);						//Reportar por UART la cantidad almacenada
		writeString("\n");
	}
}
/* EEPROM_Config.c
 * Created: 24/04/2026 10:06:22
 * Author: Ludwin Gutiérrez  */ 

#include "EEPROM_Proyecto2.h"

void writeEEPROM(uint16_t direccion, uint8_t dato){
	// Esperar a que termine de escribir dato anterior
	while(EECR & (1 << EEPE));
	// Asignar dirección donde queremos escribir
	EEAR = direccion;
	// Asignar dato a guarda
	EEDR = dato;
	// Master write
	EECR |=(1 << EEMPE);
	// Write enable
	EECR |=(1 << EEPE);
}

uint8_t readEEPROM(uint16_t direccion){
	// Esperar a que termine de escribir dato anterior
	while(EECR & (1 << EEPE));
	// Asignar dirección de donde queremos LEER el dato
	EEAR = direccion;
	// Read enable
	EECR |= (1 << EERE);
	//Retonar lectura de EEPROM en dirección EEAR
	return EEDR;	
}
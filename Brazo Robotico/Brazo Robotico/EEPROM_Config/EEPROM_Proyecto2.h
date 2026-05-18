/* EEPROM_Proyecto2.h
 * Created: 24/04/2026 10:05:57
 * Author: Ludwin Gutiérrez  */ 

#ifndef EEPROM_PROYECTO2_H_
#define EEPROM_PROYECTO2_H_

#include <avr/io.h>
#include <avr/interrupt.h>

void writeEEPROM(uint16_t direccion, uint8_t dato);
uint8_t readEEPROM(uint16_t direccion);


#endif /* EEPROM_PROYECTO2_H_ */
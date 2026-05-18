/* PWM_Proyecto2.h
 * Created: 20/04/2026 07:19:05
 * Author: Ludwin Gutiérrez */ 

#ifndef PWM_PROYECTO2_H_
#define PWM_PROYECTO2_H_

#include <avr/io.h>
#include <avr/interrupt.h>

// Modo
#define PWM_FAST 0
#define PWM_PHASE_CORRECT 1

// Salida
#define PWM_NO_INVERTIDO 0
#define PWM_INVERTIDO 1

// Prescaler
#define PWM_PRESCALER_1     1
#define PWM_PRESCALER_8     8
#define PWM_PRESCALER_64    64
#define PWM_PRESCALER_256   256
#define PWM_PRESCALER_1024  1024


// Timer0
void initTimer0(uint8_t modo, uint16_t prescaler);
// Canales
void initPWM0A(uint8_t salida);
void initPWM0B(uint8_t salida);
// Duty
void updateDutyCiclePWM0A(uint8_t ciclo);
void updateDutyCiclePWM0B(uint8_t ciclo);


// Timer1
void initTimer1(uint8_t modo, uint16_t prescaler, uint16_t ValorTop);
// Canales
void initPWM1A(uint8_t salida);
void initPWM1B(uint8_t salida);
// Duty
void updateDutyCiclePWM1A(uint16_t ciclo);
void updateDutyCiclePWM1B(uint16_t ciclo);


// Timer2
void initTimer2(uint8_t modo, uint16_t prescaler);
// Canales
void initPWM2A(uint8_t salida);
void initPWM2B(uint8_t salida);
// Duty
void updateDutyCiclePWM2A(uint8_t ciclo);
void updateDutyCiclePWM2B(uint8_t ciclo);
#endif /* PWM_PROYECTO2_H_ */
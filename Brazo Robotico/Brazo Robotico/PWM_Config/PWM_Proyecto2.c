/* PWM_Proyecto2.c
 * Created: 20/04/2026 07:18:23
 *  Author: Ludwin Gutiérrez */ 
#include "PWM_Proyecto2.h"

void initTimer0(uint8_t modo, uint16_t prescaler){
	TCCR0A = 0;
	TCCR0B = 0;
	// Modo PWM
	if(modo == PWM_FAST){
		TCCR0A |= (1<<WGM01) | (1<<WGM00);} //Fast PWM
	else if(modo == PWM_PHASE_CORRECT){
		TCCR0A |= (1<<WGM00);		      } //Phase Correct
	// Prescaler
	switch(prescaler){
		case PWM_PRESCALER_1:    TCCR0B |= (1<<CS00);			break;
		case PWM_PRESCALER_8:    TCCR0B |= (1<<CS01);			break;
		case PWM_PRESCALER_64:   TCCR0B |= (1<<CS01)|(1<<CS00); break;
		case PWM_PRESCALER_256:  TCCR0B |= (1<<CS02);			break;
		case PWM_PRESCALER_1024: TCCR0B |= (1<<CS02)|(1<<CS00); break;
	}
}

void initPWM0A(uint8_t salida){
	DDRD |= (1<<DDD6);
	// Salida
	TCCR0A &= ~((1<<COM0A1)|(1<<COM0A0));
	if  (salida == PWM_NO_INVERTIDO){
		TCCR0A |= (1<<COM0A1);}
	else if(salida == PWM_INVERTIDO){
		TCCR0A |= (1<<COM0A1) | (1<<COM0A0);}
}

void updateDutyCiclePWM0A(uint8_t ciclo){
	OCR0A = ciclo;
}

void initPWM0B(uint8_t salida){
	DDRD |= (1<<DDD5);
	//Salida
	TCCR0A &= ~((1<<COM0B1)|(1<<COM0B0));
	if  (salida == PWM_NO_INVERTIDO){
		TCCR0A |= (1<<COM0B1);}
	else if(salida == PWM_INVERTIDO){
		TCCR0A |= (1<<COM0B1) | (1<<COM0B0);}
}

void updateDutyCiclePWM0B(uint8_t ciclo){
	OCR0B = ciclo;
}


void initTimer1(uint8_t modo, uint16_t prescaler, uint16_t ValorTop){
	TCCR1A = 0;
	TCCR1B = 0;
	// Modo PWM
	if(modo == PWM_FAST){
		TCCR1A |= (1<<WGM11);
		TCCR1B |= (1<<WGM13) | (1<<WGM12);}
	else if(modo == PWM_PHASE_CORRECT){
		TCCR1A |= (1<<WGM11);
		TCCR1B |= (1<<WGM13);}
	// Prescaler
	switch(prescaler){
		case PWM_PRESCALER_1:    TCCR1B |= (1<<CS10);                         break;
		case PWM_PRESCALER_8:    TCCR1B |= (1<<CS11);                         break;
		case PWM_PRESCALER_64:   TCCR1B |= (1<<CS11)|(1<<CS10);               break;
		case PWM_PRESCALER_256:  TCCR1B |= (1<<CS12);                         break;
		case PWM_PRESCALER_1024: TCCR1B |= (1<<CS12)|(1<<CS10);               break;
		}
	ICR1 = ValorTop; //2499
}


void initPWM1A(uint8_t salida){
	DDRB |= (1<<DDB1);
	// Salida
	TCCR1A &= ~((1<<COM1A1)|(1<<COM1A0));
	if(salida == PWM_NO_INVERTIDO){
		TCCR1A |= (1<<COM1A1);}
	else if(salida == PWM_INVERTIDO){
		TCCR1A |= (1<<COM1A1) | (1<<COM1A0);}
}

void updateDutyCiclePWM1A(uint16_t ciclo){
	OCR1A = ciclo;
}

void initPWM1B(uint8_t salida){
	DDRB |= (1<<DDB2);
	// Salida
	TCCR1A &= ~((1<<COM1B1)|(1<<COM1B0));
	if(salida == PWM_NO_INVERTIDO){
		TCCR1A |= (1<<COM1B1);}
	else if(salida == PWM_INVERTIDO){
		TCCR1A |= (1<<COM1B1) | (1<<COM1B0);}
}

void updateDutyCiclePWM1B(uint16_t ciclo){
	OCR1B = ciclo;
}

void initTimer2(uint8_t modo, uint16_t prescaler){
	TCCR2A = 0;
	TCCR2B = 0;
	// Modo PWM
	if(modo == PWM_FAST){
		TCCR2A |= (1<<WGM21) | (1<<WGM20);} //Fast PWM
	else if(modo == PWM_PHASE_CORRECT){
		TCCR2A |= (1<<WGM20);		      } //Phase Correct
	// Prescaler
	switch(prescaler){
		case PWM_PRESCALER_1:    TCCR2B |= (1<<CS20);			break;
		case PWM_PRESCALER_8:    TCCR2B |= (1<<CS21);			break;
		case PWM_PRESCALER_64:   TCCR2B |= (1<<CS21)|(1<<CS20); break;
		case PWM_PRESCALER_256:  TCCR2B |= (1<<CS22);			break;
		case PWM_PRESCALER_1024: TCCR2B |= (1<<CS22)|(1<<CS20); break;
	}
}

void initPWM2A(uint8_t salida){
	DDRB |= (1<<DDB3);
	// Salida
	TCCR2A &= ~((1<<COM2A1)|(1<<COM2A0));
	if  (salida == PWM_NO_INVERTIDO){
		TCCR2A |= (1<<COM2A1);}
	else if(salida == PWM_INVERTIDO){
		TCCR2A |= (1<<COM2A1) | (1<<COM2A0);}
}

void updateDutyCiclePWM2A(uint8_t ciclo){
	OCR2A = ciclo;
}

void initPWM2B(uint8_t salida){
	DDRD |= (1<<DDD3);
	//Salida
	TCCR2A &= ~((1<<COM2B1)|(1<<COM2B0));
	if  (salida == PWM_NO_INVERTIDO){
		TCCR2A |= (1<<COM2B1);}
	else if(salida == PWM_INVERTIDO){
		TCCR2A |= (1<<COM2B1) | (1<<COM2B0);}
}

void updateDutyCiclePWM2B(uint8_t ciclo){
	OCR2B = ciclo;
}
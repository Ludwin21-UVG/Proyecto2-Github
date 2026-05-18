/* ADC_Proyecto2.c
 * Created: 20/04/2026 10:45:30
 * Author: Ludwin Gutiérrez  */ 
#include "ADC_Proyecto2.h"

volatile uint8_t Config_Mask_ADC = 0;
volatile uint8_t CanalADC    = 0;
volatile uint8_t LecturaADC0 = 0;
volatile uint8_t LecturaADC1 = 0;
volatile uint8_t LecturaADC2 = 0;
volatile uint8_t LecturaADC3 = 0;
volatile uint8_t LecturaADC4 = 0;
volatile uint8_t LecturaADC5 = 0;
volatile uint8_t LecturaADC6 = 0;
volatile uint8_t LecturaADC7 = 0;

void initADC(uint8_t Referencia, uint8_t Justificacion, uint8_t Prescaler){
	ADMUX  = 0;
	ADCSRA = 0;
	// Referencia
	switch(Referencia){
		case ADC_REF_AREF:									break;
		case ADC_REF_AVCC:  ADMUX |=			(1<<REFS0);	break;
		case ADC_REF_1V1:   ADMUX |= (1<<REFS1)|(1<<REFS0); break;
	}
	// Justificación
	if(Justificacion == ADC_LEFT){
		ADMUX |= (1<<ADLAR);
	}
	// Prescaler
	switch(Prescaler){
		case ADC_PRESCALER_2:   ADCSRA |=						(1<<ADPS0);	break;
		case ADC_PRESCALER_4:   ADCSRA |=			 (1<<ADPS1);			break;
		case ADC_PRESCALER_8:   ADCSRA |=			 (1<<ADPS1)|(1<<ADPS0);	break;
		case ADC_PRESCALER_16:  ADCSRA |= (1<<ADPS2);						break;
		case ADC_PRESCALER_32:  ADCSRA |= (1<<ADPS2)|			(1<<ADPS0);	break;
		case ADC_PRESCALER_64:  ADCSRA |= (1<<ADPS2)|(1<<ADPS1);			break;
		case ADC_PRESCALER_128: ADCSRA |= (1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0); break;
	}
	// Habilitar ADC
	ADCSRA |= (1<<ADEN);
	// Interrupción
	ADCSRA |= (1<<ADIE);
}

void MaskADC(uint8_t MaskADC_config){
	Config_Mask_ADC = MaskADC_config;
}

ISR(ADC_vect){
	if     (CanalADC == ADC_CH0) {LecturaADC0 = ADCH;}
	else if(CanalADC == ADC_CH1) {LecturaADC1 = ADCH;}
	else if(CanalADC == ADC_CH2) {LecturaADC2 = ADCH;}
	else if(CanalADC == ADC_CH3) {LecturaADC3 = ADCH;}
	else if(CanalADC == ADC_CH4) {LecturaADC4 = ADCH;}
	else if(CanalADC == ADC_CH5) {LecturaADC5 = ADCH;}
	else if(CanalADC == ADC_CH6) {LecturaADC6 = ADCH;}
	else if(CanalADC == ADC_CH7) {LecturaADC7 = ADCH;}

	// Buscar siguiente canal válido según máscara
	do{
		CanalADC++;
		if(CanalADC > 7){
		   CanalADC = 0;}
	}
	while(!(Config_Mask_ADC & (1<<CanalADC)));

	// Cambiar canal
	ADMUX = (ADMUX & 0xF0) | CanalADC;

	// Siguiente conversión
	ADCSRA |= (1<<ADSC);
}
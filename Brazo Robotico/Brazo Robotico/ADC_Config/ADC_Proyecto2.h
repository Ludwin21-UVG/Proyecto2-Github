/* ADC_Proyecto2.h
 * Created: 20/04/2026 10:46:02
 * Author: Ludwin Gutiérrez  */ 

#ifndef ADC_PROYECTO2_H_
#define ADC_PROYECTO2_H_

#include <avr/io.h>
#include <avr/interrupt.h>

extern volatile uint8_t LecturaADC0;
extern volatile uint8_t LecturaADC1;
extern volatile uint8_t LecturaADC2;
extern volatile uint8_t LecturaADC3;
extern volatile uint8_t LecturaADC4;
extern volatile uint8_t LecturaADC5;
extern volatile uint8_t LecturaADC6;
extern volatile uint8_t LecturaADC7;

//Voltaje de referencia
#define ADC_REF_AREF     0
#define ADC_REF_AVCC     1
#define ADC_REF_1V1      2

//Justificación
#define ADC_RIGHT        0
#define ADC_LEFT         1

//Prescaler
#define ADC_PRESCALER_2    2
#define ADC_PRESCALER_4    4
#define ADC_PRESCALER_8    8
#define ADC_PRESCALER_16   16
#define ADC_PRESCALER_32   32
#define ADC_PRESCALER_64   64
#define ADC_PRESCALER_128  128

//Canales
#define ADC_CH0 0
#define ADC_CH1 1
#define ADC_CH2 2
#define ADC_CH3 3
#define ADC_CH4 4
#define ADC_CH5 5
#define ADC_CH6 6
#define ADC_CH7 7

void initADC(uint8_t Referencia, uint8_t Justificacion, uint8_t Prescaler);

void MaskADC(uint8_t MaskADC_config);
#endif /* ADC_PROYECTO2_H_ */
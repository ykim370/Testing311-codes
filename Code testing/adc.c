/*
 * adc.c
 *
 * Created: 4/10/2023 3:34:38 pm
 *  Author: vsan396
 */ 


#include "adc.h"
#include <avr/io.h>

//This function configures the ADC to use AVCC as reference and read ADC0 in auto trigger mode
//15.5 ADC clock cycles for conversion therefore, it takes us for a conversion.
void adc_init(void){
	ADMUX |= 1 << REFS0;		//AVCC set as reference, ADC0 selected and results are right adjusted
	ADCSRA |= (1 << ADEN) | (1 << ADSC) | (1 << ADATE) | (0b110 << ADPS0);		//Enable ADC, start conversion, setup auto-trigger, enable interrupts and set prescaler to 64
	ADCSRB |= (1 << ADTS2); //Use TC1 overflow as auto- trigger source
	DIDR0 |= (1 << ADC0D);	//ADC0 buffer disabled
}

uint16_t adc_to_mv(uint32_t adc_value){
	adc_value = (adc_value << 12) + (adc_value<<9) + (adc_value<<8) + (adc_value<<7) + (adc_value<<3) >> 10; // adc_value * 5000/1024
	return adc_value;
}
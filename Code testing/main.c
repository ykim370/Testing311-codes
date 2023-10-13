/*
 * Tawans_code.c
 *
 * Created: 4/10/2023 3:29:04 pm
 * Author : vsan396
 */ 

//INCLUDES
#include <avr/io.h>
#include <stdint.h>
#include <avr/interrupt.h>
#include "adc.h"
#include "timer0.h"
#include "PI_control.h"

//DEFINES
#define F_CPU 16000000UL

//GLOBALS

volatile static uint16_t ADC_VALUE;	
static volatile uint16_t V_ref = 3000; // 0-1024 is equiv
volatile uint8_t ADC_flag = 0;
void PIN_TOGGLE_D(uint8_t PIN){
	PORTD ^= (1 << PIN);
}	

ISR(ADC_vect){
	PIN_TOGGLE_D(PIND0); // toggle PB5 to indicate ISR trigger
	ADC_VALUE = ADC; // Reading ADC register clears the ADC interrupt flag 
	ADC_flag = 1; 
	TIFR0 |= (1 << TOV0); //clears tc0 overflow flag
}

int main(void)
{

	tc0_init_FPWM_100k(); //initialize PWM to generate 100KHz, 50% duty
	adc_init(); //initializing ADC
	DDRD |= (1 << PIND0); //set PIND0 as output
	DDRD |= (1 << PIND2);
	ENABLE_ADC_INTERRUPT;
	sei();
	
    while (1) {
		if(ADC_flag == 1){
			PIN_TOGGLE_D(PIND2);
			pi_controller(adc_to_mv(ADC_VALUE), V_ref);
			pi_to_duty();
			PIN_TOGGLE_D(PIND2);
			ADC_flag = 0;	
		}
	}
}


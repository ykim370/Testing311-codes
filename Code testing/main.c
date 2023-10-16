#include <avr/io.h>
#include <stdint.h>
#include <avr/interrupt.h>
#include <stdbool.h>
#include <stdio.h>
#include "adc.h"
#include "timer0.h"
#include "PI_control.h"
#include "UART.h"

#define F_CPU 16000000UL
#define BUFFER_SIZE 7

static volatile uint16_t ADC_VALUE;
static volatile uint16_t V_ref;
volatile bool ADC_flag = false;
volatile static int rx_byte_pos = 0;
volatile static char rx_buffer[BUFFER_SIZE] = {0, 0, 0, 0, 0, 0, 0};
volatile static uint8_t rx_complete_flag = 0;

void PIN_TOGGLE_D(uint8_t PIN){
	PORTD ^= (1 << PIN);
}
void PIN_TOGGLE_B(uint8_t PIN){
	PORTB ^= (1 << PIN);
}
ISR(USART_RX_vect){
	rx_buffer[rx_byte_pos] = UDR0;
	rx_byte_pos++;
	if(rx_buffer[rx_byte_pos-1] == '\r'){
		rx_buffer[rx_byte_pos-1] = '\0';
		rx_complete_flag = 1;
		rx_byte_pos = 0;
		// Update V_ref from rx_buffer
		sscanf(rx_buffer, "%u", &V_ref);
	}
// 		else if(rx_byte_pos > (BUFFER_SIZE -1)){
// 			rx_byte_pos = 0;
// 		}
}


ISR(ADC_vect){
	PIN_TOGGLE_D(PIND0); // toggle PB5 to indicate ISR trigger
	ADC_VALUE = ADC; // Reading ADC register clears the ADC interrupt flag, TO DEBUG SET VALUE TO 0-1024
	ADC_flag = true;
	TIFR0 |= (1 << TOV0); //clears tc0 overflow flag
}

int main(void)
{
	//initialization
	UART_init();
	tc0_init_FPWM_100k(); //initialize PWM to generate 100KHz, 50% duty
	adc_init(); //initializing ADC
	//Setting pins as output
	DDRD |= (1 << PIND4); // PIND4 is ADC ISR debug pin 
	DDRD |= (1 << PIND2); // PIND2 is PI controller debug pin
	DDRB |= (1 << PINB4); //PINB4 is UART RX debug pin
	//sprintf(tx_buffer, "%d\r\n", 16000);
	ENABLE_ADC_INTERRUPT;
	sei();
	
    while (1) {
		if(ADC_flag == true){
			PIN_TOGGLE_D(PIND2);
			pi_controller(adc_to_mv(ADC_VALUE), V_ref); // when testing set the ADC_VALUE to avoid errors
			pi_to_duty();
			PIN_TOGGLE_D(PIND2);
			ADC_flag = false;	
		}
	}
}

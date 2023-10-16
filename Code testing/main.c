* Author : Yechan
//INCLUDES
#include <avr/io.h>
#include <stdint.h>
#include <avr/interrupt.h>
#include <stdbool.h>
#include <stdio.h>
#include "adc.h"
#include "timer0.h"
#include "PI_control.h"
#include "UART.h"

//DEFINES
#define F_CPU 16000000UL
#define BUFFER_SIZE 7

//GLOBALS
static volatile uint16_t ADC_VALUE; 
static volatile uint16_t V_ref;
volatile bool ADC_flag = false;
volatile static int rx_byte_pos = 0;
volatile static char rx_buffer[BUFFER_SIZE] = {0, 0, 0, 0, 0, 0, 0};
volatile static uint8_t rx_complete_flag = 0;
volatile static int tx_byte_pos = 0;
//static char tx_buffer[BUFFER_SIZE] = {0, 0, 0, 0, 0, 0, 0};
volatile static uint8_t tx_complete_flag = 0;


//Setting pin direction
void PIN_TOGGLE_D(uint8_t PIN){
	PORTD ^= (1 << PIN);
}	

void PIN_TOGGLE_B(uint8_t PIN){
	PORTB ^= (1 << PIN);
}

//UART interrupt
ISR(USART_RX_vect){
	rx_buffer[rx_byte_pos] = UDR0;        //Store received character to next position of array
	rx_byte_pos++;                        //Increment buffer position
	if(rx_buffer[rx_byte_pos-1] == '\r'){ //Check if a carriage return entered indicating end of message
		rx_buffer[rx_byte_pos-1] = '\0';    //Replace \r with a null to indicate end of message
		rx_complete_flag = 1;               //Indicate message ready to read
		rx_byte_pos = 0;                    //Reset position in array
	}
	else if(rx_byte_pos > (BUFFER_SIZE -1)){
		rx_byte_pos = 0;                    //If buffer full discard message and reset buffer
	}
	PIN_TOGGLE_B(PINB4);                    //Toggle pin to indicate ISR execution
}

/* ISR(USART_TX_vect){
	UDR0 = tx_buffer[tx_byte_pos];
	tx_byte_pos++;
	tx_complete_flag = 0;
	if (tx_buffer[tx_byte_pos] == '\n'){ //Check if end of message
		tx_buffer[tx_byte_pos] = '\0'; //Replace \n with a null to indicate the end of message
		tx_complete_flag = 1;
		tx_byte_pos = 0;
	}
	UCSR0A |= 1 << RXC0; //Initiate receiving
	//PIN_TOGGLE_B(PINB4);                    //Toggle pin to indicate ISR execution
} */

/*ISR(USART_UDRE_vect){
	UCSR0A |= 1 << TXC0; //Initiate transmission
} */

//ADC interrupt
ISR(ADC_vect){
	PIN_TOGGLE_D(PIND4); // toggle PB5 to indicate ISR trigger
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

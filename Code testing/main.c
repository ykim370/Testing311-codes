* Author : Yechan

#include "UART.h"
#include <avr/io.h>
#include <stdint.h>
#include <string.h>

#define F_CPU 16000000
#define BAUD_RATE 9600
#define TX_BUFFER_SIZE 9
#define RX_BUFFER_SIZE 9

void UART_init(void){
	UCSR0B |= 1 << TXEN0 | 1 << RXEN0 | 1 << TXCIE0 | 1 << RXCIE0;
	UCSR0C |= 3 << UCSZ00;
	UBRR0 = F_CPU / ((uint32_t)16 * BAUD_RATE) - 1;
}

void uart_transmit_byte(char byte){
	while (!(UCSR0A & (1 << UDRE0)));
	UDR0 = byte;
}

void uart_transmit_array(char* msg){
	for (uint8_t i = 0; i < strlen(msg); i++){
		uart_transmit_byte(msg[i]);
	}
}

int uart_extract_digits(int value){
	return uart_convert_digits(value % 10);
}

int uart_convert_digits(int digit){
	return digit + 48;
}

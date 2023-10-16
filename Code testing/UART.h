#ifndef UART_H
#define UART_H
#include <stdint.h>

void UART_init_inturpt(uint32_t BAUD_RATE);
void uart_transmit_byte(char byte);
void uart_transmit_array(char* msg);
int uart_extract_digits(int value);
int uart_convert_digits(int digit);



#endif /* UART_H_ */

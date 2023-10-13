/*
 * timer1.c
 *
 * Created: 4/10/2023 3:30:41 pm
 *  Author: vsan396
 */ 

#include "timer0.h"
#include <avr/io.h>

//changed to timer0
void tc0_init_FPWM_100k()
{
	TCCR0A = 1<<COM0B1 | 3<<WGM00; //WGM0[2..0] set to 111 for Fast PWM mode & COM1A1 set to 1 for output fast PWM
	TCCR0B = 1<<WGM02 | 1<<CS00; //WGM[2..0] to 1111 and CS0[2..0] set to 001 for a clock divider of 1
	//TIMSK1 = 1 << OCIE1A; //Enable compare A match interrupt
	OCR0A = 159; //Loading OCR0A with 159 to setup a 100kHz PWM
	OCR0B = 79; // initialize to 50% duty cycle
	DDRD |= 1<<PIND5;
}

//void tc1_init_FPWM_100k()
//{
	//TCCR1A = 1<<COM1A1 | 2<<WGM10; //WGM0[2..0] set to 111 for Fast PWM mode & COM1A1 set to 1 for output fast PWM
	//TCCR1B = 3<<WGM12 | 1<<CS10; //WGM[2..0] to 1111 and CS0[2..0] set to 001 for a clock divider of 1
	//TIMSK1 = 1 << OCIE1A; //Enable compare A match interrupt
	//ICR1 = 159; //Loading OCR0A with 159 to setup a 100kHz PWM
	//OCR1A = 140; // initialize to 50% duty cycle
	//DDRB |= 1<<PINB1;
//}


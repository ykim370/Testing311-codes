#include "PI_control.h"
#include <stdint.h>
#include <avr/io.h>
#define KP 5 // proportional controller coff
#define KI 100 // integral controller coff
#define gvs 7 // inverted gain of resistor divider Rva,Rvb
#define T_SAMPLE 1/60000 // Set Sampling rate
#define PI_LIMIT 100000// Set PI limit
static volatile float PI_out;
static volatile int32_t Int_out;
static volatile uint32_t new_duty;

int32_t sat_limit_pi(int32_t result) {
	if (result > PI_LIMIT){
		return PI_LIMIT; //If result greater than upper limit return upper limit
	}
	else if (result < -PI_LIMIT){
		return -PI_LIMIT; //If result smaller than lower limit return upper limit
	}
	else
	return result;
}

void pi_controller(uint16_t Vout, uint16_t Vref) {
	int32_t Verror = -((int32_t)Vref - Vout);
	int32_t Prop_out = Verror * KP; //calculate the output of proportional term
	Int_out = sat_limit_pi(Int_out + (Verror * KI * T_SAMPLE)); //calculate the output of integral term
	PI_out = sat_limit_pi(Prop_out + Int_out);
}

//changed to timer0
void pi_to_duty (void) {
	new_duty = (uint32_t)PI_out * 159 / (PI_LIMIT << 1) + 79.5;
	OCR0B = new_duty; //IF YOU CHANGE PI_LIMIT YOU MUST CHANGE THE NUMBERS IN THIS FUNCTION
}	

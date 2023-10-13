/*
 * PI_control.h
 *
 * Created: 5/10/2023 1:11:42 pm
 *  Author: Vsan396
 */ 


#ifndef PI_CONTROL_H_
#define PI_CONTROL_H_
#include <stdint.h>

int32_t sat_limit_pi(int32_t result);
void pi_controller(uint16_t Vout, uint16_t Vref);
void pi_to_duty (void);


#endif /* PI_CONTROL_H_ */
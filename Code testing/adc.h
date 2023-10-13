/*
 * adc.h
 *
 * Created: 4/10/2023 3:34:52 pm
 *  Author: vsan396
 */ 


#ifndef ADC_H_
#define ADC_H_
#include <stdint.h>

#define ENABLE_ADC_INTERRUPT ADCSRA |= (1 << ADIE)
#define DISABLE_ADC_INTERRUPT ADCSRA &= ~(1 << ADIE)

void adc_init(void);
uint16_t adc_to_mv(uint32_t adc_value);


#endif /* ADC_H_ */
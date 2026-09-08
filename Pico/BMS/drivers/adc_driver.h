#ifndef ADC_DRIVER_H
#define ADC_DRIVER_H

#include <stdint.h>

void adc_driver_init();

uint16_t adc_driver_get_value(uint8_t channel);

#endif // ADC_DRIVER_H

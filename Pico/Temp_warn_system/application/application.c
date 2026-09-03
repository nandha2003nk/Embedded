#include <stdint.h>
#include <stdio.h>
#include "application.h"
#include "adc/adc_driver.h"
#include "gpio/gpio_driver.h"

#define ADC_Threshold 2000U

void application_init(void)
{
    adc_driver_init();

    gpio_driver_init();
}


void application_update(void)
{   
    uint16_t adc_value;
    adc_value = adc_driver_get_value();
    
    if(adc_value > ADC_Threshold)
    {
        gpio_driver_on();
    }
    else
    {
        gpio_driver_off();
    }
}
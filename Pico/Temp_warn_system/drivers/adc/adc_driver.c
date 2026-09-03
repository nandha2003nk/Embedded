#include "adc_driver.h"
#include "hardware/adc.h"

#define ADC_INPUT_GPIO     26U
#define ADC_INPUT_CHANNEL   0U


void adc_driver_init(void)
{
    adc_init();

    adc_gpio_init(ADC_INPUT_GPIO);

    adc_select_input(ADC_INPUT_CHANNEL);
}


uint16_t adc_driver_get_value(void)
{
    return adc_read();
}
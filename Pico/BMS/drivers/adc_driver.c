
#include "adc_driver.h"
#include "hardware/adc.h"

void adc_driver_init(void)
{
    adc_init();

    adc_gpio_init(26);
    adc_gpio_init(27);
    adc_gpio_init(28);
}

uint16_t adc_driver_get_value(uint8_t channel)
{
    adc_select_input(channel);

    return adc_read();
}
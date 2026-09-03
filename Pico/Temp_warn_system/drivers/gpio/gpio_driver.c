#include "gpio_driver.h"
#include "pico/stdlib.h"

#define LED_GPIO 25U


void gpio_driver_init(void)
{
    gpio_init(LED_GPIO);

    gpio_set_dir(LED_GPIO, GPIO_OUT);

    gpio_put(LED_GPIO, 0);
}


void gpio_driver_on(void)
{
    gpio_put(LED_GPIO, 1);
}


void gpio_driver_off(void)
{
    gpio_put(LED_GPIO, 0);
}
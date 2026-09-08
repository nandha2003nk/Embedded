#ifndef GPIO_DRIVER_H
#define GPIO_DRIVER_H

#include <stdbool.h>

void gpio_driver_init(void);

void gpio_driver_set_load(bool state);
void gpio_driver_set_charging(bool state);

void gpio_driver_set_warning_led(bool state);
void gpio_driver_set_fault_led(bool state);
void gpio_driver_set_buzzer(bool state);

bool gpio_driver_read_power_button(void);

#endif // GPIO_DRIVER_H
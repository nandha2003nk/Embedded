#ifndef DS18B20_DRIVER_H
#define DS18B20_DRIVER_H

#include <stdint.h>
#include <stdbool.h>

bool ds18b20_init(uint8_t gpio_pin);

bool ds18b20_start_conversion(void);
bool ds18b20_read_temperature(float *temperature);

#endif
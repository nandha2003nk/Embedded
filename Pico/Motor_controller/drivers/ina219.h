#ifndef INA219_DRIVER_H
#define INA219_DRIVER_H

#include <stdint.h>
#include <stdbool.h>

#define INA219_DEFAULT_ADDRESS 0x40

bool ina219_init(uint8_t address);

bool ina219_read_bus_voltage(float *voltage);
bool ina219_read_current(float *current);
bool ina219_read_power(float *power);

#endif
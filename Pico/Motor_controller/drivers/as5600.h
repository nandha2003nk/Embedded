#ifndef AS5600_DRIVER_H
#define AS5600_DRIVER_H

#include <stdint.h>
#include <stdbool.h>

#define AS5600_DEFAULT_ADDRESS 0x36

bool as5600_init(uint8_t address);

bool as5600_read_angle(uint16_t *angle);
bool as5600_read_raw_angle(uint16_t *angle);

#endif
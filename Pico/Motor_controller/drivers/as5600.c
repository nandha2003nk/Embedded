#include "as5600.h"

#include "hardware/i2c.h"
#include "hardware/gpio.h"

#define AS5600_I2C_PORT i2c1

#define AS5600_SDA_PIN 6
#define AS5600_SCL_PIN 7

#define AS5600_REG_RAW_ANGLE_H 0x0C
#define AS5600_REG_RAW_ANGLE_L 0x0D

#define AS5600_REG_ANGLE_H     0x0E
#define AS5600_REG_ANGLE_L     0x0F

static uint8_t as5600_address;


/* Read one 16-bit register pair */

static bool as5600_read_register_pair(
    uint8_t high_register,
    uint16_t *value)
{
    uint8_t data[2];

    int result = i2c_write_blocking(
        AS5600_I2C_PORT,
        as5600_address,
        &high_register,
        1,
        true
    );

    if (result != 1)
    {
        return false;
    }

    result = i2c_read_blocking(
        AS5600_I2C_PORT,
        as5600_address,
        data,
        2,
        false
    );

    if (result != 2)
    {
        return false;
    }

    *value = ((uint16_t)data[0] << 8) | data[1];

    return true;
}


/* Initialize AS5600 */

bool as5600_init(uint8_t address)
{
    as5600_address = address;

    i2c_init(AS5600_I2C_PORT, 100000);

    gpio_set_function(AS5600_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(AS5600_SCL_PIN, GPIO_FUNC_I2C);

    gpio_pull_up(AS5600_SDA_PIN);
    gpio_pull_up(AS5600_SCL_PIN);

    uint16_t angle;

    return as5600_read_raw_angle(&angle);
}


/* Read raw 12-bit angle */

bool as5600_read_raw_angle(uint16_t *angle)
{
    uint16_t raw_value;

    if (!as5600_read_register_pair(
            AS5600_REG_RAW_ANGLE_H,
            &raw_value))
    {
        return false;
    }

    raw_value &= 0x0FFF;

    *angle = raw_value;

    return true;
}


/* Read processed 12-bit angle */

bool as5600_read_angle(uint16_t *angle)
{
    uint16_t raw_value;

    if (!as5600_read_register_pair(
            AS5600_REG_ANGLE_H,
            &raw_value))
    {
        return false;
    }

    raw_value &= 0x0FFF;

    *angle = raw_value;

    return true;
}
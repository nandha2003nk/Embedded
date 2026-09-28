#include "ina219.h"

#include "hardware/i2c.h"
#include "hardware/gpio.h"

#define INA219_I2C_PORT i2c0

#define INA219_SDA_PIN 4
#define INA219_SCL_PIN 5

#define INA219_REG_CONFIG     0x00
#define INA219_REG_SHUNT_VOLT 0x01
#define INA219_REG_BUS_VOLT   0x02
#define INA219_REG_POWER      0x03
#define INA219_REG_CURRENT    0x04
#define INA219_REG_CALIB      0x05

#define INA219_CONFIG_VALUE 0x399F
#define INA219_CALIB_VALUE  4096

#define INA219_CURRENT_LSB 0.0001f
#define INA219_POWER_LSB   0.002f

#define INA219_SHUNT_RESISTANCE 0.1f

static uint8_t ina219_address;


/* Write one 16-bit register */

static bool ina219_write_register(uint8_t reg, uint16_t value)
{
    uint8_t data[3];

    data[0] = reg;
    data[1] = (value >> 8) & 0xFF;
    data[2] = value & 0xFF;

    int result = i2c_write_blocking(
        INA219_I2C_PORT,
        ina219_address,
        data,
        3,
        false
    );

    return result == 3;
}


/* Read one 16-bit register */

static bool ina219_read_register(uint8_t reg, uint16_t *value)
{
    uint8_t data[2];

    int result = i2c_write_blocking(
        INA219_I2C_PORT,
        ina219_address,
        &reg,
        1,
        true
    );

    if (result != 1)
    {
        return false;
    }

    result = i2c_read_blocking(
        INA219_I2C_PORT,
        ina219_address,
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


/* Initialize INA219 */

bool ina219_init(uint8_t address)
{
    ina219_address = address;

    i2c_init(INA219_I2C_PORT, 100000);

    gpio_set_function(INA219_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(INA219_SCL_PIN, GPIO_FUNC_I2C);

    gpio_pull_up(INA219_SDA_PIN);
    gpio_pull_up(INA219_SCL_PIN);

    if (!ina219_write_register(INA219_REG_CALIB, INA219_CALIB_VALUE))
    {
        return false;
    }

    if (!ina219_write_register(INA219_REG_CONFIG, INA219_CONFIG_VALUE))
    {
        return false;
    }

    return true;
}


/* Read bus voltage in volts */

bool ina219_read_bus_voltage(float *voltage)
{
    uint16_t raw;

    if (!ina219_read_register(INA219_REG_BUS_VOLT, &raw))
    {
        return false;
    }

    raw >>= 3;

    *voltage = raw * 0.004f;

    return true;
}


/* Read current in amperes */

bool ina219_read_current(float *current)
{
    uint16_t raw;

    if (!ina219_read_register(INA219_REG_CURRENT, &raw))
    {
        return false;
    }

    int16_t signed_raw = (int16_t)raw;

    *current = signed_raw * INA219_CURRENT_LSB;

    return true;
}


/* Read power in watts */

bool ina219_read_power(float *power)
{
    uint16_t raw;

    if (!ina219_read_register(INA219_REG_POWER, &raw))
    {
        return false;
    }

    *power = raw * INA219_POWER_LSB;

    return true;
}
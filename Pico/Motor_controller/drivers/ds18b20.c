#include "ds18b20.h"

#include "hardware/gpio.h"
#include "pico/stdlib.h"

#define DS18B20_CMD_SKIP_ROM       0xCC
#define DS18B20_CMD_CONVERT_T      0x44
#define DS18B20_CMD_READ_SCRATCHPAD 0xBE

#define DS18B20_CONVERSION_TIME_MS 750

static uint8_t ds18b20_pin;


/* Reset the 1-Wire bus */

static bool ds18b20_reset(void)
{
    gpio_set_dir(ds18b20_pin, GPIO_OUT);
    gpio_put(ds18b20_pin, 0);

    sleep_us(480);

    gpio_set_dir(ds18b20_pin, GPIO_IN);

    sleep_us(70);

    bool presence = !gpio_get(ds18b20_pin);

    sleep_us(410);

    return presence;
}


/* Write one bit */

static void ds18b20_write_bit(bool bit)
{
    gpio_set_dir(ds18b20_pin, GPIO_OUT);
    gpio_put(ds18b20_pin, 0);

    if (bit)
    {
        sleep_us(6);

        gpio_set_dir(ds18b20_pin, GPIO_IN);

        sleep_us(64);
    }
    else
    {
        sleep_us(60);

        gpio_set_dir(ds18b20_pin, GPIO_IN);

        sleep_us(10);
    }
}


/* Read one bit */

static bool ds18b20_read_bit(void)
{
    bool bit;

    gpio_set_dir(ds18b20_pin, GPIO_OUT);
    gpio_put(ds18b20_pin, 0);

    sleep_us(6);

    gpio_set_dir(ds18b20_pin, GPIO_IN);

    sleep_us(9);

    bit = gpio_get(ds18b20_pin);

    sleep_us(55);

    return bit;
}


/* Write one byte */

static void ds18b20_write_byte(uint8_t data)
{
    for (uint8_t i = 0; i < 8; i++)
    {
        ds18b20_write_bit(data & 0x01);
        data >>= 1;
    }
}


/* Read one byte */

static uint8_t ds18b20_read_byte(void)
{
    uint8_t data = 0;

    for (uint8_t i = 0; i < 8; i++)
    {
        if (ds18b20_read_bit())
        {
            data |= (1u << i);
        }
    }

    return data;
}


/* Initialize driver */

bool ds18b20_init(uint8_t gpio_pin)
{
    ds18b20_pin = gpio_pin;

    gpio_init(ds18b20_pin);
    gpio_set_dir(ds18b20_pin, GPIO_IN);
    gpio_pull_up(ds18b20_pin);

    return ds18b20_reset();
}


/* Start temperature conversion */

bool ds18b20_start_conversion(void)
{
    if (!ds18b20_reset())
    {
        return false;
    }

    ds18b20_write_byte(DS18B20_CMD_SKIP_ROM);
    ds18b20_write_byte(DS18B20_CMD_CONVERT_T);

    return true;
}


/* Read temperature */

bool ds18b20_read_temperature(float *temperature)
{
    uint8_t scratchpad[9];

    if (!ds18b20_reset())
    {
        return false;
    }

    ds18b20_write_byte(DS18B20_CMD_SKIP_ROM);
    ds18b20_write_byte(DS18B20_CMD_READ_SCRATCHPAD);

    for (uint8_t i = 0; i < 9; i++)
    {
        scratchpad[i] = ds18b20_read_byte();
    }

    int16_t raw_temperature =
        ((int16_t)scratchpad[1] << 8) |
        scratchpad[0];

    *temperature = raw_temperature / 16.0f;

    return true;
}
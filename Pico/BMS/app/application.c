#include "application.h"
#include "adc_driver.h"
#include "gpio_driver.h"
#include <stdint.h>
#include <stdbool.h>

#define BATTERY_CHANNEL      0
#define TEMPERATURE_CHANNEL  1
#define CURRENT_CHANNEL      2

#define ADC_REFERENCE_VOLTAGE    3.3f
#define ADC_MAX_VALUE            4095.0f

#define BATTERY_DIVIDER_RATIO    2.0f

#define LM35_SCALE               0.010f

#define CURRENT_MAX              5.0f

// Thresholds for warnings and critical conditions
#define BATTERY_WARNING_THRESHOLD      3.3f
#define BATTERY_WARNING_CLEAR          3.4f

#define BATTERY_CRITICAL_THRESHOLD     3.0f

#define TEMP_WARNING_THRESHOLD         50.0f
#define TEMP_WARNING_CLEAR             45.0f

#define TEMP_CRITICAL_THRESHOLD        60.0f

#define CURRENT_CRITICAL_THRESHOLD     2.0f

typedef enum
{
    STATE_NORMAL,
    STATE_LOW_BATTERY_WARNING,
    STATE_OVER_TEMP_WARNING,
    STATE_FAULT

} system_state_t;

static system_state_t current_state;

static bool low_battery_warning;
static bool over_temp_warning;

static bool low_battery_fault;
static bool over_temp_fault;
static bool over_current_fault;

static bool fault_latched;

float battery_voltage_from_adc(uint16_t adc_value)
{
    float adc_voltage;
    float battery_voltage;

    adc_voltage =
        (adc_value * ADC_REFERENCE_VOLTAGE) / ADC_MAX_VALUE;

    battery_voltage =
        adc_voltage * BATTERY_DIVIDER_RATIO;

    return battery_voltage;
}

float temperature_from_adc(uint16_t adc_value)
{
    float adc_voltage;
    float temperature;

    adc_voltage =
        (adc_value * ADC_REFERENCE_VOLTAGE) / ADC_MAX_VALUE;

    temperature =
        adc_voltage / LM35_SCALE;

    return temperature;
}

float current_from_adc(uint16_t adc_value)
{
    float adc_voltage;
    float current;

    adc_voltage =
        (adc_value * ADC_REFERENCE_VOLTAGE) / ADC_MAX_VALUE;

    current =
        adc_voltage * (CURRENT_MAX / ADC_REFERENCE_VOLTAGE);

    return current;
}

void application_check_conditions(float battery_voltage,
                                  float temperature,
                                  float load_current)
{
    low_battery_warning = false;
    over_temp_warning = false;

    low_battery_fault = false;
    over_temp_fault = false;
    over_current_fault = false;


    if (battery_voltage < BATTERY_WARNING_THRESHOLD)
    {
        low_battery_warning = true;
    }


    if (temperature > TEMP_WARNING_THRESHOLD)
    {
        over_temp_warning = true;
    }


    if (battery_voltage < BATTERY_CRITICAL_THRESHOLD)
    {
        low_battery_fault = true;
    }


    if (temperature > TEMP_CRITICAL_THRESHOLD)
    {
        over_temp_fault = true;
    }


    if (load_current > CURRENT_CRITICAL_THRESHOLD)
    {
        over_current_fault = true;
    }
}

void application_update_state(float battery_voltage,
                              float temperature,
                              float load_current)
{
    bool power_button_pressed;

    power_button_pressed =
        gpio_driver_read_power_button();
    
    switch (current_state)
    {
        case STATE_NORMAL:

            if (over_current_fault ||
                over_temp_fault ||
                low_battery_fault)
            {
                current_state = STATE_FAULT;
            }
            else if (low_battery_warning)
            {
                current_state = STATE_LOW_BATTERY_WARNING;
            }
            else if (over_temp_warning)
            {
                current_state = STATE_OVER_TEMP_WARNING;
            }

            break;


        case STATE_LOW_BATTERY_WARNING:

            if (over_current_fault ||
                over_temp_fault ||
                low_battery_fault)
            {
                current_state = STATE_FAULT;
            }
            else if (battery_voltage > BATTERY_WARNING_CLEAR)
            {
                current_state = STATE_NORMAL;
            }

            break;


        case STATE_OVER_TEMP_WARNING:

            if (over_current_fault ||
                over_temp_fault ||
                low_battery_fault)
            {
                current_state = STATE_FAULT;
            }
            else if (temperature < TEMP_WARNING_CLEAR)
            {
                current_state = STATE_NORMAL;
            }

            break;


        case STATE_FAULT:

            /*
             * Stay in FAULT until the user
             * presses the power button and
             * all critical conditions have cleared.
             */

            if (power_button_pressed &&
                !over_current_fault &&
                !over_temp_fault &&
                !low_battery_fault)
            {
                fault_latched = false;
                current_state = STATE_NORMAL;
            }

            break;


        default:
            {
                current_state = STATE_FAULT;
            }
            break;
    }
}

void application_update_outputs(void)
{
    switch (current_state)
    {
        case STATE_NORMAL:

            gpio_driver_set_load(true);
            gpio_driver_set_charging(true);

            gpio_driver_set_warning_led(false);
            gpio_driver_set_fault_led(false);
            gpio_driver_set_buzzer(false);

            break;


        case STATE_LOW_BATTERY_WARNING:

            gpio_driver_set_load(true);
            gpio_driver_set_charging(true);

            gpio_driver_set_warning_led(true);
            gpio_driver_set_fault_led(false);
            gpio_driver_set_buzzer(false);

            break;


        case STATE_OVER_TEMP_WARNING:

            gpio_driver_set_load(true);
            gpio_driver_set_charging(false);

            gpio_driver_set_warning_led(true);
            gpio_driver_set_fault_led(false);
            gpio_driver_set_buzzer(false);

            break;


        case STATE_FAULT:

            gpio_driver_set_load(false);
            gpio_driver_set_charging(false);

            gpio_driver_set_warning_led(false);
            gpio_driver_set_fault_led(true);
            gpio_driver_set_buzzer(true);

            break;


        default:

            gpio_driver_set_load(false);
            gpio_driver_set_charging(false);

            gpio_driver_set_warning_led(false);
            gpio_driver_set_fault_led(true);
            gpio_driver_set_buzzer(true);

            break;
    }
}

void application_init()
{
    adc_driver_init();
    gpio_driver_init();

    current_state = STATE_NORMAL;
    fault_latched = false;
}

void application_update()
{
    uint16_t battery_raw;
    uint16_t temperature_raw;
    uint16_t current_raw;

    float battery_voltage;
    float temperature;
    float load_current;

    battery_raw =
        adc_driver_get_value(BATTERY_CHANNEL);

    temperature_raw =
        adc_driver_get_value(TEMPERATURE_CHANNEL);

    current_raw =
        adc_driver_get_value(CURRENT_CHANNEL);


    battery_voltage =
        battery_voltage_from_adc(battery_raw);

    temperature =
        temperature_from_adc(temperature_raw);

    load_current =
        current_from_adc(current_raw);

    application_check_conditions(battery_voltage,
                                 temperature,
                                 load_current);
    application_update_state(battery_voltage,
                             temperature,
                             load_current);

    application_update_outputs();
}
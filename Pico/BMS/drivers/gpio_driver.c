#include "gpio_driver.h"
#include "hardware/gpio.h"

#define LOAD_EN_PIN       2
#define CHARGE_EN_PIN     3
#define WARNING_LED_PIN   4
#define FAULT_LED_PIN     5
#define BUZZER_PIN        6
#define POWER_BUTTON_PIN  7

void gpio_driver_init(void)
{
    gpio_init(LOAD_EN_PIN);
    gpio_set_dir(LOAD_EN_PIN, GPIO_OUT);
    gpio_put(LOAD_EN_PIN, false);

    gpio_init(CHARGE_EN_PIN);
    gpio_set_dir(CHARGE_EN_PIN, GPIO_OUT);
    gpio_put(CHARGE_EN_PIN, false);

    gpio_init(WARNING_LED_PIN);
    gpio_set_dir(WARNING_LED_PIN, GPIO_OUT);
    gpio_put(WARNING_LED_PIN, false);

    gpio_init(FAULT_LED_PIN);
    gpio_set_dir(FAULT_LED_PIN, GPIO_OUT);
    gpio_put(FAULT_LED_PIN, false);

    gpio_init(BUZZER_PIN);
    gpio_set_dir(BUZZER_PIN, GPIO_OUT);
    gpio_put(BUZZER_PIN, false);

    gpio_init(POWER_BUTTON_PIN);
    gpio_set_dir(POWER_BUTTON_PIN, GPIO_IN);
    gpio_pull_up(POWER_BUTTON_PIN);
}


void gpio_driver_set_load(bool state)
{
    gpio_put(LOAD_EN_PIN, state);
}

void gpio_driver_set_charging(bool state)
{
    gpio_put(CHARGE_EN_PIN, state);
}

void gpio_driver_set_warning_led(bool state)
{
    gpio_put(WARNING_LED_PIN, state);
}

void gpio_driver_set_fault_led(bool state)
{
    gpio_put(FAULT_LED_PIN, state);
}

void gpio_driver_set_buzzer(bool state)
{
    gpio_put(BUZZER_PIN, state);
}

bool gpio_driver_read_power_button(void)
{
    return !gpio_get(POWER_BUTTON_PIN);
}


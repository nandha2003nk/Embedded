#include "gpio.h"
#include "hardware/gpio.h"

#define GPIO_DRIVER_MAX_PINS 30

static gpio_driver_irq_callback_t gpio_callbacks[GPIO_DRIVER_MAX_PINS] = {0};


/* Interrupt handler used internally by our driver */

static void gpio_driver_irq_handler(uint gpio, uint32_t events)
{
    if (gpio >= GPIO_DRIVER_MAX_PINS)
    {
        return;
    }

    gpio_driver_irq_callback_t callback =
        gpio_callbacks[gpio];

    if (callback == NULL)
    {
        return;
    }

    callback(
        (uint8_t)gpio,
        events
    );
}


/* Initialize GPIO */

bool gpio_driver_init(uint8_t pin, gpio_mode_t mode)
{
    if (pin >= GPIO_DRIVER_MAX_PINS)
    {
        return false;
    }

    gpio_init(pin);

    switch (mode)
    {
        case GPIO_MODE_INPUT:
            gpio_set_dir(pin, GPIO_IN);
            gpio_disable_pulls(pin);
            break;

        case GPIO_MODE_INPUT_PULLUP:
            gpio_set_dir(pin, GPIO_IN);
            gpio_pull_up(pin);
            break;

        case GPIO_MODE_INPUT_PULLDOWN:
            gpio_set_dir(pin, GPIO_IN);
            gpio_pull_down(pin);
            break;

        case GPIO_MODE_OUTPUT:
            gpio_set_dir(pin, GPIO_OUT);
            break;

        default:
            return false;
    }

    return true;
}


/* Write GPIO state */

bool gpio_driver_write(uint8_t pin, gpio_state_t state)
{
    if (pin >= GPIO_DRIVER_MAX_PINS)
    {
        return false;
    }

    gpio_put(pin, state == GPIO_HIGH);

    return true;
}


/* Read GPIO state */

gpio_state_t gpio_driver_read(uint8_t pin)
{
    if (pin >= GPIO_DRIVER_MAX_PINS)
    {
        return GPIO_LOW;
    }

    if (gpio_get(pin))
    {
        return GPIO_HIGH;
    }

    return GPIO_LOW;
}


/* Toggle GPIO state */

bool gpio_driver_toggle(uint8_t pin)
{
    if (pin >= GPIO_DRIVER_MAX_PINS)
    {
        return false;
    }

    gpio_put(pin, !gpio_get(pin));

    return true;
}


/* Configure GPIO interrupt */

bool gpio_driver_set_irq(uint8_t pin,
                         gpio_irq_event_t events,
                         gpio_driver_irq_callback_t callback)
{
    if (pin >= GPIO_DRIVER_MAX_PINS)
    {
        return false;
    }

    if (callback == NULL)
    {
        return false;
    }

    gpio_callbacks[pin] = callback;

    gpio_set_irq_enabled_with_callback(
        pin,
        (uint32_t)events,
        true,
        &gpio_driver_irq_handler
    );

    return true;
}
#ifndef GPIO_DRIVER_H
#define GPIO_DRIVER_H

#include <stdint.h>
#include <stdbool.h>

typedef enum
{
    GPIO_MODE_INPUT,
    GPIO_MODE_INPUT_PULLUP,
    GPIO_MODE_INPUT_PULLDOWN,
    GPIO_MODE_OUTPUT
} gpio_mode_t;

typedef enum
{
    GPIO_LOW,
    GPIO_HIGH
} gpio_state_t;

typedef enum
{
    GPIO_IRQ_FALL = 0x04,
    GPIO_IRQ_RISE = 0x08,
    GPIO_IRQ_BOTH = 0x0C
} gpio_irq_event_t;

/*
 * Our driver's interrupt callback type.
 *
 * This has a different name from the Pico SDK's
 * gpio_irq_callback_t.
 */
typedef void (*gpio_driver_irq_callback_t)(uint8_t pin,
                                           uint32_t events);

bool gpio_driver_init(uint8_t pin,
                      gpio_mode_t mode);

bool gpio_driver_write(uint8_t pin,
                       gpio_state_t state);

gpio_state_t gpio_driver_read(uint8_t pin);

bool gpio_driver_toggle(uint8_t pin);

bool gpio_driver_set_irq(uint8_t pin,
                         gpio_irq_event_t events,
                         gpio_driver_irq_callback_t callback);

#endif
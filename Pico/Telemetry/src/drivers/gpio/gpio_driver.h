#ifndef GPIO_DRIVER_H
#define GPIO_DRIVER_H

#include <stdbool.h>
#include <stdint.h>

/* ============================================================
 * GPIO configuration
 * ============================================================ */

#define GPIO_LED_PIN        25U
#define GPIO_BUTTON_PIN     15U

#define GPIO_DEBOUNCE_MS    30U


/* ============================================================
 * GPIO types
 * ============================================================ */

typedef enum
{
    GPIO_BUTTON_RELEASED = 0,
    GPIO_BUTTON_PRESSED
} GPIO_ButtonState_t;


/* ============================================================
 * GPIO initialization
 * ============================================================ */

void gpio_driver_init(void);


/* ============================================================
 * LED API
 * ============================================================ */

void gpio_led_on(void);

void gpio_led_off(void);

void gpio_led_toggle(void);

bool gpio_led_get_state(void);


/* ============================================================
 * Button API
 * ============================================================ */

GPIO_ButtonState_t gpio_button_get_state(void);

bool gpio_button_was_pressed(void);

bool gpio_button_was_released(void);


/* ============================================================
 * GPIO update
 *
 * Must be called periodically from the main loop.
 * ============================================================ */

void gpio_driver_update(uint32_t current_time_ms);

#endif /* GPIO_DRIVER_H */
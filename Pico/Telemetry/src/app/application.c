#include "application.h"

#include <stdio.h>

#include "gpio_driver.h"


/* ============================================================
 * Application initialization
 * ============================================================ */

void application_init(void)
{
    gpio_driver_init();

    printf("Application initialized\n");
}


/* ============================================================
 * Application update
 * ============================================================ */

void application_update(unsigned int current_time_ms)
{
    /*
     * Update GPIO driver.
     *
     * This handles:
     * - button sampling
     * - debouncing
     * - press detection
     * - release detection
     */

    gpio_driver_update(current_time_ms);


    /* --------------------------------------------------------
     * Button pressed
     * -------------------------------------------------------- */

    if (gpio_button_was_pressed())
    {
        gpio_led_toggle();

        printf("Button pressed - LED toggled: %s\n",
               gpio_led_get_state() ? "ON" : "OFF");
    }


    /* --------------------------------------------------------
     * Button released
     * -------------------------------------------------------- */

    if (gpio_button_was_released())
    {
        printf("Button released\n");
    }
}
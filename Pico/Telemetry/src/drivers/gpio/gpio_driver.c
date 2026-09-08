#include "gpio_driver.h"

#include "pico/stdlib.h"


/* ============================================================
 * Private variables
 * ============================================================ */

static bool led_state = false;

static GPIO_ButtonState_t button_state = GPIO_BUTTON_RELEASED;

static GPIO_ButtonState_t button_last_raw_state =
    GPIO_BUTTON_RELEASED;

static bool button_pressed_event = false;

static bool button_released_event = false;

static uint32_t button_last_change_time = 0U;


/* ============================================================
 * Private functions
 * ============================================================ */

static GPIO_ButtonState_t gpio_button_read_raw(void)
{
    /*
     * Button is connected between GPIO15 and GND.
     *
     * Internal pull-up means:
     *
     * Button released -> GPIO HIGH
     * Button pressed  -> GPIO LOW
     */

    if (gpio_get(GPIO_BUTTON_PIN) == 0)
    {
        return GPIO_BUTTON_PRESSED;
    }

    return GPIO_BUTTON_RELEASED;
}


/* ============================================================
 * Initialization
 * ============================================================ */

void gpio_driver_init(void)
{
    /* ---------------- LED ---------------- */

    gpio_init(GPIO_LED_PIN);

    gpio_set_dir(GPIO_LED_PIN, GPIO_OUT);

    gpio_put(GPIO_LED_PIN, 0);

    led_state = false;


    /* ---------------- Button ---------------- */

    gpio_init(GPIO_BUTTON_PIN);

    gpio_set_dir(GPIO_BUTTON_PIN, GPIO_IN);

    gpio_pull_up(GPIO_BUTTON_PIN);


    /* Initial button state */

    button_state = gpio_button_read_raw();

    button_last_raw_state = button_state;

    button_pressed_event = false;

    button_released_event = false;

    button_last_change_time = to_ms_since_boot(get_absolute_time());
}


/* ============================================================
 * LED functions
 * ============================================================ */

void gpio_led_on(void)
{
    gpio_put(GPIO_LED_PIN, 1);

    led_state = true;
}


void gpio_led_off(void)
{
    gpio_put(GPIO_LED_PIN, 0);

    led_state = false;
}


void gpio_led_toggle(void)
{
    if (led_state == true)
    {
        gpio_led_off();
    }
    else
    {
        gpio_led_on();
    }
}


bool gpio_led_get_state(void)
{
    return led_state;
}


/* ============================================================
 * Button state
 * ============================================================ */

GPIO_ButtonState_t gpio_button_get_state(void)
{
    return button_state;
}


/* ============================================================
 * Button event functions
 * ============================================================ */

bool gpio_button_was_pressed(void)
{
    bool event = button_pressed_event;

    /*
     * Event is consumed after being read.
     */

    button_pressed_event = false;

    return event;
}


bool gpio_button_was_released(void)
{
    bool event = button_released_event;

    /*
     * Event is consumed after being read.
     */

    button_released_event = false;

    return event;
}


/* ============================================================
 * GPIO update
 * ============================================================ */

void gpio_driver_update(uint32_t current_time_ms)
{
    GPIO_ButtonState_t raw_state;

    raw_state = gpio_button_read_raw();


    /* --------------------------------------------------------
     * Detect raw state change
     * -------------------------------------------------------- */

    if (raw_state != button_last_raw_state)
    {
        button_last_raw_state = raw_state;

        button_last_change_time = current_time_ms;
    }


    /* --------------------------------------------------------
     * Check whether state has remained stable long enough
     * -------------------------------------------------------- */

    if ((current_time_ms - button_last_change_time)
        >= GPIO_DEBOUNCE_MS)
    {
        /*
         * Only update the confirmed state if it differs
         * from the current debounced state.
         */

        if (button_state != button_last_raw_state)
        {
            button_state = button_last_raw_state;


            /* --------------------------------------------
             * Generate events
             * -------------------------------------------- */

            if (button_state == GPIO_BUTTON_PRESSED)
            {
                button_pressed_event = true;
            }
            else
            {
                button_released_event = true;
            }
        }
    }
}
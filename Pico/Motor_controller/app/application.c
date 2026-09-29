#include "application.h"
#include "state_machine.h"
#include "drivers/gpio.h"
#include "middleware/debounce.h"
#include "config/pin_config.h"

static debounce_t button_debounce;

static volatile bool button_irq_pending = false;


/* Button interrupt callback */

static void button_irq_callback(uint8_t pin,
                                uint32_t events)
{
    (void)pin;
    (void)events;

    button_irq_pending = true;
}


/* Initialize application */

void application_init(void)
{
    gpio_driver_init(
        BUTTON_GPIO,
        GPIO_MODE_INPUT_PULLDOWN
    );

    debounce_init(
        &button_debounce,
        false,
        20
    );

    gpio_driver_set_irq(
        BUTTON_GPIO,
        GPIO_IRQ_BOTH,
        button_irq_callback
    );

    state_machine_init();
}


/* Update application */

void application_update(void)
{
    if (button_irq_pending)
    {
        button_irq_pending = false;
    }

    gpio_state_t raw_gpio_state;

    raw_gpio_state =
        gpio_driver_read(BUTTON_GPIO);

    bool raw_button_state =
        (raw_gpio_state == GPIO_HIGH);

    bool state_changed =
        debounce_update(
            &button_debounce,
            raw_button_state
        );

    if (state_changed)
    {
        if (button_debounce.stable_state)
        {
            state_machine_process_event(EVENT_START);
        }
        else
        {
            state_machine_process_event(EVENT_STOP);
        }
    }
}
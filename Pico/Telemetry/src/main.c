#include "pico/stdlib.h"

#include "application.h"


int main(void)
{
    /* --------------------------------------------------------
     * Initialize Pico standard library
     * -------------------------------------------------------- */

    stdio_init_all();


    /* --------------------------------------------------------
     * Initialize application
     * -------------------------------------------------------- */

    application_init();


    /* --------------------------------------------------------
     * Main loop
     * -------------------------------------------------------- */

    while (true)
    {
        uint32_t current_time_ms;

        current_time_ms =
            to_ms_since_boot(get_absolute_time());


        application_update(current_time_ms);
    }


    return 0;
}
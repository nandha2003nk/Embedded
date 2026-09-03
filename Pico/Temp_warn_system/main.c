#include <stdio.h>
#include "pico/stdlib.h"
#include "application.h"


int main(void)
{
    stdio_init_all();

    application_init();

    while (true) {
        application_update();
    }
    return 0;
    
}

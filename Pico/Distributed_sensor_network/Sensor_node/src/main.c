#include "pico/stdlib.h"

#include "app.h"

int main(void)
{
    app_init();

    while (true)
    {
        app_run();
    }

    return 0;
}
#include "pico/stdlib.h"
#include "application.h"

int main() {

    stdio_init_all();
    application_init();



while (true)
{
    application_update();
}

}

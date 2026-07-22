#include "spi_driver.h"

#include "pico/stdlib.h"
#include "hardware/spi.h"

/* ------------ Pin Configuration ------------ */

#define SPI_PORT    spi0

#define PIN_MISO    16
#define PIN_CS      17
#define PIN_SCK     18
#define PIN_MOSI    19

#define SPI_BAUDRATE    1000000   // 1 MHz

/* ------------ Public Functions ------------ */

void spi_driver_init(void)
{
    /* Initialize SPI peripheral */
    spi_init(SPI_PORT, SPI_BAUDRATE);

    /* Configure SPI GPIO pins */
    gpio_set_function(PIN_MISO, GPIO_FUNC_SPI);
    gpio_set_function(PIN_MOSI, GPIO_FUNC_SPI);
    gpio_set_function(PIN_SCK, GPIO_FUNC_SPI);

    /* Configure Chip Select pin */
    gpio_init(PIN_CS);
    gpio_set_dir(PIN_CS, GPIO_OUT);

    /* Keep CS inactive */
    gpio_put(PIN_CS, 1);
}

void spi_cs_select(void)
{
    gpio_put(PIN_CS, 0);
}

void spi_cs_deselect(void)
{
    gpio_put(PIN_CS, 1);
}

void spi_transfer(const uint8_t *tx_buffer,
                  uint8_t *rx_buffer,
                  size_t length)
{
    spi_write_read_blocking(SPI_PORT,
                            tx_buffer,
                            rx_buffer,
                            length);
}

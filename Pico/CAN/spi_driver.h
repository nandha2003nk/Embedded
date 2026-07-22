#ifndef SPI_DRIVER_H
#define SPI_DRIVER_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* Initialize SPI peripheral and GPIO pins */
void spi_driver_init(void);

/* Control Chip Select */
void spi_cs_select(void);
void spi_cs_deselect(void);

/* Full-duplex SPI transfer */
void spi_transfer(const uint8_t *tx_buffer,
                  uint8_t *rx_buffer,
                  size_t length);

#endif
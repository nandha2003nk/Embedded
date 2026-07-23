#include "mcp2515.h"
#include "spi_driver.h"

/* MCP2515 SPI Commands */
#define MCP2515_CMD_RESET         0xC0
#define MCP2515_CMD_READ          0x03
#define MCP2515_CMD_WRITE         0x02
#define MCP2515_CMD_READ_STATUS   0xA0

void mcp2515_reset(void)
{
    uint8_t tx = MCP2515_CMD_RESET;
    uint8_t rx;

    spi_cs_select();
    spi_transfer(&tx, &rx, 1);
    spi_cs_deselect();
}

uint8_t mcp2515_read_register(uint8_t address)
{
    uint8_t tx[3];
    uint8_t rx[3];

    tx[0] = MCP2515_CMD_READ;
    tx[1] = address;
    tx[2] = 0x00;      // dummy byte

    spi_cs_select();
    spi_transfer(tx, rx, 3);
    spi_cs_deselect();

    return rx[2];
}

void mcp2515_write_register(uint8_t address, uint8_t value)
{
    uint8_t tx[3];
    uint8_t rx[3];

    tx[0] = MCP2515_CMD_WRITE;
    tx[1] = address;
    tx[2] = value;

    spi_cs_select();
    spi_transfer(tx, rx, 3);
    spi_cs_deselect();
}

uint8_t mcp2515_read_status(void)
{
    uint8_t tx[2] = {MCP2515_CMD_READ_STATUS, 0x00};
    uint8_t rx[2];

    spi_cs_select();
    spi_transfer(tx, rx, 2);
    spi_cs_deselect();

    return rx[1];
}
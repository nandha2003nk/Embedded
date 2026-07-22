#ifndef MCP2515_H
#define MCP2515_H

#include <stdint.h>

void mcp2515_reset(void);

uint8_t mcp2515_read_register(uint8_t address);

void mcp2515_write_register(uint8_t address,
                            uint8_t value);

uint8_t mcp2515_read_status(void);

#endif
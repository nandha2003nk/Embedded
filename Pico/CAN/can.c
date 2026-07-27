#include "can.h"
#include "mcp2515.h"

/* ---------------- Register Addresses ---------------- */

#define TXB0CTRL    0x30

#define TXB0SIDH    0x31
#define TXB0SIDL    0x32

#define TXB0DLC     0x35

#define TXB0D0      0x36


/* ---------------------------------------------------- */

bool can_send(const CAN_Frame *frame)
{
    uint8_t sidh;
    uint8_t sidl;

    /* Convert 11-bit CAN ID into MCP2515 format */
    sidh = (uint8_t)(frame->id >> 3);
    sidl = (uint8_t)((frame->id & 0x07) << 5);

    /* Load Standard ID */
    mcp2515_write_register(TXB0SIDH, sidh);
    mcp2515_write_register(TXB0SIDL, sidl);

    /* Load Data Length Code */
    mcp2515_write_register(TXB0DLC, frame->dlc);

    /* Load Data Bytes */
    for(uint8_t i = 0; i < frame->dlc; i++)
    {
        mcp2515_write_register(TXB0D0 + i, frame->data[i]);
    }

    /* Request Transmission */
    mcp2515_write_register(TXB0CTRL, 0x08);

    return true;
}
#ifndef CAN_H
#define CAN_H

#include <stdint.h>
#include <stdbool.h>

typedef struct
{
    uint16_t id;      // Standard 11-bit CAN ID
    uint8_t dlc;      // Data Length Code (0-8)
    uint8_t data[8];
} CAN_Frame;

bool can_send(const CAN_Frame *frame);

#endif
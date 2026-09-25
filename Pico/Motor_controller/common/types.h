#ifndef TYPES_H
#define TYPES_H

#include <stdint.h>
#include <stdbool.h>


typedef enum
{
    EVENT_NONE,
    EVENT_START,
    EVENT_STOP,
    EVENT_FAULT,
    EVENT_RESET

} system_event_t;


#endif
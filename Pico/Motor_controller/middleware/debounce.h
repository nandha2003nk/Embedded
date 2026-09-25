#ifndef DEBOUNCE_H
#define DEBOUNCE_H

#include <stdint.h>
#include <stdbool.h>


typedef struct
{
    bool stable_state;
    bool last_reading;

    uint32_t last_change_time;

    uint32_t debounce_time_ms;

} debounce_t;


void debounce_init(debounce_t *db,
                   bool initial_state,
                   uint32_t debounce_time_ms);


bool debounce_update(debounce_t *db,
                     bool current_reading);


#endif
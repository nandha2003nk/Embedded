#include "debounce.h"

#include "pico/stdlib.h"


void debounce_init(debounce_t *db,
                   bool initial_state,
                   uint32_t debounce_time_ms)
{
    db->stable_state = initial_state;
    db->last_reading = initial_state;

    db->last_change_time = to_ms_since_boot(get_absolute_time());

    db->debounce_time_ms = debounce_time_ms;
}


bool debounce_update(debounce_t *db,
                     bool current_reading)
{
    uint32_t current_time =
        to_ms_since_boot(get_absolute_time());


    if (current_reading != db->last_reading)
    {
        db->last_reading = current_reading;

        db->last_change_time = current_time;
    }


    if ((current_time - db->last_change_time)
        >= db->debounce_time_ms)
    {
        if (db->stable_state != db->last_reading)
        {
            db->stable_state = db->last_reading;

            return true;
        }
    }


    return false;
}
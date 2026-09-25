#include "state_machine.h"

#include "common/types.h"   


static system_state_t current_state;


void state_machine_init(void)
{
    current_state = STATE_INIT;
}


void state_machine_process_event(system_event_t event)
{
    switch (current_state)
    {
        case STATE_INIT:

            break;


        case STATE_IDLE:

            if (event == EVENT_START)
            {
                current_state = STATE_RUNNING;
            }
            else if (event == EVENT_FAULT)
            {
                current_state = STATE_FAULT;
            }

            break;


        case STATE_RUNNING:

            if (event == EVENT_STOP)
            {
                current_state = STATE_IDLE;
            }
            else if (event == EVENT_FAULT)
            {
                current_state = STATE_FAULT;
            }

            break;


        case STATE_FAULT:

            if (event == EVENT_RESET)
            {
                current_state = STATE_IDLE;
            }

            break;


        default:

            current_state = STATE_FAULT;

            break;
    }
}


void state_machine_update(void)
{
    if (current_state == STATE_INIT)
    {
        current_state = STATE_IDLE;
    }
}


system_state_t state_machine_get_state(void)
{
    return current_state;
}
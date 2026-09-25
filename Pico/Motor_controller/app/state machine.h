#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

typedef enum
{
    STATE_INIT,
    STATE_IDLE,
    STATE_RUNNING,
    STATE_FAULT

} system_state_t;

typedef enum
{
    EVENT_NONE,
    EVENT_START,
    EVENT_STOP,
    EVENT_FAULT,
    EVENT_RESET

} system_event_t;

void state_machine_init(void);

void state_machine_update(void);

system_state_t state_machine_get_state(void);

void state_machine_process_event(system_event_t event);

#endif
#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <stdint.h>
#include <stdbool.h>

/* Maximum number of scheduled tasks */
#define SCHEDULER_MAX_TASKS  8

/* Task callback function */
typedef void (*scheduler_task_t)(void);

/* Scheduler task structure */
typedef struct
{
    scheduler_task_t task;
    uint32_t period_ms;
    uint32_t last_run_ms;
    bool enabled;
} scheduler_entry_t;


/* Initialize scheduler */
void scheduler_init(void);

/* Add a periodic task */
bool scheduler_add_task(scheduler_task_t task,
                        uint32_t period_ms);

/* Run scheduler */
void scheduler_run(void);

#endif /* SCHEDULER_H */
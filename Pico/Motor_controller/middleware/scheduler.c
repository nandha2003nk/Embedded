#include "scheduler.h"

#include "pico/stdlib.h"


static scheduler_entry_t scheduler_tasks[SCHEDULER_MAX_TASKS];

static uint8_t scheduler_task_count = 0;


void scheduler_init(void)
{
    scheduler_task_count = 0;

    for (uint8_t i = 0; i < SCHEDULER_MAX_TASKS; i++)
    {
        scheduler_tasks[i].task = NULL;
        scheduler_tasks[i].period_ms = 0;
        scheduler_tasks[i].last_run_ms = 0;
        scheduler_tasks[i].enabled = false;
    }
}


bool scheduler_add_task(scheduler_task_t task,
                        uint32_t period_ms)
{
    if (task == NULL)
    {
        return false;
    }

    if (period_ms == 0)
    {
        return false;
    }

    if (scheduler_task_count >= SCHEDULER_MAX_TASKS)
    {
        return false;
    }

    scheduler_tasks[scheduler_task_count].task = task;
    scheduler_tasks[scheduler_task_count].period_ms = period_ms;
    scheduler_tasks[scheduler_task_count].last_run_ms = to_ms_since_boot(get_absolute_time());
    scheduler_tasks[scheduler_task_count].enabled = true;

    scheduler_task_count++;

    return true;
}


void scheduler_run(void)
{
    uint32_t current_time_ms =
        to_ms_since_boot(get_absolute_time());

    for (uint8_t i = 0; i < scheduler_task_count; i++)
    {
        scheduler_entry_t *entry = &scheduler_tasks[i];

        if (!entry->enabled)
        {
            continue;
        }

        if ((current_time_ms - entry->last_run_ms) >= entry->period_ms)
        {
            entry->last_run_ms = current_time_ms;

            entry->task();
        }
    }
}
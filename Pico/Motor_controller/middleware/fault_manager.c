#include "fault_manager.h"
#include "pico/stdlib.h"

/* Internal state */

static uint32_t active_faults = 0;
static uint32_t latched_faults = 0;

static fault_history_entry_t fault_history[FAULT_HISTORY_SIZE];

static uint32_t fault_occurrence_count[FAULT_COUNT];

static uint8_t fault_history_write_index = 0;
static uint8_t fault_history_count = 0;


/* Internal helper */

static uint32_t fault_manager_get_time_ms(void)
{
    return to_ms_since_boot(get_absolute_time());
}


/* Initialization */

void fault_manager_init(void)
{
    active_faults = 0;
    latched_faults = 0;

    fault_history_write_index = 0;
    fault_history_count = 0;

    for (uint8_t i = 0; i < FAULT_HISTORY_SIZE; i++)
    {
        fault_history[i].fault_id = FAULT_OVERCURRENT;
        fault_history[i].timestamp_ms = 0;
        fault_history[i].occurrence_count = 0;
        fault_history[i].valid = false;
    }

    for (uint8_t i = 0; i < FAULT_COUNT; i++)
    {
        fault_occurrence_count[i] = 0;
    }
}


/* Set fault */

void fault_manager_set_fault(fault_id_t fault)
{
    if (fault >= FAULT_COUNT)
    {
        return;
    }

    uint32_t mask = FAULT_MASK(fault);

    /*
     * Check whether this fault was already active.
     *
     * A fault should create a new history entry only when
     * it changes from inactive -> active.
     */
    bool was_active = (active_faults & mask) != 0;

    /* Mark fault as active */
    active_faults |= mask;

    /* Remember that this fault has occurred */
    latched_faults |= mask;

    /*
     * If the fault was already active, do not create
     * another history entry.
     */
    if (was_active)
    {
        return;
    }

    /* Count this occurrence */
    fault_occurrence_count[fault]++;

    /* Store the new history entry */
    fault_history[fault_history_write_index].fault_id = fault;
    fault_history[fault_history_write_index].timestamp_ms =
        fault_manager_get_time_ms();
    fault_history[fault_history_write_index].occurrence_count =
        fault_occurrence_count[fault];
    fault_history[fault_history_write_index].valid = true;

    /* Move to the next history slot */
    fault_history_write_index++;

    if (fault_history_write_index >= FAULT_HISTORY_SIZE)
    {
        fault_history_write_index = 0;
    }

    /* Increase history count until the buffer becomes full */
    if (fault_history_count < FAULT_HISTORY_SIZE)
    {
        fault_history_count++;
    }
}


/* Clear active fault */

void fault_manager_clear_fault(fault_id_t fault)
{
    if (fault >= FAULT_COUNT)
    {
        return;
    }

    uint32_t mask = FAULT_MASK(fault);

    /*
     * Clear only the active status.
     *
     * The latched fault and history remain.
     */
    active_faults &= ~mask;
}


/* Check whether one fault is active */

bool fault_manager_is_active(fault_id_t fault)
{
    if (fault >= FAULT_COUNT)
    {
        return false;
    }

    return (active_faults & FAULT_MASK(fault)) != 0;
}


/* Check whether any fault is active */

bool fault_manager_has_any_fault(void)
{
    return active_faults != 0;
}


/* Get active fault mask */

uint32_t fault_manager_get_active_faults(void)
{
    return active_faults;
}


/* Get latched fault mask */

uint32_t fault_manager_get_latched_faults(void)
{
    return latched_faults;
}


/* Get fault history */

const fault_history_entry_t *fault_manager_get_history(void)
{
    return fault_history;
}


/* Get number of valid history entries */

uint8_t fault_manager_get_history_count(void)
{
    return fault_history_count;
}
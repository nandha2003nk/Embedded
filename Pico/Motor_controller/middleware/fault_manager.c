#include "fault_manager.h"

static uint32_t active_faults;
static uint32_t fault_history;


void fault_manager_init(void)
{
    active_faults = FAULT_NONE;
    fault_history = FAULT_NONE;
}


void fault_manager_set_fault(fault_type_t fault)
{
    active_faults |= fault;
    fault_history |= fault;
}


void fault_manager_clear_fault(fault_type_t fault)
{
    active_faults &= ~fault;
}


bool fault_manager_is_active(fault_type_t fault)
{
    return (active_faults & fault) != 0;
}


uint32_t fault_manager_get_active_faults(void)
{
    return active_faults;
}


uint32_t fault_manager_get_fault_history(void)
{
    return fault_history;
}
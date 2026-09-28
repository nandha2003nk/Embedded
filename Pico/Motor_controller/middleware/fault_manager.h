#ifndef FAULT_MANAGER_H
#define FAULT_MANAGER_H

#include <stdint.h>
#include <stdbool.h>

/* Number of entries stored in fault history */
#define FAULT_HISTORY_SIZE 10

/* Fault identifiers */
typedef enum
{
    FAULT_OVERCURRENT = 0,
    FAULT_OVERVOLTAGE,
    FAULT_UNDERVOLTAGE,
    FAULT_OVERTEMPERATURE,
    FAULT_MOTOR_STALL,
    FAULT_SENSOR,

    FAULT_COUNT
} fault_id_t;

/* Convert a fault ID into its bitmask */
#define FAULT_MASK(fault) (1u << (fault))

/* One entry in the fault history */
typedef struct
{
    fault_id_t fault_id;
    uint32_t timestamp_ms;
    uint32_t occurrence_count;
    bool valid;
} fault_history_entry_t;

/* Initialization */
void fault_manager_init(void);

/* Fault handling */
void fault_manager_set_fault(fault_id_t fault);
void fault_manager_clear_fault(fault_id_t fault);

/* Active fault checking */
bool fault_manager_is_active(fault_id_t fault);
bool fault_manager_has_any_fault(void);

/* Fault masks */
uint32_t fault_manager_get_active_faults(void);
uint32_t fault_manager_get_latched_faults(void);

/* Fault history */
const fault_history_entry_t *fault_manager_get_history(void);
uint8_t fault_manager_get_history_count(void);

#endif
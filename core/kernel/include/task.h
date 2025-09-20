#pragma once
#include "types.h"

typedef enum {
	TASK_STATE_READY = 0,
	TASK_STATE_RUNNING = 1,
	TASK_STATE_SLEEPING = 2,
	TASK_STATE_ZOMBIE = 3
} task_state_t;

typedef struct task_control_block {
	u32 id;
	task_state_t state;
	struct task_control_block* next;
} task_t;

void scheduler_init(void);
void scheduler_tick(void);
void scheduler_add(task_t* task);
task_t* scheduler_current(void);



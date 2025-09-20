#include "../include/task.h"
#include "../include/log.h"

static task_t idle_task = { .id = 0, .state = TASK_STATE_RUNNING, .next = 0 };
static task_t* current_task = &idle_task;

void scheduler_init(void) {
	current_task = &idle_task;
	idle_task.next = &idle_task;
	log_info("scheduler: init (round-robin)");
}

void scheduler_add(task_t* task) {
	if (!task) return;
	// Insert just after current in the circular list
	task->state = TASK_STATE_READY;
	task->next = current_task->next;
	current_task->next = task;
}

task_t* scheduler_current(void) { return current_task; }

void scheduler_tick(void) {
	// Simple round-robin: advance if next is ready
	if (!current_task) current_task = &idle_task;
	if (current_task->next && current_task->next->state == TASK_STATE_READY) {
		current_task->state = TASK_STATE_READY;
		current_task = current_task->next;
		current_task->state = TASK_STATE_RUNNING;
	}
}



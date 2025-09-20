#include "include/syscalls.h"
#include "include/log.h"
#include "include/types.h"
#include "include/task.h"

extern volatile u32 timer_ticks_per_sec; // from timer driver init
extern volatile u32 timer_ticks_total;   // total ticks since boot

void syscalls_init(void) {
	log_info("syscalls: init (stubs)");
}

u64 sys_uptime_ms(void) {
	if (timer_ticks_per_sec == 0) return 0;
	// Avoid 64-bit division for now - simple approximation
	u32 ms = (timer_ticks_total * 1000) / timer_ticks_per_sec;
	return (u64)ms;
}

void sys_list_tasks(void) {
	// Simple dump of round-robin list starting at current
	task_t* cur = scheduler_current();
	if (!cur) { log_info("tasks: none"); return; }
	log_info("tasks: listing");
	task_t* it = cur;
	int count = 0;
	do {
		// Minimal print
		log_puts(" - task id=");
		// hex/id print simplified
		log_puts("...");
		log_puts(" state=");
		switch (it->state) {
			case TASK_STATE_READY: log_puts("READY\n"); break;
			case TASK_STATE_RUNNING: log_puts("RUNNING\n"); break;
			case TASK_STATE_SLEEPING: log_puts("SLEEPING\n"); break;
			case TASK_STATE_ZOMBIE: log_puts("ZOMBIE\n"); break;
			default: log_puts("?\n"); break;
		}
		it = it->next; count++;
	} while (it && it != cur && count < 64);
}

void sys_shutdown(void) {
	log_error("shutdown: not implemented; halting CPU");
	for(;;) { __asm__ volatile ("hlt"); }
}



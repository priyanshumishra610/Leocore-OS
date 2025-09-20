#pragma once
#include "types.h"

// Initialize syscall subsystem (if needed)
void syscalls_init(void);

// System information
u64 sys_uptime_ms(void);

// Task management debug
void sys_list_tasks(void);

// Power
void sys_shutdown(void);



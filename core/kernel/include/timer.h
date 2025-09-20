#pragma once
#include "types.h"

void timer_init(u32 freq_hz);
void timer_tick(void);

extern volatile u32 timer_ticks_total;
extern volatile u32 timer_ticks_per_sec;

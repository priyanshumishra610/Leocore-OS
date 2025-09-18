#pragma once
#include "types.h"

void cpu_get_vendor(char out[13]);
#define cpu_vendor(out) cpu_get_vendor(out)

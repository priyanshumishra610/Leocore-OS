#include "include/log.h"
#include "include/vga.h"
#include "include/serial.h"

void log_init(void) { serial_init(); vga_init(); }

void log_puts(const char* s) { vga_write(s); serial_write(s); }

void log_info(const char* s) { log_puts("[INFO] "); log_puts(s); log_puts("\n"); }

void log_warn(const char* s) { log_puts("[WARN] "); log_puts(s); log_puts("\n"); }

void log_error(const char* s) { log_puts("[ERR ] "); log_puts(s); log_puts("\n"); }

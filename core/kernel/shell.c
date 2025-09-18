#include "include/log.h"
#include "include/vga.h"
#include "include/ps2.h"

static void prompt(void) { log_puts("\nleo> "); }

static void handle_cmd(const char* s) {
	if (!s[0]) return;
	if (s[0]=='h') { log_info("help: help, about, clear, panic"); }
	else if (s[0]=='a') { log_info("LeoCore OS - Embryo Phase 1"); }
	else if (s[0]=='c') { vga_clear(); }
	else if (s[0]=='p') { extern void panic(const char*); panic("Kernel simulated panic: curiosity overflow"); }
	else { log_warn("unknown command"); }
}

void shell_run(void) {
	char buf[64]; int len = 0;
	prompt();
	for (;;) {
		char c = kb_getchar();
		if (!c) continue;
		if (c == '\n') { buf[len]=0; handle_cmd(buf); len=0; prompt(); continue; }
		if (c == '\b') { if (len>0) { len--; log_puts("\b "); } continue; }
		if (len < (int)sizeof(buf)-1) { buf[len++]=c; char s[2]={c,0}; log_puts(s); }
	}
}


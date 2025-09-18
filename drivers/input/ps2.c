#include <stdint.h>
#include "../../core/kernel/include/io.h"
#include "../../core/kernel/include/log.h"
#include "../../core/kernel/include/types.h"

#define KBD_DATA 0x60

static volatile char kb_buf[128];
static volatile u32 kb_head = 0, kb_tail = 0;

static const char scancode_set1[128] = {
	0,27,'1','2','3','4','5','6','7','8','9','0','-','=', '\b',
	'\t','q','w','e','r','t','y','u','i','o','p','[',']','\n',0,
	'a','s','d','f','g','h','j','k','l',';','\'','`',0,'\\','z','x','c','v','b','n','m',',','.','/',0,'*',0,' ',
};

void ps2_init(void) {
}

void ps2_handle_irq(void) {
	uint8_t sc = inb(KBD_DATA);
	if (!(sc & 0x80)) {
		char c = sc < 128 ? scancode_set1[sc] : 0;
		if (c) {
			kb_buf[kb_head & 127] = c;
			kb_head++;
		}
	}
}

char kb_getchar(void) {
	if (kb_tail == kb_head) return 0;
	char c = kb_buf[kb_tail & 127];
	kb_tail++;
	return c;
}

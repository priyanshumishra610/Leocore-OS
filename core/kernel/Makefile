# Kernel build (i386, Multiboot v1)

# Try cross-compilers; allow environment CC/LD to override
CC ?= $(firstword $(foreach c,i686-elf-gcc i386-elf-gcc gcc,$(if $(shell command -v $(c) 2>/dev/null),$(c),)))
LD ?= $(firstword $(foreach c,i686-elf-ld i386-elf-ld ld,$(if $(shell command -v $(c) 2>/dev/null),$(c),)))
AS := $(CC)

CFLAGS_COMMON := -ffreestanding -fno-stack-protector -fno-pic -m32 -Wall -Wextra -Werror -nostdlib -nodefaultlibs
ASFLAGS := -m32 -x assembler-with-cpp

ifdef RELEASE
	CFLAGS := $(CFLAGS_COMMON) -O2 -DLEO_RELEASE=1
else
	CFLAGS := $(CFLAGS_COMMON) -O0 -g -DLEO_RELEASE=0
endif

LDFLAGS := -T linker.ld -nostdlib -m elf_i386

INCLUDE := -I include

SRCS_C = \
	main.c \
	shell.c \
	gdt.c \
	idt.c \
	irq.c \
	pic.c \
	log.c \
	panic.c \
	mem/bump.c \
	mem/kmalloc.c \
	../../drivers/timer.c \
	../../drivers/vga.c \
	../../drivers/serial.c \
	../../drivers/input/ps2.c

SRCS_S = \
	gdt_flush.S \
	irq_stub.S \
	idt_load.S \
	cpu.S \
	io.S \
	interrupts.S \
	../bootloader/boot.S

OBJS := $(SRCS_C:.c=.o) $(SRCS_S:.S=.o)

TARGET = kernel.elf

all: $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

%.o: %.S
	$(AS) $(ASFLAGS) $(INCLUDE) -c $< -o $@

$(TARGET): $(OBJS) linker.ld
	$(LD) $(LDFLAGS) -o $(TARGET) $(OBJS)

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean

# Kernel build (i386, Multiboot v1)

# Try common cross-compilers, fallback to system gcc with -m32
CC := $(firstword $(foreach c,i686-elf-gcc i386-elf-gcc gcc,$(if $(shell command -v $(c) 2>/dev/null),$(c),)))
LD := $(firstword $(foreach c,i686-elf-ld i386-elf-ld ld,$(if $(shell command -v $(c) 2>/dev/null),$(c),)))
AS := $(CC)

CFLAGS_COMMON := -ffreestanding -fno-stack-protector -fno-pic -m32 -Wall -Wextra -Werror -nostdlib -nostartfiles -nodefaultlibs
ASFLAGS := -m32 -x assembler-with-cpp

ifdef RELEASE
	CFLAGS := $(CFLAGS_COMMON) -O2
else
	CFLAGS := $(CFLAGS_COMMON) -O0 -g
endif

LDFLAGS := -T linker.ld -nostdlib -melf_i386

INCLUDE := -I include

SRCS_C = \
	main.c \
	gdt.c \
	idt.c \
	irq.c \
	pic.c \
	timer.c \
	vga.c \
	serial.c \
	panic.c \
	log.c \
	mem/bump.c \
	mem/kmalloc.c

SRCS_S = \
	gdt.S \
	irq_stub.S \
	idt_load.S

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

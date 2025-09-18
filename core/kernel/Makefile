TARGET = kernel.elf
CC = x86_64-elf-gcc
LD = x86_64-elf-ld
AS = x86_64-elf-as
CFLAGS = -ffreestanding -fno-stack-protector -fno-pic -mno-red-zone -m64 -Wall -Wextra -nostdlib -nostartfiles -nodefaultlibs
LDFLAGS = -T linker.ld -nostdlib

OBJS = boot.o main.o

all: $(TARGET)

boot.o: ../bootloader/boot.S
	$(CC) $(CFLAGS) -c $< -o $@

main.o: main.c
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET): $(OBJS) linker.ld
	$(LD) $(LDFLAGS) -o $(TARGET) $(OBJS)

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean

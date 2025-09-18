SHELL := /bin/sh
.ONESHELL:

# Toolchain detection (prefer cross compilers, fall back to system gcc)
CC_CANDIDATES := i686-elf-gcc i386-elf-gcc gcc
LD_CANDIDATES := i686-elf-ld i386-elf-ld ld

find_tool = $(firstword $(foreach c,$(1),$(if $(shell command -v $(c) 2>/dev/null),$(c),)))

CC_CHOSEN := $(call find_tool,$(CC_CANDIDATES))
LD_CHOSEN := $(call find_tool,$(LD_CANDIDATES))

export CC_CHOSEN
export LD_CHOSEN

# Print selection info once
$(info [build] CC=$(CC_CHOSEN) LD=$(LD_CHOSEN))
ifeq ($(CC_CHOSEN),gcc)
$(warning [build] Falling back to system gcc (using -m32). Install a cross-compiler for reproducible builds.)
endif

.PHONY: all build run clean debug nographic

all: build

build:
	./scripts/build.sh

run:
	./scripts/run_qemu.sh

nographic:
	./scripts/run_qemu.sh --nographic

debug:
	./scripts/run_qemu.sh --debug --nographic

clean:
	./scripts/clean.sh

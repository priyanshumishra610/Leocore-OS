SHELL := /bin/sh
.ONESHELL:

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

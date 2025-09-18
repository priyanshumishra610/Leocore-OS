#!/bin/sh
set -e

ROOT_DIR=$(CDPATH= cd -- "$(dirname -- "$0")/.." && pwd)
BUILD_DIR="$ROOT_DIR/build"
ISO_PATH="$BUILD_DIR/leocore-os.iso"
KERNEL_PATH="$ROOT_DIR/core/kernel/kernel.elf"

if [ -f "$ISO_PATH" ]; then
	echo "[+] Booting ISO in QEMU"
	qemu-system-x86_64 -cdrom "$ISO_PATH" -m 256 -serial stdio
else
	echo "[!] ISO not found at $ISO_PATH. Trying direct multiboot kernel load."
	qemu-system-x86_64 -kernel "$KERNEL_PATH" -m 256 -serial stdio
fi

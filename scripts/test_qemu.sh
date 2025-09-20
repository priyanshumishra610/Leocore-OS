#!/bin/sh
set -eu

ROOT_DIR=$(CDPATH= cd -- "$(dirname -- "$0")/.." && pwd)
ISO_PATH="$ROOT_DIR/build/leocore-os.iso"
KERNEL_PATH="$ROOT_DIR/core/kernel/kernel.elf"

if [ ! -f "$KERNEL_PATH" ]; then
	echo "[test] Kernel not built. Building..."
	"$ROOT_DIR/scripts/build.sh"
fi

# Run QEMU quickly to see banner (headless)
qemu-system-i386 -kernel "$KERNEL_PATH" -nographic -serial stdio -monitor none -d guest_errors -no-reboot -no-shutdown -append ""

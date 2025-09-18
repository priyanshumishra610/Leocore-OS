#!/bin/sh
set -eu

usage() {
	echo "Usage: $0 [--iso|--kernel] [--debug] [--nographic] [--accel hvf|kvm|tcg]";
	echo "  --iso        Boot the generated ISO (default if exists)";
	echo "  --kernel     Boot the kernel.elf directly (Multiboot)";
	echo "  --debug      Add -s -S for GDB (listen on :1234 and pause)";
	echo "  --nographic  Run headless (CI-friendly)";
	echo "  --accel X    Force accelerator: hvf (macOS), kvm (Linux), tcg (software)";
}

MODE=auto
DEBUG=0
NOGRAPHIC=0
ACCEL=auto

for arg in "$@"; do
	case "$arg" in
		--help) usage; exit 0 ;;
		--iso) MODE=iso ;;
		--kernel) MODE=kernel ;;
		--debug) DEBUG=1 ;;
		--nographic) NOGRAPHIC=1 ;;
		--accel) shift; ACCEL=${1-}; if [ -z "$ACCEL" ]; then echo "[!] --accel requires value"; exit 1; fi ;;
	esac
	shift || true
done

ROOT_DIR=$(CDPATH= cd -- "$(dirname -- "$0")/.." && pwd)
BUILD_DIR="$ROOT_DIR/build"
ISO_PATH="$BUILD_DIR/leocore-os.iso"
KERNEL_PATH="$ROOT_DIR/core/kernel/kernel.elf"

QEMU_ARGS="-m 256 -serial stdio"

# Accelerator selection
if [ "$ACCEL" = auto ]; then
	if [ "$(uname -s)" = "Darwin" ]; then ACCEL=hvf; else ACCEL=kvm; fi
fi
if [ "$ACCEL" = hvf ] || [ "$ACCEL" = kvm ] || [ "$ACCEL" = tcg ]; then
	QEMU_ARGS="$QEMU_ARGS -accel $ACCEL"
fi

[ "$DEBUG" -eq 1 ] && QEMU_ARGS="$QEMU_ARGS -s -S"
[ "$NOGRAPHIC" -eq 1 ] && QEMU_ARGS="$QEMU_ARGS -nographic"

if [ "$MODE" = auto ]; then
	if [ -f "$ISO_PATH" ]; then MODE=iso; else MODE=kernel; fi
fi

echo "[+] QEMU mode: $MODE (accel=$ACCEL debug=$DEBUG nographic=$NOGRAPHIC)"

if [ "$MODE" = iso ]; then
	if [ ! -f "$ISO_PATH" ]; then echo "[!] ISO not found at $ISO_PATH"; exit 1; fi
	exec qemu-system-x86_64 -cdrom "$ISO_PATH" $QEMU_ARGS
else
	if [ ! -f "$KERNEL_PATH" ]; then echo "[!] Kernel not found at $KERNEL_PATH"; exit 1; fi
	exec qemu-system-x86_64 -kernel "$KERNEL_PATH" $QEMU_ARGS
fi

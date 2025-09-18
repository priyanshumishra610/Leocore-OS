#!/bin/sh
set -eu

usage() {
	echo "Usage: $0 [--release]";
	echo "  --release  Build with optimizations (passes to kernel Makefile)";
}

MODE=dev
if [ "${1-}" = "--help" ]; then
	usage; exit 0
elif [ "${1-}" = "--release" ]; then
	MODE=release
fi

ROOT_DIR=$(CDPATH= cd -- "$(dirname -- "$0")/.." && pwd)
BUILD_DIR="$ROOT_DIR/build"
ISO_DIR="$BUILD_DIR/isodir"
BOOT_DIR="$ISO_DIR/boot"
GRUB_DIR="$BOOT_DIR/grub"
ISO_PATH="$BUILD_DIR/leocore-os.iso"

need() { command -v "$1" >/dev/null 2>&1 || { echo "[!] Missing tool: $1"; exit 1; }; }

# Check required tools
need make
need x86_64-elf-gcc
need x86_64-elf-ld
if ! command -v grub-mkrescue >/dev/null 2>&1; then
	echo "[!] grub-mkrescue not found. ISO creation may fail on macOS without GRUB tools."
fi

mkdir -p "$BUILD_DIR" "$BOOT_DIR" "$GRUB_DIR"

echo "[+] Building kernel (mode: $MODE)"
KMODE=
if [ "$MODE" = release ]; then KMODE=RELEASE=1; fi
( cd "$ROOT_DIR/core/kernel" && make $KMODE )

if [ ! -f "$ROOT_DIR/core/kernel/kernel.elf" ]; then
	echo "[!] kernel.elf not found after build"; exit 1
fi

cp "$ROOT_DIR/core/kernel/kernel.elf" "$BOOT_DIR/"
cp "$ROOT_DIR/core/bootloader/grub.cfg" "$GRUB_DIR/"

echo "[+] Creating ISO at $ISO_PATH"
if command -v grub-mkrescue >/dev/null 2>&1; then
	grub-mkrescue -o "$ISO_PATH" "$ISO_DIR"
else
	echo "[!] grub-mkrescue missing; skipping ISO creation. You can still run QEMU with -kernel."
fi

echo "[+] Build completed"

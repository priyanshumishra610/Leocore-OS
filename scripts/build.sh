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

# Required base tools
need make

# Toolchain detection (prefer 32-bit cross compilers)
CC_CHOSEN=${CC_CHOSEN-}
LD_CHOSEN=${LD_CHOSEN-}

find_tool() {
	for t in "$@"; do
		if command -v "$t" >/dev/null 2>&1; then echo "$t"; return 0; fi
	done
	printf "" # empty
}

if [ -z "${CC_CHOSEN}" ]; then
	CC_CHOSEN=$(find_tool i686-elf-gcc i386-elf-gcc x86_64-elf-gcc gcc)
fi
if [ -z "${LD_CHOSEN}" ]; then
	LD_CHOSEN=$(find_tool i686-elf-ld i386-elf-ld x86_64-elf-ld ld)
fi

if [ -z "${CC_CHOSEN}" ] || [ -z "${LD_CHOSEN}" ]; then
	echo "[!] Could not find a suitable compiler/linker (tried i686-elf, i386-elf, x86_64-elf, system)."; exit 1
fi

echo "[build] CC=${CC_CHOSEN} LD=${LD_CHOSEN}"
case "$CC_CHOSEN" in
	gcc) echo "[warn] Falling back to system gcc (-m32). Install a cross-compiler for reproducible builds.";;
	*) : ;;
 esac

# Optional: ISO creation tools
if ! command -v grub-mkrescue >/dev/null 2>&1; then
	echo "[!] grub-mkrescue not found. ISO creation will be skipped. Use QEMU -kernel mode."
fi

mkdir -p "$BUILD_DIR" "$BOOT_DIR" "$GRUB_DIR"

echo "[+] Building kernel (mode: $MODE)"
KMODE=
if [ "$MODE" = release ]; then KMODE=RELEASE=1; fi
# Export chosen tools via env and build in kernel dir
env CC="$CC_CHOSEN" LD="$LD_CHOSEN" make -C "$ROOT_DIR/core/kernel" $KMODE

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

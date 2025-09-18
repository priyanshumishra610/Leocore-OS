#!/bin/sh
set -e

ROOT_DIR=$(CDPATH= cd -- "$(dirname -- "$0")/.." && pwd)
BUILD_DIR="$ROOT_DIR/build"
ISO_DIR="$BUILD_DIR/isodir"
BOOT_DIR="$ISO_DIR/boot"
GRUB_DIR="$BOOT_DIR/grub"

mkdir -p "$BUILD_DIR" "$BOOT_DIR" "$GRUB_DIR"

# Build kernel
echo "[+] Building kernel"
(cd "$ROOT_DIR/core/kernel" && make)

# Copy kernel and GRUB config
cp "$ROOT_DIR/core/kernel/kernel.elf" "$BOOT_DIR/"
cp "$ROOT_DIR/core/bootloader/grub.cfg" "$GRUB_DIR/"

# Create ISO (requires grub-mkrescue or xorriso + grub tools)
ISO_PATH="$BUILD_DIR/leocore-os.iso"

echo "[+] Creating ISO at $ISO_PATH"
if command -v grub-mkrescue >/dev/null 2>&1; then
	grub-mkrescue -o "$ISO_PATH" "$ISO_DIR"
elif command -v xorriso >/dev/null 2>&1; then
	# Fallback raw ISO creation (not a full El Torito bootable image without grub binaries)
	xorriso -as mkisofs -R -b boot/grub/i386-pc/eltorito.img -no-emul-boot -boot-load-size 4 -boot-info-table -o "$ISO_PATH" "$ISO_DIR" || true
	printf "[!] grub-mkrescue not found. Install it for a bootable ISO.\n"
else
	echo "[!] Neither grub-mkrescue nor xorriso found. Please install grub-mkrescue."
	exit 1
fi

echo "[+] Done."

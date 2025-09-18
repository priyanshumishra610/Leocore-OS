#!/bin/sh
set -eu

ROOT_DIR=$(CDPATH= cd -- "$(dirname -- "$0")/.." && pwd)
BUILD_DIR="$ROOT_DIR/build"

echo "[+] Cleaning build artifacts"
rm -rf "$BUILD_DIR"

if [ -d "$ROOT_DIR/core/kernel" ]; then
	( cd "$ROOT_DIR/core/kernel" && make clean || true )
fi

echo "[+] Clean complete"

#!/bin/sh
set -eu

# Launch GDB and connect to QEMU at :1234
# Usage: run QEMU with --debug (or make debug) first.

gdb -ex "target remote :1234" -ex "set architecture i386" "$@"

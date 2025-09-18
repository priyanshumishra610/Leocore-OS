# Testing & Debugging

## Quick run

```bash
make run
```

Headless/CI:
```bash
make nographic
```

## Debug with GDB

1. Start QEMU paused and listening:
```bash
make debug
```
This passes `-s -S` (waits for GDB on tcp::1234 and halts CPU).

2. In another terminal:
```bash
gdb -ex "target remote :1234" -ex "set architecture i386" -ex continue
```
Note: Multiboot v1 hands off in 32-bit protected mode initially; we compile with `-m64` only for future 64-bit transition. You can inspect early symbols and step from `_start` to `kernel_main`.

## ISO not created?
- Ensure `grub-mkrescue` is installed and available in PATH.
- On macOS, consider using a Linux container/VM for reliable GRUB tooling.
- You can still run directly:
```bash
qemu-system-x86_64 -kernel core/kernel/kernel.elf -serial stdio
```

## CI hint
- Use `./scripts/run_qemu.sh --nographic` for headless runs.
- Limit runtime and assert output by capturing `-serial stdio` log.

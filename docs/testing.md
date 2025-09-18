# Testing & Debugging

## Quick run

```bash
make run
```

Headless/CI:
```bash
make nographic
```

## Compiler fallback (CI-friendly)

- The build prefers cross-compilers (`i686-elf-gcc`/`i386-elf-gcc`).
- If they are not found, it falls back to system `gcc` with `-m32` flags.
- Selection is printed at build start (e.g., `CC=gcc LD=ld`).
- For reproducible builds across hosts, install a cross-compiler.

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
Note: Multiboot v1 hands off in 32-bit protected mode. You can inspect early symbols and step from `_start` to `kernel_main`.

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

# Phase 1 Deliverables

- GRUB + Multiboot boot path
- Minimal C kernel that prints to VGA: "LeoCore OS kernel loaded!"
- Scripts: build.sh, run_qemu.sh, clean.sh
- Root Makefile for build/run/clean
- Placeholders for AI, UI, drivers, apps, resources, tests

## Build

```bash
make        # builds kernel, creates ISO if grub-mkrescue present
```

## Run

```bash
make run            # GUI window if available
make nographic      # headless (CI-friendly)
make debug          # QEMU with -s -S for GDB
```

## Tooling

- Cross-compiler: x86_64-elf-gcc, x86_64-elf-ld
- Boot ISO: grub-mkrescue (recommended), xorriso (fallback)
- Emulator: qemu-system-x86_64

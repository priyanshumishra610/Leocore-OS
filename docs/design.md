# Design Overview (Phase 1 Advanced)

- Boot: GRUB Multiboot v1 -> `_start` -> `kernel_main`
- CPU: GDT (flat 32-bit), IDT with IRQ stubs, PIC remapped to 0x20-0x2F
- Timer: PIT @ 100Hz
- Output: VGA text driver (scroll, color), Serial COM1 for debug
- Logging & Panic: dual-channel to VGA + serial
- Input: PS/2 keyboard IRQ stub (reads scancode)
- Memory: Bump allocator + kmalloc wrapper (temporary)
- Init order: log -> GDT -> IDT -> PIC -> PIT -> kmalloc -> PS/2 -> sti -> ready

Phase 2: Awakening

This phase introduces paging, a round-robin scheduler, input drivers, and prepares the foundation for VFS and LeoShell 2.0. The codebase is modularized under `core/kernel` into `mem/`, `task/`, `drivers/`, and will be extended for `fs/` and `shell/` submodules.

Features (initial scaffolding)
- Paging init stub (`paging_init`) to be expanded to enable page tables.
- Round-robin scheduler with minimal task control block and tick-driven switch.
- Keyboard driver wrapper (PS/2 based) and timer tick integration.
- Build system updates to compile new modules.

Next steps
- Implement page directory/table setup and enable CR3/CR0.PG.
- Add syscall stubs (write, exit, fork) and trap gates.
- Introduce VFS with ramfs and `/proc` pseudo-files.
- LeoShell 2.0 with history, autocomplete, pipes and built-ins.
- Unit and integration tests with coverage target (>80%).

Testing strategy
- Unit tests per module in `tests/unit/` using hosted builds where possible and kernel-linked tests for low-level parts.
- QEMU integration tests in `tests/integration/` ensure boot banner and shell prompt.
- Coverage via `make coverage` using gcov/llvm-cov. CI fails if <80%.

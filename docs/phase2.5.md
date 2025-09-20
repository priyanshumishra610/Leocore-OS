Phase 2.5: Jarvis Assistant Integration

Overview
- Adds a Jarvis-like AI assistant reachable from the kernel shell via the `jarvis` command.
- A Python bridge (`ai/jarvis/bridge.py`) listens on stdio (serial-forwarded) and routes commands to Ollama or system handlers.

Run
1) In one terminal, start the kernel in QEMU with serial forwarded (default):
   ./scripts/run_qemu.sh --kernel --nographic
2) In another terminal, start the Jarvis bridge:
   ./scripts/ollama_bridge.sh

Usage
- In the kernel shell: `jarvis "What is LeoCore?"`
- Interactive: `jarvis` then continue sending prompts; exit with `exit` (planned).

Testing
- Python unit tests: `pytest -q` under repo root (ensure Python deps installed).
- Coverage target: >80% for ai/jarvis modules.

Design Notes
- Simple text protocol: kernel writes lines prefixed with `JARVIS:` to serial; the bridge reads stdin and emits responses.
- Short-term memory: last 10 exchanges are kept in `Session`.
- System commands are recognized and handled without hitting Ollama.
